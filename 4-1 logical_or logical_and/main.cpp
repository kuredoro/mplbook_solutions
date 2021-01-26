#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace mpl = boost::mpl;

/**************
 * Logical Or *
 **************/
template <class A, class B, bool vA = A::value>
struct logical_or;

template <class A, class B>
struct logical_or<A, B, true>
    : mpl::bool_<true>
{};

template <class A, class B>
struct logical_or<A, B, false>
    : mpl::bool_<B::value>
{};

static_assert(
    logical_or<mpl::bool_<true>, int>::value == true
);

static_assert(
    logical_or<mpl::bool_<false>, mpl::bool_<false>>::value == false
);

static_assert(
    logical_or<mpl::bool_<false>, mpl::bool_<true>>::value == true
);


/******************************
 * Logical And                *
 * Alternative implementation *
 ******************************/
template <class A, class B>
struct logical_and
    : mpl::if_<
        A,
        B,
        mpl::false_
      >::type
{};

static_assert(
    logical_and<mpl::bool_<false>, int>::value == false
);

static_assert(
    logical_and<mpl::bool_<true>, mpl::bool_<false>>::value == false
);

static_assert(
    logical_and<mpl::bool_<true>, mpl::bool_<true>>::value == true
);

int main()
{
}
