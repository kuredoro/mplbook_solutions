#pragma once

#include <boost/mpl/advance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/list_c.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/insert.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

#include "permutation_iterator.hpp"

/// This works on lists, unlike the prior<end<Sequence>>
template <class Sequence>
using last_element_t =
    typename mpl::advance<
        typename mpl::begin<Sequence>::type,
        typename mpl::minus<
            typename mpl::size<Sequence>::type,
            mpl::int_<1>
        >::type
    >::type;

/// The make_permutation_iterator metafunctions here differ from those in the
/// exercises 4 and 5 and address the issue of the end iterator. That is,
/// iterator adaptors don't care about how past-the-end iterators look like,
/// that's the job of a sequence to define it. So here it goes.
///
/// The problem: the permutation_iterator that points to the last element
/// is not dereferencable. That's because for mpl::next to work, it needs to
/// know what the next index gonna be, but the permutation iterator also points
/// to the last element, there's simply *no* next index to go to.
///
/// Solution presented here: make it so that there *is* actually the next index.
/// Moreover, let it not be just some random index, but an index that will make
/// SeqIt point past-the-end of its sequence.

template <class Sequence, class Permutation>
struct make_permutation_iterator
{
    using seqIterator =
        typename mpl::advance<
            typename mpl::begin<Sequence>::type,
            typename mpl::front<Permutation>::type
        >::type;

    /// We're using insert instead of push_back, because lists don't support it
    using augmentedPermutation =
        typename mpl::insert<
            Permutation,
            typename mpl::end<Permutation>::type,
            typename mpl::size<Sequence>::type
        >::type;

    using type =
        permutation_iterator<
            seqIterator,
            typename mpl::begin<augmentedPermutation>::type
        >;
};

template <class Sequence, class Permutation>
struct make_permutation_iterator_end
{
    using seqIterator = typename mpl::end<Sequence>::type;

    // Code duplication :(
    // At least it's duplicated only once...
    using augmentedPermutation =
        typename mpl::insert<
            Permutation,
            typename mpl::end<Permutation>::type,
            typename mpl::size<Sequence>::type
        >::type;

    using type =
        permutation_iterator<
            seqIterator,
            last_element_t<augmentedPermutation>
        >;
};

template <class Sequence, class Permutation>
struct permutation_view
    : mpl::iterator_range<
        typename make_permutation_iterator<Sequence, Permutation>::type,
        typename make_permutation_iterator_end<Sequence, Permutation>::type
      >
{};

static_assert(
    mpl::equal<
        permutation_view<
            mpl::vector_c<int, 0, 1, 2>,
            mpl::vector_c<int, 2, 1, 0>
        >,
        mpl::vector_c<int, 2, 1, 0>,
        mpl::equal_to<_, _>
    >(),
    ""
);

static_assert(
    mpl::equal<
        permutation_view<
            mpl::vector_c<int, 11, 22, 33, 44>,
            mpl::list_c<int, 2, 1, 3, 0, 2>
        >,
        mpl::vector_c<int, 33, 22, 44, 11, 33>,
        mpl::equal_to<_, _>
    >(),
    ""
);

