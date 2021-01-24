#include <boost/mpl/bool.hpp>
#include <boost/mpl/bool_fwd.hpp>

namespace mpl = boost::mpl;

/**************
 * Logical Or *
 **************/
template <class A, bool v, class ...Rest>
struct logical_or_impl; // THE FUNDAMENTAL THEOREM OF SOFTWARE ENGINEERING!!!

template <class A, class ...Rest>
struct logical_or 
    : logical_or_impl<A, A::value, Rest...>
{};


template <class A, class ...Rest>
struct logical_or_impl<A, true, Rest...>
    : mpl::bool_<true>
{};

template <class A, class B, class ...Rest>
struct logical_or_impl<A, false, B, Rest...>
    : logical_or<B, Rest...>
{};

template <class A>
struct logical_or_impl<A, false>
    : mpl::bool_<false>
{};


/***************
 * Logical And *
 ***************/
template <class A, bool v, class ...Rest>
struct logical_and_impl;

template <class A, class ...Rest>
struct logical_and
    : logical_and_impl<A, A::value, Rest...>
{};

template <class A, class ...Rest>
struct logical_and_impl<A, false, Rest...>
    : mpl::bool_<false>
{};

template <class A, class B, class ...Rest>
struct logical_and_impl<A, true, B, Rest...>
    : logical_and<B, Rest...>
{};

template <class A>
struct logical_and_impl<A, true>
    : mpl::bool_<true>
{};


int main()
{
    // Or
    static_assert(
        logical_or<mpl::true_, int>::value == true
    );

    static_assert(
        logical_or<mpl::false_, mpl::false_, mpl::false_>::value == false
    );

    static_assert(
        logical_or<mpl::false_, mpl::true_>::value == true
    );

    static_assert(
        logical_or<mpl::false_, mpl::true_, int>::value == true
    );


    // And
    static_assert(
        logical_and<mpl::false_, int>::value == false
    );

    static_assert(
        logical_and<mpl::true_, mpl::true_, mpl::true_>::value == true
    );

    static_assert(
        logical_and<mpl::true_, mpl::false_>::value == false
    );

    static_assert(
        logical_and<mpl::true_, mpl::false_, int>::value == false
    );
}
