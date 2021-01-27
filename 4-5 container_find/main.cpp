#include <algorithm>
#include <type_traits>
#include <vector>
#include <cassert>
#include <boost/mpl/if.hpp>

namespace mpl = boost::mpl;

// I'll use std lib here, just for diversity
template <class T>
using container_primary_iterator =
    std::conditional_t<
        std::is_const<T>::value,
        typename T::const_iterator,
        typename T::iterator
    >;

template <class Container, class Value>
container_primary_iterator<Container> container_find(Container& c, Value const& v)
{
    return std::find(c.begin(), c.end(), v);
}

// C++14 rules!!!!
template <class Container, class Value>
auto container_find2(Container& c, Value const& v)
{
    return std::find(c.begin(), c.end(), v);
}

using namespace std;

int main()
{
    vector<int> a{1, 2, 3, 4, 5};
    auto it1 = container_find(a, 4);
    assert(*it1 == 4);

    const vector<int> b{5, 1, 2, 4, 3};
    auto it2 = container_find(b, 2);
    assert(*it2 == 2);

    const vector<int> c{5, 1, 2, 4, 3};
    auto it3 = container_find2(c, 1);
    assert(*it3 == 1);

    return 0;
}
