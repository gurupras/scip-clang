  
// definition [..] `<file>/myheader.h`/
  /// Look, a function!
  void f();
//     ^ declaration [..] f(49f6e7a06ebc5aa8).
  
  /// Look, a struct!
  struct S;
//       ^ declaration [..] S#
  
  struct C {
//       ^ definition [..] C#
    /// Look, a method!
    void m();
//       ^ declaration [..] C#m(49f6e7a06ebc5aa8).
  };
  
  /// Look, a global!
  extern int Global;
//           ^^^^^^ declaration [..] Global.
