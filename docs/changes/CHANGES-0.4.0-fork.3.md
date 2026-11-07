# scip-clang 0.4.0-fork.3: what changes and why

This fork release changes what scip-clang records about C and C++ code, so
that an index answers code-navigation queries the way the compiler sees the
code: go-to-definition, find-references, callers and hover. Each change is
its own commit and explains itself in its message. This document collects
those explanations for review.

## Contents

| # | Change | Problem it fixes |
|---|---|---|
| 1 | `SymbolInformation.kind` and `Occurrence.enclosing_range` on definitions | Readers couldn't tell callables from values, or find the function containing a call |
| 2 | Macro arguments at their spelling; `Generated` on macro-body tokens | Tokens inside `CHECK(...)` had no occurrence of their own |
| 3 | `ForwardDefinition` on every non-defining declaration | Declarations looked like references, or like definitions |
| 4 | `Relationship{is_definition}` from a using-declaration's alias to its target | Uses through `using ns::X;` couldn't be followed to `ns::X` |
| 5 | Macros named `` `<file>/defs.h`/NAME! `` | Editing a header renamed every macro below the edit |
| 5b | A fork-only role bit on template specializations | A specialization looked like a second definition of its template |
| 6 | `position_encoding` set on every document | Readers had to guess the column unit |
| 6b | A header's representative FileID no longer depends on ASLR | References into re-included headers were dropped in some runs |
| 7 | Snapshot printer and tests | Makes the changes above visible and tested |

---

## 1. Symbol kinds and enclosing ranges

**Change.** Every definition's `SymbolInformation` gets a `kind`, decided
from the AST: Function, Method, StaticMethod, Constructor, Class, Struct,
Field, Variable, TypeAlias and so on. Every definition occurrence of a
function, class, struct, union, enum or using-declaration gets an
`enclosing_range` spanning the whole declaration:
- a function, from its return type or its `template <...>` header to its
  closing brace;
- a class, to its closing brace.

This needs scip `v0.5.0`, whose proto has both fields. It is additive over
the previously vendored `aa0e511d`.

**Why.**
- Without `enclosing_range`, a reader that wants "the function containing
  this call" has to guess the extent from the text.
- Without `kind`, a reader can't tell a call to a function from a use of a
  variable.

**Details worth reviewing.**
- **Macros.** Both ends of the range are taken through their expansion
  ranges. A function made by a macro gets the extent of the invocation, and
  gtest's `TEST(A, B) { ... }` gives `TestBody` the invocation through the
  closing brace.
- **No range.** Pure virtual functions and declarations without a body get
  no range. Nor does a range whose ends expand into different files.
- **Merging.** Merged records keep the smallest non-zero kind, and
  occurrences compare and hash on `enclosing_range`, so the order in which
  records are merged doesn't decide the output.

---

## 2. Macro arguments at their spelling; macro-body tokens are Generated

**Before.** Every token that came out of a macro expansion was filed at the
*expansion location*, the macro name at the use site:

```cpp
CHECK(ptr != nullptr);
// before: the reference to ptr is filed on `CHECK`
```

- Clicking `ptr` found nothing, because no occurrence covered it.
- Find-references listed the line, but pointed at `CHECK`.
- scip-clang de-duplicates occurrences by (range, symbol). So
  `CHECK(q == s.p && q == s.p)` produced **one** occurrence of `q`, not two.

**After.** Occurrences go where clangd puts them,
`SourceManager::getFileLoc(loc)`:

| Token comes from | Filed at |
|---|---|
| ordinary source | the token (unchanged) |
| a macro **argument** | where the argument is spelled, e.g. on `ptr` |
| a macro **body** | the outermost expansion, i.e. the macro name (unchanged) |

`getFileLoc` walks the macro stack. At each level it takes the spelling for
an argument expansion and the expansion for a body token, so nesting works:
- `WRAP(q)` where `#define WRAP(x) CHECK(x)`: `q` is filed on `q`.
- `USE_Q` where `#define USE_Q CHECK(q)`: `q` is filed on `USE_Q`, and is
  Generated (below).

