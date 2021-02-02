#pragma once

#include <boost/mpl/aux_/empty_impl.hpp>
#include <boost/mpl/aux_/push_front_impl.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/minus.hpp>
#include <type_traits>

namespace mpl = boost::mpl;

struct none {}; // tag type to denote no element

struct tiny_tag {};

/*
 * tiny is a RandomAccessSequence
 */
template <class T0 = none, class T1 = none, class T2 = none>
struct tiny
{
    typedef tiny_tag tag;
    typedef tiny type;  // metaprogramming value
    typedef T0 t0;
    typedef T1 t1;
    typedef T2 t2;
};

/*
 * tiny_iterator is a RandomAccessIterator
 */
template <class Tiny, class Pos>
struct tiny_iterator
{
    typedef mpl::random_access_iterator_tag category;
    typedef tiny_iterator<Tiny, Pos> type;
};

using tiny123 = tiny<mpl::int_<0>, mpl::int_<1>, mpl::int_<2>>;

/* 
 * tiny_size returns the number of elements in tiny<T0, T1, T2>
 */
template <class T0, class T1, class T2>
struct tiny_size
    : mpl::int_<3>
{};

template <class T0, class T1>
struct tiny_size<T0, T1, none>
    : mpl::int_<2>
{};

template <class T0>
struct tiny_size<T0, none, none>
    : mpl::int_<1>
{};

template <>
struct tiny_size<none, none, none>
    : mpl::int_<0>
{};

/*
 * mpl::size is implemented via tiny_size
 */
template <>
struct mpl::size_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
        : tiny_size<
            typename Tiny::t0,
            typename Tiny::t1,
            typename Tiny::t2
          >
    {};
};

static_assert(
    mpl::size<tiny<>>::value == 0
);

static_assert(
    mpl::size<tiny<int>>::value == 1
);

static_assert(
    mpl::size<tiny<int, long>>::value == 2
);

static_assert(
    mpl::size<tiny<int, long, short>>::value == 3
);

/*
 * mpl::empty is implemented via size<T> == 0
 */
template <>
struct mpl::empty_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
        : mpl::equal_to<typename mpl::size<Tiny>::type, mpl::int_<0>>
    {};
};

static_assert(
    mpl::empty<tiny<>>::type::value == true
);

static_assert(
    mpl::empty<tiny<int>>::type::value == false
);

/*
 * mpl::front is just the first element
 */
template <>
struct mpl::front_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
    {
        typedef typename Tiny::t0 type;
    };
};

static_assert(
    std::is_same<mpl::front<tiny<int, long>>::type, int>::value
);

/*
 * mpl::begin returns an iterator to the first element in the sequence
 */
template <>
struct mpl::begin_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
        : tiny_iterator<Tiny, mpl::int_<0>>
    {};
};

/*
 * mpl::end returns the past-the-end iterator of the sequence
 */
template <>
struct mpl::end_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
        : tiny_iterator<Tiny, typename mpl::size<Tiny>::type>
    {};
};

static_assert(
    std::is_same<
        mpl::end<tiny123>::type, 
        tiny_iterator<tiny123, mpl::int_<3>>
    >::value
);

static_assert(
    std::is_same<
        mpl::begin<tiny<>>::type,
        mpl::end<tiny<>>::type
    >::value
);


// ---------------------------------------------------
// tiny is now a model of ForwardSequence
// since it implements: size, empty, front, begin, end
// ---------------------------------------------------


/*
 * tiny_at returns the type at the given position
 */
template <class Tiny, int Index>
struct tiny_at;

template <class Tiny>
struct tiny_at<Tiny, 0>
{
    typedef typename Tiny::t0 type;
};

template <class Tiny>
struct tiny_at<Tiny, 1>
{
    typedef typename Tiny::t1 type;
};

template <class Tiny>
struct tiny_at<Tiny, 2>
{
    typedef typename Tiny::t2 type;
};

// Partial specialization is needed for each new type,
// Tag dispatch is needed for each new tag
// And tags can be shared among many different types.
// It's something like std::is_same vs. mpl::equal
//
// template <class T0, class T1, class T2, class Pos>
// struct mpl::at<tiny<T0, T1, T2>, Pos>
//     : tiny_at<tiny<T0, T1, T2>, Pos::value>
// {};

/*
 * mpl::at returns the value at arbitrary position
 */
template <>
struct mpl::at_impl<tiny_tag>
{
    template <class Tiny, class Pos>
    struct apply
        : tiny_at<Tiny, Pos::value>
    {};
};

static_assert(
    mpl::at<tiny123, mpl::int_<0>>::type::value == 0
);

static_assert(
    mpl::at<tiny123, mpl::int_<1>>::type::value == 1
);

static_assert(
    mpl::at<tiny123, mpl::int_<2>>::type::value == 2
);

/*
 * mpl::back returns the value of the last item in the sequence
 * It is needed to be implemented for the sequence to become eventually
 * a RandomAccessSequence, even though we already have `at`
 */
