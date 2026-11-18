// extra-args: -std=c++17

// The constructors and destructor of a class template should get the
// same symbol at their declarations, their definitions and their uses.

template <typename T, typename U>
class Path {
public:
  Path(int x);
  Path() : Path(0) {}
  template <typename V> Path(V *v) {}
  ~Path();

  struct Inner { Inner(int) {} };
  template <typename X> struct InnerT { InnerT(X) {} ~InnerT() {} };
};

template <typename T, typename U>
Path<T, U>::Path(int x) {
  struct Local { Local() {} };
  Local l;
  auto f = [] {};
  f();
}

template <typename T, typename U>
Path<T, U>::~Path() {}

template <typename T>
class Path<T, T *> {
public:
  Path(int x);
  ~Path() {}
};

template <typename T>
Path<T, T *>::Path(int x) {}

template <>
class Path<long, long> {
public:
  Path(int x) {}
  ~Path() {}
};

struct Plain {
  template <typename B> struct Member { Member(B); };
};

template <typename B>
Plain::Member<B>::Member(B) {}

template <typename A>
struct Outer {
  template <typename B> struct Mid { Mid(A, B); };
};

template <typename A>
template <typename B>
Outer<A>::Mid<B>::Mid(A, B) {}

void use_template_ctors() {
  Path<char, int> p0(1);
  Path<char, int> p1;
  int i = 0;
  Path<char, int> p2(&i);
  p2.~Path();
  Path<char, int>::Inner in(1);
  Path<char, int>::InnerT<double> it(1.0);
  Path<char, char *> ps(2);
  Path<long, long> pl(3);
  Plain::Member<int> pm(4);
  Outer<int>::Mid<char> m(1, 'a');
}
