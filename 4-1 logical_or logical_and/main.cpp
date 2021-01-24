#include <boost/mpl/bool.hpp>

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

/***************
 * Logical And *
 ***************/
template <class A, class B, bool vA = A::value>
struct logical_and;

template <class A, class B>
struct logical_and<A, B, false>
    : mpl::bool_<false>
{};

template <class A, class B>
struct logical_and<A, B, true>
    : mpl::bool_<B::value>
{};

int main()
{
    // Or
    static_assert(
        logical_or<mpl::bool_<true>, int>::value == true
    );

    static_assert(
        logical_or<mpl::bool_<false>, mpl::bool_<false>>::value == false
    );

    static_assert(
        logical_or<mpl::bool_<false>, mpl::bool_<true>>::value == true
    );


    // And
    static_assert(
        logical_and<mpl::bool_<false>, int>::value == false
    );

    static_assert(
        logical_and<mpl::bool_<true>, mpl::bool_<false>>::value == false
    );

    static_assert(
        logical_and<mpl::bool_<true>, mpl::bool_<true>>::value == true
    );
}
