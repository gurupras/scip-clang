// Declarations that are not definitions are "declaration" (ForwardDefinition)
// occurrences, never definitions and never plain references.

void freeFunction(int);
struct Forward;

struct C {
  void method();
  static int staticMember;
  static const int constMember = 3;
  static constexpr int inlineMember = 4;
};

extern int globalVar;
extern "C" int cFunction(int);

void freeFunction(int);
