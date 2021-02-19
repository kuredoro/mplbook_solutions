#include <type_traits>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/iterator_category.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/negate.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal.hpp>
namespace mpl = boost::mpl;

/// For testing
#include "permutation_view.hpp"

/// Reverse iterator points to the element before the one It points to.
/// Refer to: https://en.cppreference.com/w/cpp/iterator/reverse_iterator
template <class It>
struct reverse_iterator
{
    using category = typename mpl::iterator_category<It>::type;

    static_assert(
        !std::is_same<category, mpl::forward_iterator_tag>(),
        "The underlying iterator must be at least the model of BidirectionalIterator"
    );

    using type =
        typename mpl::deref<
            typename mpl::prior<It>::type
        >::type;
};

// -----------------------------------------------------------------------------

template <class It>
struct mpl::next<reverse_iterator<It>>
{
    using type = reverse_iterator<typename mpl::prior<It>::type>;
};

template <class It>
struct mpl::prior<reverse_iterator<It>>
{
    using type = reverse_iterator<typename mpl::next<It>::type>;
};

template <class It, class N>
struct mpl::advance<reverse_iterator<It>, N>
{
    using type = 
        reverse_iterator<
            typename mpl::advance<
                It,
                typename mpl::negate<N>::type
            >::type
        >;
};

template <class It1, class It2>
struct mpl::distance<reverse_iterator<It1>, reverse_iterator<It2>>
{
    using type = typename mpl::distance<It2, It1>::type;
};

// -----------------------------------------------------------------------------

template <class Sequence>
struct reverse_view
    : mpl::iterator_range<
        reverse_iterator<typename mpl::end<Sequence>::type>,
        reverse_iterator<typename mpl::begin<Sequence>::type>
      >
{};

// -----------------------------------------------------------------------------

using seq =  mpl::vector_c<int, 11, 22, 33, 44>;

static_assert(
    mpl::equal<
        reverse_view<seq>,
        mpl::vector_c<int, 44, 33, 22, 11>
    >(),
    ""
);

static_assert(
    mpl::equal<
        reverse_view<
            permutation_view<
                seq,
                mpl::vector_c<int, 2, 1, 3, 0, 2>
            >
        >,
        mpl::vector_c<int, 33, 11, 44, 22, 33>
    >(),
    ""
);

int main()
{
}
