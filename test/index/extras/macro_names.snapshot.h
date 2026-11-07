  // Macros are named by defining file and name, not line and column.
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/macro_names.h`/
  
  #define MACRO_ONE 1
//        ^^^^^^^^^ definition [..] `<file>/macro_names.h`/MACRO_ONE!
  
  #ifdef MACRO_TWO_ALT
  #define MACRO_TWO 2
  #else
  #define MACRO_TWO 22
//        ^^^^^^^^^ definition [..] `<file>/macro_names.h`/MACRO_TWO!
  #endif
  
  #undef MACRO_ONE
//       ^^^^^^^^^ reference [..] `<file>/macro_names.h`/MACRO_ONE!
  #define MACRO_ONE 11
//        ^^^^^^^^^ definition [..] `<file>/macro_names.h`/MACRO_ONE!
