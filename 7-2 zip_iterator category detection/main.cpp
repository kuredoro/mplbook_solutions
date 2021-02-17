#include <boost/mpl/distance.hpp>
#include <boost/mpl/integral_c_fwd.hpp>
#include <type_traits>

#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/iterator_category.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/front.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

using iter_tag_order =
    mpl::map<
        mpl::pair<mpl::forward_iterator_tag,       mpl::int_<1>>,
        mpl::pair<mpl::bidirectional_iterator_tag, mpl::int_<2>>,
        mpl::pair<mpl::random_access_iterator_tag, mpl::int_<3>>,

        mpl::pair<mpl::int_<1>, mpl::forward_iterator_tag>,
        mpl::pair<mpl::int_<2>, mpl::bidirectional_iterator_tag>,
        mpl::pair<mpl::int_<3>, mpl::random_access_iterator_tag>
    >;

/// min_element_value is a wrapper around mpl::min_element that dereferences
/// the resulting iterator.
/// Note: since mpl::min_element returns an iterator within the sequence,
/// min_element_value will be a member of that saqunece.
template <class Seq>
using min_element_value =
    typename mpl::deref<
        mpl::min_element<Seq>
    >::type;

/// common_iterator_tag accepcts a sequence of iterator_tags and returns
/// the least refined iterator tag.
template <class TagSeq>
using common_iterator_tag =
    mpl::at<
        iter_tag_order,
        // the key will be of type mpl:int_<...>.
        // Refer to the note for the min_element_value
        typename min_element_value<
            mpl::transform_view<
                TagSeq,
                mpl::at<iter_tag_order, _>
            >
        >::type
    >;

static_assert(
    std::is_same<
        common_iterator_tag<
            mpl::vector<
                mpl::random_access_iterator_tag, 
                mpl::bidirectional_iterator_tag, 
                mpl::forward_iterator_tag
            >
        >::type,
        mpl::forward_iterator_tag
    >(),
    ""
);


template <class IteratorSeq>
struct zip_iterator
{
    using category =
        typename common_iterator_tag<
            mpl::transform_view<
                IteratorSeq,
                mpl::iterator_category<_>
            >
        >::type;

    /// The default impl of mpl::deref will call ::type on the iterator.
    using type =
        mpl::transform_view<
            IteratorSeq,
            mpl::deref<_>
        >;

    /// Convenience alias to the underlying iterators;
    using base = IteratorSeq;
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

// -----------------------------------------------------------------------------

template <class IteratorSeq>
struct mpl::next<::zip_iterator<IteratorSeq>>
{
    using type =
        ::zip_iterator<
            mpl::transform_view<
                IteratorSeq,
                mpl::next<_>
            >
        >;
};

template <class IteratorSeq>
struct mpl::prior<::zip_iterator<IteratorSeq>>
{
    static_assert(
        !std::is_same<
            mpl::forward_iterator_tag,
            typename ::zip_iterator<IteratorSeq>::category
        >(),
        "trying to call prior on a ForwardIterator"
    );

    using type =
        ::zip_iterator<
            mpl::transform_view<
                IteratorSeq,
                mpl::prior<_>
            >
        >;
};

template <class IteratorSeq, class Offset>
struct mpl::advance<::zip_iterator<IteratorSeq>, Offset>
{
    using it_category = typename ::zip_iterator<IteratorSeq>::category;

    static_assert(
        !std::is_same<mpl::forward_iterator_tag, it_category>(),
        "trying to call advance on a ForwardIterator"
    );

    static_assert(
        !std::is_same<mpl::bidirectional_iterator_tag, it_category>(),
        "trying to call advance on a BidirectionalIterator"
    );

    using type =
        ::zip_iterator<
            mpl::transform_view<
                IteratorSeq,
                mpl::advance<_, Offset>
            >
        >;
};

template <class IterSeq1, class IterSeq2>
struct mpl::distance<::zip_iterator<IterSeq1>, ::zip_iterator<IterSeq2>>
{
    static_assert(
        std::is_same<
            mpl::random_access_iterator_tag, 
            typename ::zip_iterator<IterSeq1>::category
        >(),
        "first iterator to distance is not a model of RandomAccessIterator"
    );

    static_assert(
        std::is_same<
            mpl::random_access_iterator_tag, 
            typename ::zip_iterator<IterSeq2>::category
        >(),
        "second iterator to distance is not a model of RandomAccessIterator"
    );


    using distance_vector =
        typename mpl::transform<
            IterSeq1,
            IterSeq2,
            mpl::distance<_, _>
        >::type;

    /// Note that we cannot use transform_view instead of mpl::transform here,
    /// something like
    ///
    /// using distance_vector =
    ///     mpl::transform_view<
    ///         ::zip_view<mpl::vector<IterSeq1, IterSeq2>>,
    ///         mpl::unpack_args<mpl::distance<_, _>>
    ///     >;
    ///
    /// But since mpl::unique down there uses mpl::clear on the input sequence,
    /// it results in compilation error, because views are not extensible.

    using different_distances =
        typename mpl::unique<
            distance_vector,
            mpl::equal_to<_, _>
        >::type;

    static_assert(
        mpl::size<different_distances>::type::value == 1,
        "iterator invariant violation: not all iterators have the same distance between them"
    );

    using type = typename mpl::front<different_distances>::type;
};

// -----------------------------------------------------------------------------

using seq1 = mpl::vector<char, int, long>;
using seq2 = mpl::vector<long, short, char>;
using zv = ::zip_view<mpl::vector<seq1, seq2>>;

static_assert(
    mpl::equal_to<
        mpl::distance<
            mpl::begin<zv>::type,
            mpl::end<zv>::type
        >::type,
        mpl::int_<3>
    >(),
    ""
);

static_assert(
    mpl::equal<
        mpl::advance<
            mpl::begin<zv>::type,
            mpl::int_<3>
        >::type,
        mpl::end<zv>::type
    >(),
    ""
);

static_assert(
    mpl::equal<
        mpl::deref<
            mpl::begin<
                ::zip_view<mpl::vector<seq1, seq2>>
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
                    ::zip_view<mpl::vector<seq1, seq2>>
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
            mpl::prior<
                mpl::end<
                    ::zip_view<mpl::vector<seq1, seq2>>
                >::type
            >::type
        >::type,
        mpl::vector<long, char>
    >(),
    ""
);

int main()
{
}
