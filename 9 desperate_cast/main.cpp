#include <iostream>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include "type_name.hpp"

namespace mpl = boost::mpl;

/// Implementation for polymorphic types.
template <class T, class U>
T desperate_cast_impl2(U& x, mpl::true_)
{
    return dynamic_cast<T>(x);  //< legal iff U is polymorphic
}

/// Implementation for non-polymorphic types.
template <class T, class U>
T desperate_cast_impl2(U& x, mpl::false_)
{
    return static_cast<T>(x);
}

// Dispatcher
template <class T, class U>
T desperate_cast_impl(U& x)
{
    std::cout << "impl's U is " << type_name<U>() << '\n'; //< int const
    return desperate_cast_impl2<T>(
        x,
        boost::is_polymorphic<
            typename boost::remove_pointer<U>::type
        >());
}

/// Public interface
template <class T, class U>
T desperate_cast(U const& x)
{
    std::cout << "const ver called\n";
    return desperate_cast_impl<T>(x);
}

template <class T, class U>
T desperate_cast(U& x)
{
    return desperate_cast_impl<T>(x);
}

int main()
{
    [[maybe_unused]]
    double f = desperate_cast<double>(42);
}