template <>
struct mpl::back_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
    {
        typedef typename mpl::at<
            Tiny, 
            typename mpl::prior<
                typename mpl::size<Tiny>::type
            >::type
        >::type type;
    };
};

static_assert(
    std::is_same<mpl::back<tiny<char, int, long>>::type, long>::value
);


// ----------------------------------------------------------------
// tiny is now a model of BidirectionalSequence
// Since it implements: size, empty, begin, end, front and **back**
// ----------------------------------------------------------------


// --------------------------------------------------------------------
// tiny is now a model of RandomAccessSequence
// Since it implements: size, empty, begin, end, front, back and **at**
// --------------------------------------------------------------------


// NOTE:
// Partial specialization on iterator type is ok, since no two
// iterators with different types can be the same.

/*
 * mpl::deref returns the value referenced by the iterator
 */
template <class Tiny, class Pos>
struct mpl::deref<tiny_iterator<Tiny, Pos>>
    : mpl::at<Tiny, Pos>
{};

static_assert(
    mpl::deref<mpl::begin<tiny123>::type>::type::value == 0
);

/*
 * mpl::next returns the next iterator in the sequence. It can be
 * called only on a valid iterator.
 */
template <class Tiny, class Pos>
struct mpl::next<tiny_iterator<Tiny, Pos>>
{
    typedef tiny_iterator<
        Tiny,
        typename mpl::next<Pos>::type
    > type;
};

static_assert(
    std::is_same<
        mpl::deref<
            mpl::next<
                mpl::begin<
                    tiny<char, int, long>
                >::type
            >::type
        >::type,
        int
    >::value
);


// -----------------------------------------------
// tiny_iterator is now a model of ForwardIterator
// Since it implements: deref and next
// -----------------------------------------------


/*
 * mpl::prior allows to iterate sequence backwards
 */
template <class Tiny, class Pos>
struct mpl::prior<tiny_iterator<Tiny, Pos>>
{
    typedef tiny_iterator<
        Tiny,
        typename mpl::prior<Pos>::type
    > type;
};

static_assert(
    std::is_same<
        mpl::deref<
            mpl::prior<
                mpl::end<
                    tiny<char, int, long>
                >::type
            >::type
        >::type,
        long
    >::value
);


// -----------------------------------------------------
// tiny_iterator is now a model of BidirectionalIterator
// Since it implements: deref, next and **prior**
// -----------------------------------------------------


/*
 * mpl::advance allows to iterate in any direction in however
 * long strides in constant time
 */
template <class Tiny, class Pos, class N>
struct mpl::advance<tiny_iterator<Tiny, Pos>, N>
    : tiny_iterator<Tiny, typename mpl::plus<Pos, N>::type>    
{};

static_assert(
    std::is_same<
        mpl::deref<
            mpl::advance<
                mpl::begin<
                    tiny<char, int, long>
                >::type, mpl::int_<2>
            >::type
        >::type,
        long
    >::value
);

/*
 * mpl::distance returns the number of steps required to get from one
 * iterator to another. Works in constant time.
 */
template <class Tiny, class Pos1, class Pos2>
struct mpl::distance<tiny_iterator<Tiny, Pos1>,
                     tiny_iterator<Tiny, Pos2>
                     >
    : mpl::minus<Pos2, Pos1>
{};

static_assert(
    mpl::distance<mpl::begin<tiny123>::type, mpl::end<tiny123>::type>::type::value == 3
);


// ---------------------------------------------------------------------
// tiny_iterator is now a model of RandomAccessIterator
// Since it implements: deref, next, prior, **advance** and **distance**
// ---------------------------------------------------------------------


// For the sequence to be ExtensibleSequence, it actually has to 
// implement insert and erase functionality.
// What was implemented in the chapter is everything but the 
// ExtensibleSequence.
// That is, the functionality needed for it to become 
// FrontExtensible and BackExtensible Sequence, but failing to do so
// by missing implementation for ExtensibleSequence itself.


/*
 * mpl::clear returns an empty sequence 
 * (dunno why it needs a sequence though)
 */
template <>
struct mpl::clear_impl<tiny_tag>
{
    template <class Tiny>
    struct apply
        : tiny<>
    {};
};

static_assert(
    std::is_same<
        mpl::clear<tiny<char, int, long>>::type,
        tiny<>
    >::value
);

/*
 * mpl::push_front pushes the values one step right and puts the T
 * into the first position. 
 * <!> Does not cause a compilation error if
 * tiny is full.
 */
template <>
struct mpl::push_front_impl<tiny_tag>
{
    template <class Tiny, class T>
    struct apply
        : tiny<T, typename Tiny::t0, typename Tiny::t1>
    {
        static_assert(std::is_same<typename Tiny::t2, none>::value);
    };
};

static_assert(
    std::is_same<
        mpl::push_front<tiny<int>, long>::type,
        tiny<long, int>
    >::value
);

