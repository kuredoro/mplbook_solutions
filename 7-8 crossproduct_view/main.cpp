#include <boost/config/detail/suffix.hpp>
#include <type_traits>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/advance.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

/// Utility
template <class Sequence>
using last_element_t =
    typename mpl::advance<
        typename mpl::begin<Sequence>::type,
        typename mpl::minus<
            typename mpl::size<Sequence>::type,
            mpl::int_<1>
        >::type
    >::type;

// -----------------------------------------------------------------------------

template <class RightIt, class LeftIt, class LeftSeq>
struct crossproduct_iterator
{
    using category = mpl::forward_iterator_tag;

    using type =
        mpl::pair<
            typename mpl::deref<RightIt>::type,
            typename mpl::deref<LeftIt>::type
        >;
};

template <class RightIt, class LeftIt, class LeftSeq>
struct mpl::next<crossproduct_iterator<RightIt, LeftIt, LeftSeq>>
{
    using type =
        typename mpl::if_c<
            std::is_same<
                LeftIt,
                last_element_t<LeftSeq>
            >::value,
            crossproduct_iterator<
                typename mpl::next<RightIt>::type,
                typename mpl::begin<LeftSeq>::type,
                LeftSeq
            >,
            crossproduct_iterator<
                RightIt,
                typename mpl::next<LeftIt>::type,
                LeftSeq
            >
        >::type;
};

// -----------------------------------------------------------------------------

template <class Seq1, class Seq2>
struct crossproduct_view
    : mpl::iterator_range<
        crossproduct_iterator<
            typename mpl::begin<Seq1>::type,
            typename mpl::begin<Seq2>::type,
            Seq2
        >,
        crossproduct_iterator<
            typename mpl::end<Seq1>::type,
            typename mpl::begin<Seq2>::type,
            Seq2
        >
      >
{};

using crossprod =
    crossproduct_view<
        mpl::vector_c<int, 1, 2, 3>,
        mpl::vector_c<int, 1, 2, 3>
    >;

static_assert(
    mpl::size<crossprod>::type::value == 9,
    ""
);

static_assert(
    mpl::equal<
        crossprod,
        mpl::vector<
            mpl::pair<mpl::int_<1>, mpl::int_<1>>,
            mpl::pair<mpl::int_<1>, mpl::int_<2>>,
            mpl::pair<mpl::int_<1>, mpl::int_<3>>,
            mpl::pair<mpl::int_<2>, mpl::int_<1>>,
            mpl::pair<mpl::int_<2>, mpl::int_<2>>,
            mpl::pair<mpl::int_<2>, mpl::int_<3>>,
            mpl::pair<mpl::int_<3>, mpl::int_<1>>,
            mpl::pair<mpl::int_<3>, mpl::int_<2>>,
            mpl::pair<mpl::int_<3>, mpl::int_<3>>
        >,
        mpl::and_<
            mpl::equal_to<mpl::first<_1>, mpl::first<_2>>,
            mpl::equal_to<mpl::second<_1>, mpl::second<_2>>
        >
    >(),
    ""
);

int main()
{
}
