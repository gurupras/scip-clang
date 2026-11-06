  #pragma GCC system_header
//^ definition [..] `<file>/system_header.h`/
  
  #define SYSTEM_INT 0
//        ^^^^^^^^^^ definition [..] `<file>/system_header.h`/SYSTEM_INT!
  
  #define OTHER_SYSTEM_INT (SYSTEM_INT + 1)
//        ^^^^^^^^^^^^^^^^ definition [..] `<file>/system_header.h`/OTHER_SYSTEM_INT!
//                          ^^^^^^^^^^ reference [..] `<file>/system_header.h`/SYSTEM_INT!
