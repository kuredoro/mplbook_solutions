#pragma once

#include <boost/mpl/minus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/iterator_category.hpp>
#include <boost/mpl/advance.hpp>
namespace mpl = boost::mpl;

/// Invariant: BeginIt + *PermutIt = SeqIt
/// Also, it's a traversal iterator adaptor, and it needs only the iterators,
/// nice improvement over version 1.
template <class SeqIt, class PermutIt>
struct permutation_iterator
{
    using category = typename mpl::iterator_category<PermutIt>::type;

    using type = typename mpl::deref<SeqIt>::type;
};

/// The task requires permutation_iterator to be of the same category as
/// PermutIt. But in this implementation the resulting category is actually
/// common_iterator_tag<SeqIt, PermutIt> due to the use of mpl::advance on
/// SeqIt to compute `nextSeqIt` down there.
///
/// We could take another approach, by abandoning SeqIt and storing Sequence
/// itself and an Index within it. Then we could satisfy the requirement of the
/// task, since we would mpl::advance only PermutIt. When we actually would
/// need to get the value under the Sequence[Index], we could compute the
/// actual iterator and dereference it in the mpl::deref<permutation_iterator<...>>
///
/// But look, unlike the first solution, the complexity of deref will be O(1)
/// only when accessing Sequence[Index] is O(1), but this is true only when
/// the underlying sequence is RandomAccessSequence! Which means if it's not,
/// permutation_iterator will fail to be an Iterator at all!
template <class SeqIt, class PermutIt, class N>
struct mpl::advance<permutation_iterator<SeqIt, PermutIt>, N>
{
    using nextPermutIt = typename mpl::advance<PermutIt, N>::type;

    using currentIndex = typename mpl::deref<PermutIt>::type;
    using nextIndex = typename mpl::deref<nextPermutIt>::type;
    using distance = typename mpl::minus<nextIndex, currentIndex>::type;

    using nextSeqIt = typename mpl::advance<SeqIt, distance>::type;

    using type = permutation_iterator<nextSeqIt, nextPermutIt>;
};

template <class SeqIt, class PermutIt>
struct mpl::next<permutation_iterator<SeqIt, PermutIt>>
    : mpl::advance<
        permutation_iterator<SeqIt, PermutIt>,
        mpl::int_<1>
      >
{};

template <class SeqIt, class PermutIt>
struct mpl::prior<permutation_iterator<SeqIt, PermutIt>>
    : mpl::advance<
        permutation_iterator<SeqIt, PermutIt>,
        mpl::int_<-1>
      >
{};

