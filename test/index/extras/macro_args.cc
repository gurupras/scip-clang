// format-options: showDocs
// Macro arguments are filed where they are spelled; tokens from a macro
// body are filed at the expansion and marked generated.

void abort_();

#define CHECK(x) do { if (!(x)) abort_(); } while (0)

struct S { int *p; };

void f(S &s, int *q) {
  CHECK(s.p != nullptr);
  CHECK(q == s.p && q == s.p);
}

void g();
#define CALL_G() g()
#define WRAP(x) CHECK(x)
#define USE_Q CHECK(q)

void h(int *q) {
  CALL_G();
  WRAP(q != nullptr);
  USE_Q;
}

#define DECLARE_FIELD(type, name) type name
struct T {
  DECLARE_FIELD(int, count);
};

int useT(T &t) { return t.count; }
