  #include "system_header.h"
//^ definition [..] `<file>/use_system_header.cc`/
//         ^^^^^^^^^^^^^^^^^ reference [..] `<file>/system_header.h`/
  
  const int total = SYSTEM_INT + OTHER_SYSTEM_INT;
//          ^^^^^ definition [..] total.
//                  ^^^^^^^^^^ reference [..] `<file>/system_header.h`/SYSTEM_INT!
//                               ^^^^^^^^^^^^^^^^ reference [..] `<file>/system_header.h`/OTHER_SYSTEM_INT!