This applies to references, definitions and forward declarations alike. For
example, `DECLARE_FIELD(int, count)` now defines `count` on `count`.

**`Generated` on macro-body tokens.** A macro-body token isn't written at the
location it is filed under. Without a marker, a reader can't tell "the user
wrote this call here" from "a macro expanded here and made this call". Every
call a macro makes would look like a call written at the macro name, on top
of the macro-name token itself. These occurrences now carry
`SymbolRole::Generated` (0x10).

The test: a token comes from a macro body when its file location differs
from its spelling location. Argument tokens are spelled exactly where they
are filed.

*Why `Generated` and not a private bit:* it is the SCIP role for "this
occurrence isn't something the user wrote here", and readers that don't
know it see an ordinary occurrence. Nothing else in scip-clang sets it.
*Reviewer question:* is reusing `Generated` acceptable, or should a
fork-private bit be used?

**Risk.**
- More occurrences, because de-duplication no longer collapses distinct
  arguments.
- Snapshots under `test/index/macros` change where arguments are involved.
- Ranges stay single-token, and arguments are always in the invocation's
  file, so the same-file ENFORCE in `fromNonEmpty` still holds.

---

## 3. Non-defining declarations get ForwardDefinition, never Definition

**Before.** A declaration that isn't a definition was recorded in one of two
wrong ways:

| Source | Previous role | Consequence |
|---|---|---|
| `void f();`, `struct S;`, a method declared in its class | **none** | Indistinguishable from a reference: a header of declarations reads like a header of uses |
| `extern int x;`, in-class `static int x;` | **Definition** | "Go to definition" of a global can land on the `extern` in a header instead of the definition in a `.cpp` |

**After.** Every non-defining declaration carries
`SymbolRole::ForwardDefinition` (0x40) and never `Definition`. A reference
whose target is only forward-declared keeps its own roles.

**Mechanism (the part to review closely).**
- **Why a blanket 0x40 would be wrong.** Forward declarations go into a
  per-TU *forward-declaration map*. Workers send it to the driver
  (`ForwardDeclIndex`), and the driver resolves each suffix against the
  definitions it has. The same map also holds *references* whose canonical
  decl is non-defining (`RefersToForwardDecl::check`, in `saveReference`).
  Setting 0x40 on everything when the map is emitted would mislabel those
  references.
- **Per-entry roles.** Each map entry carries its own roles. On the wire this
  is a new field, `int32 symbol_roles = 3`, in `ForwardDecl.Reference`
  (`proto/fwd_decls.proto`). The driver copies it onto the occurrence
  (`ForwardDeclOccurrence::addTo`):
  - `saveForwardDeclaration` sets `ForwardDefinition`, plus `Generated` if
    the declaration is macro-made.
  - The reference path passes the reference's own roles.
- **Declarations with a SymbolInformation.** `extern` and in-class `static`
  variables go through the new `saveDeclaration`. It shares
  `saveDefinitionOrDeclaration` with `saveDefinition`, so documentation, kind
  and external-symbol handling are unchanged. Only the role differs, and
  there is no `enclosing_range`. The rule is clang's own
  `VarDecl::isThisDeclarationADefinition() == DeclarationOnly`:
  - an in-class `static const int x = 3;` is a declaration;
  - `static constexpr int y = 4;` is implicitly inline, so it stays a
    definition;
  - C's tentative `int x;` stays a definition.
- **Guards.** ENFORCEs keep `ForwardDefinition` out of `saveReference` and
  `saveDefinition`.

**Not changed.**
- Pure virtual methods are still definitions. They are the only "body" an
  interface method has.
- How forward declarations resolve to definitions across TUs.

**Reader impact.** A reader should treat 0x40 as a declaration, not as a
reference. A reader that ignores the role sees what it saw before for
forward declarations.

---

## 4. `using ns::X;` gets `Relationship{X, is_definition}`

