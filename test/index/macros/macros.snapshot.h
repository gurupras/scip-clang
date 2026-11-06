  #define MY_MACRO 0
//^ definition [..] `<file>/macros.h`/
//        ^^^^^^^^ definition [..] `<file>/macros.h`/MY_MACRO!
  
  #ifdef MY_MACRO
//       ^^^^^^^^ reference [..] `<file>/macros.h`/MY_MACRO!
  #endif
  
  #ifndef MY_MACRO
//        ^^^^^^^^ reference [..] `<file>/macros.h`/MY_MACRO!
  #endif
  
  #define MY_MACRO_ALIAS MY_MACRO
//        ^^^^^^^^^^^^^^ definition [..] `<file>/macros.h`/MY_MACRO_ALIAS!
//                       ^^^^^^^^ reference [..] `<file>/macros.h`/MY_MACRO!
