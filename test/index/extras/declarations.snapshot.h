  // Declarations that are not definitions are "declaration" (ForwardDefinition)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/declarations.h`/
  // occurrences, never definitions and never plain references.
  
  void freeFunction(int);
//     ^^^^^^^^^^^^ declaration [..] freeFunction(d4f767463ce0a6b3).
  struct Forward;
//       ^^^^^^^ declaration [..] Forward#
  
  struct C {
//       ^ definition [..] C#
    void method();
//       ^^^^^^ declaration [..] C#method(49f6e7a06ebc5aa8).
    static int staticMember;
//             ^^^^^^^^^^^^ declaration [..] C#staticMember.
    static const int constMember = 3;
//                   ^^^^^^^^^^^ declaration [..] C#constMember.
    static constexpr int inlineMember = 4;
//                       ^^^^^^^^^^^^ definition [..] C#inlineMember.
  };
  
  extern int globalVar;
//           ^^^^^^^^^ declaration [..] globalVar.
  extern "C" int cFunction(int);
//               ^^^^^^^^^ declaration [..] cFunction(7864480464b09eea).
  
  void freeFunction(int);
//     ^^^^^^^^^^^^ declaration [..] freeFunction(d4f767463ce0a6b3).
  
  // Specializations share the primary template's symbol; they are marked
  // "(specialization)" so that consumers can tell them from the template.
  template <typename T> struct Traits { static int size(); };
//                   ^ definition local 0
//                             ^^^^^^ definition [..] Traits#
//                                                 ^^^^ declaration [..] Traits#size(b126dc7c1de90089).
  template <> struct Traits<char> { static int size(); };
//                   ^^^^^^ reference [..] Traits#
//                   ^^^^^^ definition (specialization) [..] Traits#
//                                             ^^^^ declaration (specialization) [..] Traits#size(b126dc7c1de90089).
  template <typename T> struct Traits<T *> { static int size(); };
//                   ^ definition (specialization) local 1
//                             ^^^^^^ definition (specialization) [..] Traits#
//                                    ^ reference local 1
//                                                      ^^^^ declaration (specialization) [..] Traits#size(b126dc7c1de90089).
  template <typename T> int sizeOf() { return Traits<T>::size(); }
//                   ^ definition local 2
//                          ^^^^^^ definition [..] sizeOf(b126dc7c1de90089).
//                                            ^^^^^^ reference [..] Traits#
  template <> int sizeOf<void>();
//                ^^^^^^ declaration (specialization) [..] sizeOf(b126dc7c1de90089).
