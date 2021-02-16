#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/iterator_range.hpp>
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

int main()
{
}
