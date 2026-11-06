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

// Specializations share the primary template's symbol; they are marked
// "(specialization)" so that consumers can tell them from the template.
template <typename T> struct Traits { static int size(); };
template <> struct Traits<char> { static int size(); };
template <typename T> struct Traits<T *> { static int size(); };
template <typename T> int sizeOf() { return Traits<T>::size(); }
template <> int sizeOf<void>();
