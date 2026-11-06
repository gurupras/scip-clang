  // format-options: showDocs
//^^^^^^^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/macro_args.cc`/
//documentation
//| File: macro_args.cc
  // Macro arguments are filed where they are spelled; tokens from a macro
  // body are filed at the expansion and marked generated.
  
  void abort_();
//     ^^^^^^ declaration [..] abort_(49f6e7a06ebc5aa8).
  
  #define CHECK(x) do { if (!(x)) abort_(); } while (0)
//        ^^^^^ definition [..] `<file>/macro_args.cc`/CHECK!
//        documentation
//        | No documentation available.
  
  struct S { int *p; };
//       ^ definition [..] S#
//       documentation
//       | No documentation available.
//                ^ definition [..] S#p.
//                documentation
//                | No documentation available.
  
  void f(S &s, int *q) {
//     ^ definition [..] f(cf31d733a5e22d48).
//     documentation
//     | No documentation available.
//       ^ reference [..] S#
//          ^ definition local 0
//                  ^ definition local 1
    CHECK(s.p != nullptr);
//  ^^^^^ reference (generated) [..] abort_(49f6e7a06ebc5aa8).
//  ^^^^^ reference [..] `<file>/macro_args.cc`/CHECK!
//        ^ reference local 0
//          ^ reference [..] S#p.
    CHECK(q == s.p && q == s.p);
//  ^^^^^ reference (generated) [..] abort_(49f6e7a06ebc5aa8).
//  ^^^^^ reference [..] `<file>/macro_args.cc`/CHECK!
//        ^ reference local 1
//             ^ reference local 0
//               ^ reference [..] S#p.
//                    ^ reference local 1
//                         ^ reference local 0
//                           ^ reference [..] S#p.
  }
  
  void g();
//     ^ declaration [..] g(49f6e7a06ebc5aa8).
  #define CALL_G() g()
//        ^^^^^^ definition [..] `<file>/macro_args.cc`/CALL_G!
//        documentation
//        | No documentation available.
  #define WRAP(x) CHECK(x)
//        ^^^^ definition [..] `<file>/macro_args.cc`/WRAP!
//        documentation
//        | No documentation available.
//                ^^^^^ reference [..] `<file>/macro_args.cc`/CHECK!
  #define USE_Q CHECK(q)
//        ^^^^^ definition [..] `<file>/macro_args.cc`/USE_Q!
//        documentation
//        | No documentation available.
//              ^^^^^ reference [..] `<file>/macro_args.cc`/CHECK!
  
  void h(int *q) {
//     ^ definition [..] h(11b0e290e57a7e53).
//     documentation
//     | No documentation available.
//            ^ definition local 2
    CALL_G();
//  ^^^^^^ reference (generated) [..] g(49f6e7a06ebc5aa8).
//  ^^^^^^ reference [..] `<file>/macro_args.cc`/CALL_G!
    WRAP(q != nullptr);
//  ^^^^ reference (generated) [..] abort_(49f6e7a06ebc5aa8).
//  ^^^^ reference [..] `<file>/macro_args.cc`/WRAP!
//       ^ reference local 2
    USE_Q;
//  ^^^^^ reference (generated) local 2
//  ^^^^^ reference (generated) [..] abort_(49f6e7a06ebc5aa8).
//  ^^^^^ reference [..] `<file>/macro_args.cc`/USE_Q!
  }
  
  #define DECLARE_FIELD(type, name) type name
//        ^^^^^^^^^^^^^ definition [..] `<file>/macro_args.cc`/DECLARE_FIELD!
//        documentation
//        | No documentation available.
  struct T {
//       ^ definition [..] T#
//       documentation
//       | No documentation available.
    DECLARE_FIELD(int, count);
//  ^^^^^^^^^^^^^ reference [..] `<file>/macro_args.cc`/DECLARE_FIELD!
//                     ^^^^^ definition [..] T#count.
//                     documentation
//                     | No documentation available.
  };
  
  int useT(T &t) { return t.count; }
//    ^^^^ definition [..] useT(7d10df9b8e05b38c).
//    documentation
//    | No documentation available.
//         ^ reference [..] T#
//            ^ definition local 3
//                        ^ reference local 3
//                          ^^^^^ reference [..] T#count.
