#include "declarations.h"

int globalVar = 1;
int C::staticMember = 2;
void C::method() {}
struct Forward { int x; };

void freeFunction(int v) {
  Forward f{v};
  C c;
  c.method();
  (void)(f.x + globalVar + C::staticMember + C::constMember);
}

void useForward(Forward *f);
