  #include "declarations.h"
//^ definition [..] `<file>/declarations.cc`/
//         ^^^^^^^^^^^^^^^^ reference [..] `<file>/declarations.h`/
  
  int globalVar = 1;
//    ^^^^^^^^^ definition [..] globalVar.
  int C::staticMember = 2;
//    ^ reference [..] C#
//       ^^^^^^^^^^^^ definition [..] C#staticMember.
  void C::method() {}
//     ^ reference [..] C#
//        ^^^^^^ definition [..] C#method(49f6e7a06ebc5aa8).
  struct Forward { int x; };
//       ^^^^^^^ definition [..] Forward#
//                     ^ definition [..] Forward#x.
  
  void freeFunction(int v) {
//     ^^^^^^^^^^^^ definition [..] freeFunction(d4f767463ce0a6b3).
//                      ^ definition local 0
    Forward f{v};
//  ^^^^^^^ reference [..] Forward#
//          ^ definition local 1
//            ^ reference local 0
    C c;
//  ^ reference [..] C#
//    ^ definition local 2
    c.method();
//  ^ reference local 2
//    ^^^^^^ reference [..] C#method(49f6e7a06ebc5aa8).
    (void)(f.x + globalVar + C::staticMember + C::constMember);
//         ^ reference local 1
//           ^ reference [..] Forward#x.
//               ^^^^^^^^^ reference [..] globalVar.
//                           ^ reference [..] C#
//                              ^^^^^^^^^^^^ reference [..] C#staticMember.
//                                             ^ reference [..] C#
//                                                ^^^^^^^^^^^ reference [..] C#constMember.
  }
  
  void useForward(Forward *f);
//     ^^^^^^^^^^ declaration [..] useForward(d59dbfce827cfac0).
//                ^^^^^^^ reference [..] Forward#
//                         ^ definition local 3
