#include <type_traits>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal.hpp>

namespace mpl = boost::mpl;

template <int64_t N>
using number = mpl::integral_c<int64_t, N>;

template <class Seq>
using push_last_two_sum_t =
    typename mpl::push_back<
        Seq,
        typename mpl::plus<
            typename mpl::deref<
                typename mpl::advance<
                    typename mpl::begin<Seq>::type,
                    typename mpl::minus<
                        typename mpl::size<Seq>::type,
                        number<2>
                    >::type
                >::type
            >::type,
            typename mpl::back<Seq>::type
        >::type
    >::type;

template 
    < int Count
    , class Seq = mpl::vector<number<0>, number<1>>
    , bool t = mpl::size<Seq>::type::value == Count
    >
struct fibonacci_series
{};

template <int Count, class Seq>
struct fibonacci_series<Count, Seq, false>
    : fibonacci_series<Count, push_last_two_sum_t<Seq>>
{};

template <int Count, class Seq>
struct fibonacci_series<Count, Seq, true>
    : Seq
{};

// Corner cases
template <class Seq>
struct fibonacci_series<0, Seq, false>
    : mpl::vector<>
{};

template <class Seq>
struct fibonacci_series<1, Seq, false>
    : mpl::vector<number<0>>
{};


using seq = fibonacci_series<8>;

static_assert(mpl::size<seq>::value == 8, "");
// Okay, in book it says 21, but, like
// 1 2 3 4 5 6 7 8
// 0 1 1 2 3 5 8 13
static_assert(mpl::back<seq>::type::value == 13, "");


static_assert(mpl::equal<fibonacci_series<0>, mpl::vector<>>(), "");

static_assert(mpl::equal<fibonacci_series<1>, mpl::vector<number<0>>>());

static_assert(mpl::equal<fibonacci_series<2>, mpl::vector<number<0>, number<1>>>());

int main()
{
}
