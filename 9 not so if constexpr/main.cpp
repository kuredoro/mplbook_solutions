#include <iostream>
#include <type_traits>
#include <boost/mpl/int.hpp>

namespace mpl = boost::mpl;

/// What we would like to have
///
/// template <class T>
/// void f(T x)
/// {
///     if (std::is_class<T>::value)
///     {
///         std::cout << T::value << '\n';
///     }
///     else
///     {
///         std::cout << x << '\n';
///     }
/// }
///
/// But the problem is that both branches are type-checked.
/// Suppose we invoke f(42), then T = int and int doesn't have
/// ::value member.
/// One of the solutions is to use template specialization.

template <bool>
struct f_impl
{
    template <class T>
    static void print(T x)
    {
        std::cout << "integral constant " << T::value << '\n';
    }
};

template <>
struct f_impl<false>
{
    template <class T>
    static void print(T x)
    {
        std::cout << "flat " << x << '\n';
    }
};

template <class T>
void f(T x)
{
    f_impl<std::is_class<T>::value>::print(x);
}

int main()
{
    f(42);
    f(mpl::int_<42>{});
}
