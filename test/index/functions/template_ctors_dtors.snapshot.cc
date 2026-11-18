  // extra-args: -std=c++17
//^^^^^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/template_ctors_dtors.cc`/
  
  // The constructors and destructor of a class template should get the
  // same symbol at their declarations, their definitions and their uses.
  
  template <typename T, typename U>
//                   ^ definition local 0
//                               ^ definition local 1
  class Path {
//      ^^^^ definition [..] Path#
  public:
    Path(int x);
//  ^^^^ declaration [..] Path#Path(d4f767463ce0a6b3).
//           ^ definition local 2
    Path() : Path(0) {}
//  ^^^^ definition [..] Path#Path(49f6e7a06ebc5aa8).
    template <typename V> Path(V *v) {}
//                     ^ definition local 3
//                        ^^^^ definition [..] Path#Path(7e6e718d4f0ca9be).
//                             ^ reference local 3
//                                ^ definition local 4
    ~Path();
//  ^ declaration [..] Path#`~Path`(49f6e7a06ebc5aa8).
  
    struct Inner { Inner(int) {} };
//         ^^^^^ definition [..] Path#Inner#
//                 ^^^^^ definition [..] Path#Inner#Inner(d4f767463ce0a6b3).
    template <typename X> struct InnerT { InnerT(X) {} ~InnerT() {} };
//                     ^ definition local 5
//                               ^^^^^^ definition [..] Path#InnerT#
//                                        ^^^^^^ definition [..] Path#InnerT#InnerT(b07662a27bd562f9).
//                                               ^ reference local 5
//                                                     ^ definition [..] Path#InnerT#`~InnerT`(49f6e7a06ebc5aa8).
  };
  
  template <typename T, typename U>
//                   ^ definition local 6
//                               ^ definition local 7
  Path<T, U>::Path(int x) {
//^^^^ reference [..] Path#
//            ^^^^ definition [..] Path#Path(d4f767463ce0a6b3).
//                     ^ definition local 8
    struct Local { Local() {} };
//         ^^^^^ definition [..] Path#Path(d4f767463ce0a6b3).Local#
//                 ^^^^^ definition [..] Path#Path(d4f767463ce0a6b3).Local#Local(49f6e7a06ebc5aa8).
    Local l;
//  ^^^^^ reference [..] Path#Path(d4f767463ce0a6b3).Local#
//        ^ definition local 9
    auto f = [] {};
//       ^ definition local 10
    f();
//  ^ reference local 10
  }
  
  template <typename T, typename U>
//                   ^ definition local 11
//                               ^ definition local 12
  Path<T, U>::~Path() {}
//^^^^ reference [..] Path#
//            ^ definition [..] Path#`~Path`(49f6e7a06ebc5aa8).
  
  template <typename T>
//                   ^ definition (specialization) local 13
  class Path<T, T *> {
//      ^^^^ definition (specialization) [..] Path#
//           ^ reference local 13
//              ^ reference local 13
  public:
    Path(int x);
//  ^^^^ declaration (specialization) [..] Path#Path(d4f767463ce0a6b3).
//           ^ definition (specialization) local 14
    ~Path() {}
//  ^ definition (specialization) [..] Path#`~Path`(49f6e7a06ebc5aa8).
  };
  
  template <typename T>
//                   ^ definition local 15
  Path<T, T *>::Path(int x) {}
//^^^^ reference [..] Path#
//              ^^^^ definition (specialization) [..] Path#Path(d4f767463ce0a6b3).
//                       ^ definition (specialization) local 16
  
  template <>
  class Path<long, long> {
//      ^^^^ reference [..] Path#
//      ^^^^ definition (specialization) [..] Path#
  public:
    Path(int x) {}
//  ^^^^ definition (specialization) [..] Path#Path(d4f767463ce0a6b3).
//           ^ definition (specialization) local 17
    ~Path() {}
//  ^ definition (specialization) [..] Path#`~Path`(ced63f7c635d850d).
//   ^^^^ reference [..] Path#
  };
  
  struct Plain {
//       ^^^^^ definition [..] Plain#
    template <typename B> struct Member { Member(B); };
//                     ^ definition local 18
//                               ^^^^^^ definition [..] Plain#Member#
//                                        ^^^^^^ declaration [..] Plain#Member#Member(9b289cee16747614).
//                                               ^ reference local 18
  };
  
  template <typename B>
