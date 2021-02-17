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
namespace mpl = boost::mpl;

/// Invariant: `It` always points to the advance<BeginIt, Permutation[PermutIndex]>
/// That means that if Permutation[0] is not 0, `It` must be computed
/// manually before instantiating this template.
/// See: make_permutation_iterator
template <
      class It
    , class Permutation
    , class PermutIndex = mpl::int_<0>
    >
struct permutation_iterator
{
    using category = mpl::forward_iterator_tag;

    using type = typename mpl::deref<It>::type;
};

template <class It, class Permutation, class PermutIndex>
struct mpl::next<permutation_iterator<It, Permutation, PermutIndex>>
{
    using currentOffset = typename mpl::at<Permutation, PermutIndex>::type;

    using nextPermutIndex =
        typename mpl::plus<
            PermutIndex,
            mpl::int_<1>
        >::type;

    using nextOffset =
        typename mpl::at<
            Permutation,
            nextPermutIndex
        >::type;

    using distance = typename mpl::minus<nextOffset, currentOffset>::type;

    using nextIt = typename mpl::advance<It, distance>::type;

    using type = permutation_iterator<nextIt, Permutation, nextPermutIndex>;
};

template <class Sequence, class Permutation>
struct make_permutation_iterator
{
    using it =
        typename mpl::advance<
            typename mpl::begin<Sequence>::type,
            typename mpl::front<Permutation>::type
        >::type;

    using type = permutation_iterator<it, Permutation>;
};

using seq = mpl::vector_c<int, 0, 1, 2>;
using permut = mpl::vector_c<int, 2, 1, 0>;

using pit = make_permutation_iterator<seq, permut>::type;

static_assert(
    mpl::equal_to<
        pit::type,
        mpl::int_<2>
    >(),
    ""
);

static_assert(
    mpl::equal_to<
        mpl::advance<
            pit,
            mpl::int_<1>
        >::type::type,
        mpl::int_<1>
    >(),
    ""
);

static_assert(
    mpl::equal_to<
        mpl::advance<
            pit,
            mpl::int_<2>
        >::type::type,
        mpl::int_<0>
    >(),
    ""
);

int main()
{
}
