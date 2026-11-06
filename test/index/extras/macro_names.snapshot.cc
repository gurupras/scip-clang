  #include "macro_names.h"
//^ definition [..] `<file>/macro_names.cc`/
//         ^^^^^^^^^^^^^^^ reference [..] `<file>/macro_names.h`/
  
  int values[] = {MACRO_ONE, MACRO_TWO};
//    ^^^^^^ definition [..] values.
//                ^^^^^^^ reference [..] `<file>/macro_names.h`/MACRO_ONE!
//                         ^^^^^^^ reference [..] `<file>/macro_names.h`/MACRO_TWO!
  int count = sizeof(values) / sizeof(values[0]);
//    ^^^^^ definition [..] count.
//                   ^^^^^^ reference [..] values.
//                                    ^^^^^^ reference [..] values.
