#include "tiny.hpp"
#include <boost/mpl/insert.hpp>
#include <boost/mpl/equal.hpp>

template <class Seq, class T, int Index>
struct tiny_insert;

template <class Tiny, class T>
struct tiny_insert<Tiny, T, 0>
    : tiny<T, typename Tiny::t0, typename Tiny::t1>
{
    static_assert(mpl::size<Tiny>::type::value != 3, "trying to insert into a full vector");
};

template <class Tiny, class T>
struct tiny_insert<Tiny, T, 1>
    : tiny<typename Tiny::t0, T, typename Tiny::t1>
{
    static_assert(mpl::size<Tiny>::type::value != 3, "trying to insert into a full vector");
};

template <class Tiny, class T>
struct tiny_insert<Tiny, T, 2>
    : tiny<typename Tiny::t0, typename Tiny::t1, T>
{
    static_assert(mpl::size<Tiny>::type::value != 3, "trying to insert into a full vector");
};

template <>
struct mpl::insert_impl<tiny_tag>
{
    template <class Tiny, class Iter, class T>
    struct apply;

    template <class Tiny, class Pos, class T>
    struct apply<Tiny, tiny_iterator<Tiny, Pos>, T>
        : tiny_insert<Tiny, T, Pos::value>
    {};
};

static_assert(
    mpl::equal<
        mpl::insert<tiny<>, mpl::begin<tiny<>>::type, int>::type,
        tiny<int>
    >()
);

static_assert(
    mpl::equal<
        mpl::insert<tiny<char>, mpl::begin<tiny<char>>::type, int>::type,
        tiny<int, char>
    >()
);

static_assert(
    mpl::equal<
        mpl::insert<
            tiny<char, long>, 
            mpl::next<
                mpl::begin<tiny<char, long>>::type
            >::type, 
            int
        >::type,
        tiny<char, int, long>
    >()
);

static_assert(
    mpl::equal<
        mpl::insert<
            tiny<char, int>, 
            mpl::end<tiny<char, int>>::type,
            long
        >::type,
        tiny<char, int, long>
    >()
);

/*
 * mpl::push_back appends T at the end of the sequence
 * Compilation error if tiny is full.
 */
template <>
struct mpl::push_back_impl<tiny_tag>
{
    template <class Tiny, class T>
    struct apply
        : mpl::insert<Tiny, typename mpl::end<Tiny>::type, T>::type
    {};
};

static_assert(
    std::is_same<
        mpl::push_back<tiny<int>, long>::type,
        tiny<int, long>
    >::value
);

int main()
{
}