//                   ^ definition local 19
  Plain::Member<B>::Member(B) {}
//^^^^^ reference [..] Plain#
//       ^^^^^^ reference [..] Plain#Member#
//                  ^^^^^^ definition [..] Plain#Member#Member(9b289cee16747614).
//                         ^ reference local 19
  
  template <typename A>
//                   ^ definition local 20
  struct Outer {
//       ^^^^^ definition [..] Outer#
    template <typename B> struct Mid { Mid(A, B); };
//                     ^ definition local 21
//                               ^^^ definition [..] Outer#Mid#
//                                     ^^^ declaration [..] Outer#Mid#Mid(bf83631c5ba9d873).
//                                         ^ reference local 20
//                                            ^ reference local 21
  };
  
  template <typename A>
//                   ^ definition local 22
  template <typename B>
//                   ^ definition local 23
  Outer<A>::Mid<B>::Mid(A, B) {}
//^^^^^ reference [..] Outer#
//          ^^^ reference [..] Outer#Mid#
//                  ^^^ definition [..] Outer#Mid#Mid(bf83631c5ba9d873).
//                      ^ reference local 22
//                         ^ reference local 23
  
  void use_template_ctors() {
//     ^^^^^^^^^^^^^^^^^^ definition [..] use_template_ctors(49f6e7a06ebc5aa8).
    Path<char, int> p0(1);
//  ^^^^ reference [..] Path#
//                  ^^ definition local 24
//                  ^^ reference [..] Path#Path(d4f767463ce0a6b3).
    Path<char, int> p1;
//  ^^^^ reference [..] Path#
//                  ^^ definition local 25
//                  ^^ reference [..] Path#Path(49f6e7a06ebc5aa8).
    int i = 0;
//      ^ definition local 26
    Path<char, int> p2(&i);
//  ^^^^ reference [..] Path#
//                  ^^ definition local 27
//                  ^^ reference [..] Path#Path(7e6e718d4f0ca9be).
//                      ^ reference local 26
    p2.~Path();
//  ^^ reference local 27
//      ^^^^ reference [..] Path#
    Path<char, int>::Inner in(1);
//  ^^^^ reference [..] Path#
//                   ^^^^^ reference [..] Path#Inner#
//                         ^^ definition local 28
//                         ^^ reference [..] Path#Inner#Inner(d4f767463ce0a6b3).
    Path<char, int>::InnerT<double> it(1.0);
//  ^^^^ reference [..] Path#
//                   ^^^^^^ reference [..] Path#InnerT#
//                                  ^^ definition local 29
//                                  ^^ reference [..] Path#InnerT#InnerT(b07662a27bd562f9).
    Path<char, char *> ps(2);
//  ^^^^ reference [..] Path#
//                     ^^ definition local 30
//                     ^^ reference [..] Path#Path(d4f767463ce0a6b3).
    Path<long, long> pl(3);
//  ^^^^ reference [..] Path#
//                   ^^ definition local 31
//                   ^^ reference [..] Path#Path(d4f767463ce0a6b3).
    Plain::Member<int> pm(4);
//  ^^^^^ reference [..] Plain#
//         ^^^^^^ reference [..] Plain#Member#
//                     ^^ definition local 32
//                     ^^ reference [..] Plain#Member#Member(9b289cee16747614).
    Outer<int>::Mid<char> m(1, 'a');
//  ^^^^^ reference [..] Outer#
//              ^^^ reference [..] Outer#Mid#
//                        ^ definition local 33
//                        ^ reference [..] Outer#Mid#Mid(bf83631c5ba9d873).
  }
