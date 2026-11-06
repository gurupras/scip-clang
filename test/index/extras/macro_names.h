// Macros are named by defining file and name, not line and column.

#define MACRO_ONE 1

#ifdef MACRO_TWO_ALT
#define MACRO_TWO 2
#else
#define MACRO_TWO 22
#endif

#undef MACRO_ONE
#define MACRO_ONE 11