**Before.** `using lib::Widget;` inside `namespace app` makes an alias
symbol, `app/Widget#`, with a definition at the using and a reference to
`lib/Widget#` at the same range. Uses of `Widget` found through the using
point at **the alias**. Upstream does this on purpose, in `saveDeclRefExpr`
and `saveUsingTypeLoc`, to keep "which using brought this in". But the alias
had no Relationship, so a reader couldn't follow it to the target except by
guessing that the reference sharing its range is the target.

**After.**
- The alias's SymbolInformation carries `Relationship{target,
  is_definition: true}`. In SCIP, that means go-to-definition on this symbol
  goes to the target.
- Uses still point at the alias, so nothing upstream relies on changes.
- A self-relationship is skipped.

**Why not `is_reference`.** In SCIP, `is_reference` says that
find-references on one symbol should include the other. An alias isn't a
use of its target at every place the alias is used, and readers that turn
`is_reference` into reference or inheritance edges would get misleading
results. `is_definition` says exactly what a using-declaration means.

---

## 5. Macros named by file and name

**Before.** `` `path/to/defs.h:LINE:COL`! ``, the position of the
`#define`. Adding a line above a `#define` renamed that macro and every
macro below it. Every reference in every other file then changed, and any
stored cross-file link to it broke, though nothing about the macros
changed.

**After.** `` `<file>/path/to/defs.h`/NAME! ``: the macro's name, nested
under the file symbol scip-clang already emits for the defining file, for
example `` `<file>/macros.h`/ ``.
- **Name.** Read from the source at `MacroInfo::getDefinitionLoc()`, by
  scanning identifier characters.
- **Path.** Comes from the file's stable id, which is always relative
  (package-relative or synthetic), so no absolute paths appear. A file with
  no metadata, such as a system header known only by its absolute path, is
  named `<external>/HASH/basename`. That is the scheme scip-clang already
  uses for files outside the project. Before, such a macro was named like
  `` `<file>//usr/include/assert.h`/assert! ``.
- **Redefinitions.** Two `#define`s of one name in one file (an `#undef` and
  redefine, or two `#if` branches) now **share** a symbol, which is what a
  reader means by "the macro NAME from defs.h". Navigation offers both
  definitions. The same name defined in two files stays two symbols.

**Externals.**
- Macros from `<built-in>` and the command line were external symbols and
  stay so. Their names follow the same scheme, e.g.
  `` `<file>/<built-in>`/__GNUC__! ``.
- **New:** a macro defined in a file outside the project (a system or
  third-party header) and referenced from a project file becomes an external
  symbol too. A reader can then tell "defined outside this index" from
  "definition missing".
- External macro symbols carry kind `Macro`.

**Reader impact.** Macro symbol strings change, so indexes built before and
after this release don't share macro symbols.

---

## 5b. Template specializations carry a fork-only role bit

**Before.** scip-clang names a specialization with its primary template's
symbol, so that navigation from `Traits<X>` reaches `Traits`. That makes
`template <> struct Traits<X> { ... };` emit a **Definition of `Traits#`**
that looks exactly like the template's own definition. The same goes for:
- partial specializations;
- explicit function and variable specializations;
- members declared inside a specialization's body (`Traits#size().`).

A reader that has to pick "the" definition of `Traits#` can't tell the
template from its specializations.

**After.** Definitions and declarations of specializations, and of members
written inside them, carry role bit `1 << 24` (`scip::specializationRole`,
in `indexer/ScipExtras.h`). The symbol and the Definition role are
unchanged, so navigation behaves exactly as before.
- **Why a bit and not a new symbol:** renaming specializations would break
  navigation from `Traits<X>` to `Traits`, and diverge from every other
  scip-clang index.
- **Why a high bit:** so it can never collide with a role scip.proto adds
  later.
- **Reader rule:** this bit isn't a SCIP role. A reader should interpret it,
  for example as "not the primary definition", and shouldn't store it as a
  SCIP role.

