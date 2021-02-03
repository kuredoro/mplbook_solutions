#include "tiny.hpp"
#include <boost/mpl/erase.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/pop_front.hpp>

/*
 * tiny_erase is a helper metafunction that returns a sequence with the elment
 * erased at position Index.
 */
template <class Tiny, int Index>
struct tiny_erase;

template <class Tiny>
struct tiny_erase<Tiny, 0>
    : tiny<typename Tiny::t1, typename Tiny::t2, none>
{};

template <class Tiny>
struct tiny_erase<Tiny, 1>
    : tiny<typename Tiny::t0, typename Tiny::t2, none>
{};

template <class Tiny>
struct tiny_erase<Tiny, 2>
    : tiny<typename Tiny::t0, typename Tiny::t1, none>
{};

template <class Tiny, class Pos>
using tiny_erase_t = typename tiny_erase<Tiny, Pos::value>::type;

/*
 * mpl::erase will do a recursion distance<First, Last> in depth, calling the
 * helper tiny_erase for each element to be erased.
 */
template <>
struct mpl::erase_impl<tiny_tag>
{
    template <class Tiny, class First, class Last>
    struct apply;

    // If no Last provided, conform to the documentation and expected
    // semantics: 
    // https://www.boost.org/doc/libs/1_75_0/libs/mpl/doc/refmanual/erase.html
    template <class Tiny, class First>
    struct apply<Tiny, First, mpl::na>
        : apply<Tiny, First, typename mpl::next<First>::type>
    {};

    // While we have elements to delete, erase the elemented pointed by First,
    // and move Last one element back (since the next element to erase will
    // be shifted towards First after erasure)
    template <class Tiny, class FirstPos, class LastPos>
    struct apply<Tiny, tiny_iterator<Tiny, FirstPos>, tiny_iterator<Tiny, LastPos>>
        : apply<
            tiny_erase_t<Tiny, FirstPos>,
            tiny_iterator<tiny_erase_t<Tiny, FirstPos>, FirstPos>,
            typename mpl::prior<tiny_iterator<tiny_erase_t<Tiny, FirstPos>, LastPos>>::type
          >
          {};

    // When First and Last are the same, we terminate and return the sequence
    template <class Tiny, class Pos>
    struct apply<Tiny, tiny_iterator<Tiny, Pos>, tiny_iterator<Tiny, Pos>>
        : Tiny
    {};
};

static_assert(
    mpl::equal<
        mpl::erase<tiny<char, int>, mpl::begin<tiny<char, int>>::type>::type,
        tiny<int>
    >(),
    "erase first element"
);

static_assert(
    mpl::equal<
        mpl::erase<
            tiny<char, int, long>, 
            mpl::advance<
                mpl::begin<tiny<char, int, long>>::type,
                mpl::int_<1>
            >::type
        >::type,
        tiny<char, long>
    >(),
    "erase middle element"
);

static_assert(
    mpl::equal<
        mpl::erase<
            tiny<char, int, long>, 
            mpl::advance<
                mpl::begin<tiny<char, int, long>>::type,
                mpl::int_<2>
            >::type
        >::type,
        tiny<char, int>
    >(),
    "erase last element"
);

static_assert(
    mpl::equal<
        mpl::erase<
            tiny<char, int, long>, 
            mpl::begin<tiny<char, int, long>>::type,
            mpl::advance<
                mpl::begin<tiny<char, int, long>>::type,
                mpl::int_<2>
            >::type
        >::type,
        tiny<long>
    >(),
    "erase first two"
);

static_assert(
    mpl::equal<
        mpl::erase<
            tiny<char, int, long>, 
            mpl::advance<
                mpl::begin<tiny<char, int, long>>::type,
                mpl::int_<1>
            >::type,
            mpl::end<tiny<char, int, long>>::type
        >::type,
        tiny<char>
    >(),
    "erase last two"
);

static_assert(
    mpl::equal<
        mpl::erase<
            tiny<char, int, long>, 
            mpl::begin<tiny<char, int, long>>::type,
            mpl::end<tiny<char, int, long>>::type
        >::type,
        tiny<>
    >(),
    "erase all"
);


/*
 * Auxillary metafunctions that are implemented via mpl::erase
 */
template <>
struct mpl::pop_front_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
        : mpl::erase<Tiny, typename mpl::begin<Tiny>::type>::type
    {};
};

static_assert(
    std::is_same<
        mpl::pop_front<tiny<char, int, long>>::type,
        tiny<int, long>
    >::value
);

template <>
struct mpl::pop_back_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
        : mpl::erase<
            Tiny, 
            typename mpl::prior<
                typename mpl::end<Tiny>::type
            >::type
          >::type
    {};
};

static_assert(
    std::is_same<
        mpl::pop_back<tiny<char, int>>::type,
        tiny<char>
    >::value
);

int main()
{
}
