  // format-options: showDocs
//^^^^^^^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/using_alias.cc`/
//documentation
//| File: using_alias.cc
  // A using-declaration's alias is related to its target by is_definition.
  
  namespace lib {
//          ^^^ definition [..] lib/
//          documentation
//          | namespace lib
  struct Widget {};
//       ^^^^^^ definition [..] lib/Widget#
//       documentation
//       | No documentation available.
  int helper(int x) { return x; }
//    ^^^^^^ definition [..] lib/helper(7864480464b09eea).
//    documentation
//    | No documentation available.
//               ^ definition local 0
//                           ^ reference local 0
  } // namespace lib
  
  namespace app {
//          ^^^ definition [..] app/
//          documentation
//          | namespace app
  using lib::Widget;
//      ^^^ reference [..] lib/
//           ^^^^^^ definition [..] app/Widget#
//           documentation
//           | No documentation available.
//           relation definition [..] lib/Widget#
//           ^^^^^^ reference [..] lib/Widget#
  using lib::helper;
//      ^^^ reference [..] lib/
//           ^^^^^^ definition [..] app/helper(7864480464b09eea).
//           documentation
//           | No documentation available.
//           relation definition [..] lib/helper(7864480464b09eea).
//           ^^^^^^ reference [..] lib/helper(7864480464b09eea).
  
  int run() {
//    ^^^ definition [..] app/run(b126dc7c1de90089).
//    documentation
//    | No documentation available.
    Widget w;
//  ^^^^^^ reference [..] app/Widget#
//  documentation
//  | No documentation available.
//  relation definition [..] lib/Widget#
//         ^ definition local 1
    (void)w;
//        ^ reference local 1
    return helper(1);
//         ^^^^^^ reference [..] app/helper(7864480464b09eea).
//         documentation
//         | No documentation available.
//         relation definition [..] lib/helper(7864480464b09eea).
  }
  } // namespace app
