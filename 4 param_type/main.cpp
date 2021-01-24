#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/equal.hpp>
#include <type_traits>
#include <cassert>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

template <class T>
struct param_type_v1
    : mpl::eval_if<     // forwarding to selected transformation
        std::is_scalar<T>,  // is_scalar derives from bool_
        mpl::identity<T>,
        std::add_lvalue_reference<T const>
      > // ::type (if using if_ instead of eval_if)
{};

template <class T>
struct add_reference
{
    using type = T&;
};

template <class T>
struct param_type_v2
    : mpl::eval_if<
        mpl::or_<std::is_scalar<T>, std::is_reference<T>>,
        mpl::identity<T>,
        add_reference<T const>
      >
{};

template <class T>
using param_type = param_type_v2<T>;

template <class T>
struct holder
{
    using Object = typename param_type<T>::type;

    holder(Object x)
        : m_x(x)
    {
    }

    T const& get() const
    {
        return m_x;
    }

private:
    Object m_x;
};

int main()
{
    holder<int> a{42};
    
    int _b{42};
    holder<int&> b{_b};
    assert(a.get() == b.get());

    using args = mpl::vector<int, holder<int>, int&>;
    using param_types = mpl::transform<
        args,
        mpl::if_<
            mpl::or_<std::is_scalar<_1>, std::is_lvalue_reference<_1>>,
            mpl::identity<_1>,
            add_reference<std::add_const<_1>>
        >
    >::type;

    static_assert(mpl::equal<
        param_types, 
        mpl::vector<int, holder<int> const&, int&>
    >::value, "WTF?????");
}