**Implicit instantiations are excluded.** The visitor also sees implicit
instantiations, such as `zero<int>` or the members of `C<int>`, at the
primary template's own source ranges, and occurrences at one range merge
their roles. Only specializations written in the source count: partial, or
`template <>`, and members inside their bodies. Otherwise a primary
template's definition, such as `int C<T>::static_mono = 0;`, would get the
bit. `test/index/vars/templates` covers this.

*Reviewer question:* the rule covers:
- explicit and partial class specializations;
- explicit function and variable specializations;
- every decl whose context is a written class template specialization.

Is that too wide? It also marks template parameters and locals of a partial
specialization, which have no cross-file identity anyway.

---

## 6. `position_encoding = UTF8CodeUnitOffsetFromLineStart`

scip-clang's columns are Clang presumed-location columns: byte offsets from
the start of the line, i.e. UTF-8 code units. Documents left the encoding
unspecified, so a reader that works in UTF-16 columns, as LSP clients do,
had to guess. On a line with non-ASCII text before a token, a wrong guess
shifts the token. Every document the driver writes
(`IndexWriter::writeDocument`) now states the encoding. **Positions
themselves don't change.**

---

## 6b. A header's representative FileID no longer depends on ASLR

This is a fix to upstream behaviour.

**Before.** `ClangIdLookupMap` keeps one representative FileID per
(path, preprocessor hash), and the last one inserted wins. It is filled by
iterating an absl hash map keyed by FileID. absl seeds its hashing from an
address, so with ASLR the iteration order, and therefore the representative,
changed from run to run.

For a header entered several times with one hash, such as `<stddef.h>` or
`<assert.h>`, only one entry holds the declarations that survived the
include guards. When a different entry was the representative, those
declarations had no `FileMetadata`, and every reference to them (`size_t`,
`__assert_fail`, `assert`) was silently dropped from the whole file. Two
identical runs could therefore produce different indexes.

**After.** The first-entered (smallest) FileID is kept. That is
deterministic, and it is the entry that holds the declarations.
`lookupAnyFileId` takes the smallest FileID across hashes for the same
reason. With this, repeated `--jobs=1` runs produce the same index.

**Not changed.** At `--jobs>1` the driver still gives each header to the
first TU whose semantic analysis reports it, so which TU supplies a header
can depend on scheduling. This is upstream's pipelined design, and
`--deterministic` documents that work scheduling isn't deterministic.

---

## 7. Tests

**Printer.** The snapshot printer now prints:
- `declaration` for a `ForwardDefinition` occurrence;
- `(generated)` for a `Generated` one;
- `(specialization)` for the specialization bit.

Without these, the role changes above would be invisible in snapshots.

**New cases in `test/index/extras`:**

| File | Covers |
|---|---|
| `macro_args.cc` | arguments at their spelling; repeated arguments not collapsed; body tokens generated; nested macros; a field declared through a macro |
| `declarations.{h,cc}` | forward, in-class, `extern` and `extern "C"` declarations vs their definitions; `static const` vs `static constexpr` members; class, partial and function specializations |
| `using_alias.cc` | `relation definition` lines on aliases |
| `macro_names.{h,cc}` | file-and-name symbols across `#undef`/`#if` redefinitions |

The snapshot updates for existing tests are in their own commit, so the diff
shows exactly how each change alters upstream's expected output.

## How to verify

1. `bazel test //test:test_index --config=dev` passes.
2. Read the snapshot-update commit. Every changed line should be explained
   by one of the changes above.
3. Index a project twice at `--jobs=1` with `--deterministic`. The two
   indexes should have the same documents and occurrences.

## Known limits

- `Generated` is per occurrence. A symbol that is both written and
  macro-made on one line gets two occurrences, one of each.
- Merging redefinitions under one macro symbol is a deliberate trade-off
  (see 5).
- In a build where a header is compiled in several configurations, its
  document comes from whichever TU indexes it. That is unchanged.
