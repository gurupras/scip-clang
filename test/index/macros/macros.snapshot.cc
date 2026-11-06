  // extra-args: -std=c++23
//^^^^^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/macros.cc`/
  
  #include "macros.h"
//         ^^^^^^^^^^ reference [..] `<file>/macros.h`/
  
  int a = MY_MACRO + MY_MACRO_ALIAS;
//    ^ definition [..] a.
//        ^^^^^^^^ reference [..] `<file>/macros.h`/MY_MACRO!
//                   ^^^^^^^^^^^^^^ reference [..] `<file>/macros.h`/MY_MACRO_ALIAS!
  
  #undef MY_MACRO
//       ^^^^^^^^ reference [..] `<file>/macros.h`/MY_MACRO!
  
  #if defined(MY_MACRO) // no reference
  #endif
  
  #define MY_MACRO 1
//        ^^^^^^^^ definition [..] `<file>/macros.cc`/MY_MACRO!
  
  int b = MY_MACRO;
//    ^ definition [..] b.
//        ^^^^^^^^ reference [..] `<file>/macros.cc`/MY_MACRO!
  
  #if defined(MY_MACRO)
//            ^^^^^^^^ reference [..] `<file>/macros.cc`/MY_MACRO!
  #endif
  
  #ifdef MY_MACRO
//       ^^^^^^^^ reference [..] `<file>/macros.cc`/MY_MACRO!
  #endif
  
  #ifndef MY_MACRO
//        ^^^^^^^^ reference [..] `<file>/macros.cc`/MY_MACRO!
  #endif
  
  #ifdef NOT_YET_DEFINED_MACRO
  #elifndef MY_MACRO
//          ^^^^^^^^ reference [..] `<file>/macros.cc`/MY_MACRO!
  #elifdef MY_MACRO
//         ^^^^^^^^ reference [..] `<file>/macros.cc`/MY_MACRO!
  #endif
  
  #define NOT_YET_DEFINED_MACRO
//        ^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/macros.cc`/NOT_YET_DEFINED_MACRO!
  
  #define IDENTITY_MACRO(__x) __x
//        ^^^^^^^^^^^^^^ definition [..] `<file>/macros.cc`/IDENTITY_MACRO!
  
  int c = IDENTITY_MACRO(10);
//    ^ definition [..] c.
//        ^^^^^^^^^^^^^^ reference [..] `<file>/macros.cc`/IDENTITY_MACRO!
  
  #define MACRO_USING_MACRO \
//        ^^^^^^^^^^^^^^^^^ definition [..] `<file>/macros.cc`/MACRO_USING_MACRO!
    IDENTITY_MACRO(0)
//  ^^^^^^^^^^^^^^ reference [..] `<file>/macros.cc`/IDENTITY_MACRO!
  
  // two uses of a macro that expands to another macro
  int d = MACRO_USING_MACRO + MACRO_USING_MACRO;
//    ^ definition [..] d.
//        ^^^^^^^^^^^^^^^^^ reference [..] `<file>/macros.cc`/MACRO_USING_MACRO!
//                            ^^^^^^^^^^^^^^^^^ reference [..] `<file>/macros.cc`/MACRO_USING_MACRO!
