#include <boost/mpl/advance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/bool_fwd.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/integral_c_fwd.hpp>
#include <boost/mpl/lower_bound.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/bool.hpp>
#include <limits>

namespace mpl = boost::mpl;

template <int64_t N>
using number = mpl::integral_c<int64_t, N>;

template <class This, class Next>
struct fibonacci_iterator
{
    using category = mpl::forward_iterator_tag;
    using type = fibonacci_iterator<This, Next>;
    static constexpr auto value = This::value;
};

template <class This, class Next>
struct mpl::deref<fibonacci_iterator<This, Next>>
    : This
{};

template <class This, class Next>
struct mpl::next<fibonacci_iterator<This, Next>>
    : fibonacci_iterator<Next, typename mpl::plus<This, Next>::type>
{};

struct fib_tag {};

struct fibonacci_series
{
    using tag = fib_tag;
    using type = fibonacci_series;
};

template <>
struct mpl::begin_impl<fib_tag>
{
    template <class Fib>
    struct apply
        : fibonacci_iterator<number<0>, number<1>>
    {};
};

static_assert(mpl::deref<mpl::begin<fibonacci_series>::type>::type::value == 0);
static_assert(
        mpl::deref<
            mpl::advance<
                mpl::begin<fibonacci_series>::type,
                mpl::int_<1>
            >::type
        >::type::value == 1
);
static_assert(
        mpl::deref<
            mpl::advance<
                mpl::begin<fibonacci_series>::type,
                mpl::int_<2>
            >::type
        >::type::value == 1
);
static_assert(
        mpl::deref<
            mpl::advance<
                mpl::begin<fibonacci_series>::type,
                mpl::int_<3>
            >::type
        >::type::value == 2
);

template <>
struct mpl::front_impl<fib_tag>
{
    template <class Fib>
    struct apply
        : mpl::deref<typename mpl::begin<Fib>::type>::type
    {};
};

template <>
struct mpl::end_impl<fib_tag>
{
    template <class Fib>
    struct apply
        : fibonacci_iterator<number<-1>, number<-1>>
    {};
};

template <>
struct mpl::size_impl<fib_tag>
{
    template <class Fib>
    struct apply
        : number<100> // almost the limit with the compiler
                      // it seems lower_bound uses mpl::size
                      // but then fib_seq cannot be infinite :(
    {};
};

template <>
struct mpl::empty_impl<fib_tag>
{
    template <class Fib>
    struct apply : mpl::false_ {};
};


using n = mpl::lower_bound<fibonacci_series, number<10>>::type;

static_assert(n::value == 13);

using m = mpl::lower_bound<fibonacci_series, number<50>>::type;

static_assert(m::value == 55);

int main()
{
}
