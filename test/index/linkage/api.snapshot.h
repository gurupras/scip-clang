  extern "C" {
//^^^^^^ definition [..] `<file>/api.h`/
  int c_api(int value);
//    ^^^^^ declaration [..] c_api(7864480464b09eea).
//              ^^^^^ definition local 0
  extern int c_counter;
//           ^^^^^^^^^ declaration [..] c_counter.
  struct CValue { int value; };
//       ^^^^^^ definition [..] CValue#
//                    ^^^^^ definition [..] CValue#value.
  extern "C++" {
  int cpp_api(int value);
//    ^^^^^^^ declaration [..] cpp_api(7864480464b09eea).
//                ^^^^^ definition local 1
  int cpp_api(double value);
//    ^^^^^^^ declaration [..] cpp_api(620b9ac44d2573a6).
//                   ^^^^^ definition local 2
  }
  }
  namespace nested {
//          ^^^^^^ definition [..] nested/
  extern "C" {
  int nested_api(int value);
//    ^^^^^^^^^^ declaration [..] nested/nested_api(7864480464b09eea).
//                   ^^^^^ definition local 3
  }
  }
  extern "C" int single_api(int value);
//               ^^^^^^^^^^ declaration [..] single_api(7864480464b09eea).
//                              ^^^^^ definition local 4
