#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/mpl/placeholders.hpp>
using namespace boost::mpl::placeholders;

#include "type_name.hpp"

namespace mpl = boost::mpl;

/// wrap carries a single type and is value-initializable. Hence, it can be
/// used to pass type metadata within the function argument types.
///
/// \see mpl::for_each
template <class T>
struct wrap
{};

struct print_type
{
    template <class T>
    void operator() (T)
    {
        std::cout << type_name<T>() << '\n';
    }

    template <class T>
    void operator() (wrap<T>)
    {
        std::cout << type_name<T>() << '\n';
    }
};

struct print_numbers
{
    template <class T>
    void operator() (T num)
    {
        std::cout << num << ' ';
    }
};

int main()
{
    using v1 = mpl::vector<char, short, int, unsigned, long, long long, int64_t*>;

    mpl::for_each<v1>(print_type());
    std::cout << std::endl;

    using v2 = mpl::vector<int&, long&, char*&>;

    /// compilation error, since an argument of type T in print_type::operator()(T)
    /// will have to be value-initialized, but references cannot be.
    /// Additionally, classes with no default constructors and void cannot
    /// be value-initialized.
    /// Value-initialization is kinda a zero-initialization, but if your class
    /// has a user-defined constructor, then _it_ will be called.
    ///
    /// mpl::for_each<v2>(print_type());

    mpl::for_each<
        mpl::transform<v2, wrap<_>>::type
    >(print_type());
    std::cout << std::endl;

    /// Or synonymously...
    mpl::for_each<v2, wrap<_>>(print_type());
    std::cout << std::endl;

    /// Checkin' that it is convenient to work with integral constants.
    using n1 = mpl::vector_c<int, 1, 2, 3, 4, 5>;
    mpl::for_each<n1>(print_numbers());
}
