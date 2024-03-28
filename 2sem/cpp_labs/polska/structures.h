#include <forward_list>

using namespace std;

template<typename T>
struct Stack{
    forward_list<T> lst;
};

template<typename T>
void push(Stack<T> &s, T val)
{
    s.lst.push_front(val);
}

template<typename T>
T pop(Stack<T> &s)
{
  T element = s.lst.front();
  s.lst.pop_front();
  return element;
}

template<typename T>
T read(Stack<T> &s)
{
  return s.lst.front();
}

namespace util {
template<typename T>
bool is_empty(Stack<T> &s){
  return s.lst.empty();
}
}