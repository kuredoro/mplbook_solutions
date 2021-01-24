#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/multiplies.hpp>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

/***********
 * Version 1
 ***********/
template <class N, class Predicate>
struct next_if
    : mpl::if_<
        typename mpl::apply<Predicate, N>::type,
        typename mpl::next<N>::type,
        N
      >
{};

static_assert(
    next_if<mpl::int_<0>, mpl::equal_to<_1, mpl::int_<0>>>::type::value == 1,
    "0 == 0 -> next(0)"
);

static_assert(
    next_if<mpl::int_<1>, mpl::equal_to<_1, mpl::int_<0>>>::type::value == 1,
    "1 != 0 -> identity(1)"
);

/***********
 * Version 2
 ***********/
template <class N, class Predicate>
struct next_if2
    : mpl::eval_if_c<
        mpl::apply<Predicate, N>::type::value,
        mpl::next<N>,
        mpl::identity<N>
      >
{};

static_assert(
    next_if2<mpl::int_<0>, mpl::equal_to<_1, mpl::int_<0>>>::type::value == 1,
    "0 == 0 -> next(0)"
);

static_assert(
    next_if2<mpl::int_<1>, mpl::equal_to<_1, mpl::int_<0>>>::type::value == 1,
    "1 != 0 -> identity(1)"
);

// ----------------------------------------------------------------------------

/***********
 * Version 1
 ***********/
template <class N1, class N2>
struct formula
    : mpl::if_<
        mpl::not_equal_to<N1, N2>,
        typename mpl::if_<
            mpl::greater<N1, N2>,
            typename mpl::minus<N1, N2>::type,
            N1
        >::type,
        typename mpl::plus<
            N1,
            typename mpl::multiplies<N1, mpl::int_<2>>::type
        >::type
      >::type
{};

static_assert(
    formula<mpl::int_<2>, mpl::int_<2>>::value == 2 + 2 * 2,
    "N1 + N1 * 2"
);

static_assert(
    formula<mpl::int_<2>, mpl::int_<3>>::value == 2,
    "N1 < N2 -> N1"
);

static_assert(
    formula<mpl::int_<3>, mpl::int_<2>>::value == 1,
    "N1 > N2 -> N1 - N2"
);

/***********
 * Version 2
 ***********/
template <class N1, class N2>
struct formula2
    : mpl::eval_if<
        mpl::not_equal_to<N1, N2>,
        mpl::eval_if<
            mpl::greater<N1, N2>,
            mpl::minus<N1, N2>,
            mpl::identity<N1>
        >,
        mpl::plus<
            N1,
            typename mpl::multiplies<N1, mpl::int_<2>>::type
        >
      >::type
{};

static_assert(
    formula2<mpl::int_<2>, mpl::int_<2>>::value == 2 + 2 * 2,
    "N1 + N1 * 2"
);

static_assert(
    formula2<mpl::int_<2>, mpl::int_<3>>::value == 2,
    "N1 < N2 -> N1"
);

static_assert(
    formula2<mpl::int_<3>, mpl::int_<2>>::value == 1,
    "N1 > N2 -> N1 - N2"
);

int main()
{
    return 0;
}
