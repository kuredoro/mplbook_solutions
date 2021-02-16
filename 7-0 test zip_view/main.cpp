#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

/// For the sake of simplicity zip_iterator will model ForwardIterator
template <class IteratorSeq>
struct zip_iterator
{
    using category = mpl::forward_iterator_tag;

    /// The default impl of mpl::deref will call ::type on the iterator.
    using type =
        typename mpl::transform<
            IteratorSeq,
            mpl::deref<_>
        >::type;

    /// Convenience alias to the underlying iterators;
    using base = IteratorSeq;
};


template <class IteratorSeq>
struct mpl::next<::zip_iterator<IteratorSeq>>
{
    using type =
        ::zip_iterator<
            typename mpl::transform<
                IteratorSeq,
                mpl::next<_>
            >::type
        >;
};


template <class Sequences>
struct zip_view
    : mpl::iterator_range<
        zip_iterator<
            typename mpl::transform<
                Sequences,
                mpl::begin<_>
            >::type
        >,
        zip_iterator<
            typename mpl::transform<
                Sequences,
                mpl::end<_>
            >::type
        >
      >
{};


using seq1 = mpl::vector<char, int, long>;
using seq2 = mpl::vector<long, short, char>;
using merged = mpl::vector<mpl::vector<char, long>, mpl::vector<int, short>, mpl::vector<long, char>>;

/*
/// Has some wierd template instantiation error deep within the library :(
static_assert(
    mpl::equal<
        zip_view<mpl::vector<seq1, seq2>>,
        merged,
        mpl::equal<_, _>
    >()
);
*/

static_assert(
    mpl::equal<
        mpl::deref<
            mpl::begin<
                zip_view<mpl::vector<seq1, seq2>>
            >::type
        >::type,
        mpl::vector<char, long>
    >(),
    ""
);

static_assert(
    mpl::equal<
        mpl::deref<
            mpl::advance<
                mpl::begin<
                    zip_view<mpl::vector<seq1, seq2>>
                >::type,
                mpl::int_<1>
            >::type
        >::type,
        mpl::vector<int, short>
    >(),
    ""
);

static_assert(
    mpl::equal<
        mpl::deref<
            mpl::advance<
                mpl::begin<
                    zip_view<mpl::vector<seq1, seq2>>
                >::type,
                mpl::int_<2>
            >::type
        >::type,
        mpl::vector<long, char>
    >(),
    ""
);

int main()
{
}
