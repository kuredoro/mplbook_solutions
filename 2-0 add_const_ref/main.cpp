#include <boost/type_traits/is_same.hpp>

using namespace boost;

template <class T>
struct add_const_ref
{
    typedef const T& type;
};

int main()
{
    // TODO: how does this work?
    static_assert(is_same<add_const_ref<int&>::type, int&>::value);

    static_assert(is_same<add_const_ref<int>::type, int const&>::value);
    static_assert(is_same<add_const_ref<int const&>::type, const int&>::value);
    static_assert(is_same<add_const_ref<int const>::type, const int&>::value);
    return 0;
}
