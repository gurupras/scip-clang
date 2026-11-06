// format-options: showDocs
// A using-declaration's alias is related to its target by is_definition.

namespace lib {
struct Widget {};
int helper(int x) { return x; }
} // namespace lib

namespace app {
using lib::Widget;
using lib::helper;

int run() {
  Widget w;
  (void)w;
  return helper(1);
}
} // namespace app
