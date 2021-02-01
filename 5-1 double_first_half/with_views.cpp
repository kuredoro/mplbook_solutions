#include <boost/mpl/int.hpp>
#include <boost/mpl/multiplies.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/joint_view.hpp>
#include <type_traits>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

template <class Sequence>
using past_middle =
    mpl::advance<
        typename mpl::begin<Sequence>::type,
        typename mpl::divides<
            typename mpl::size<Sequence>::type,
            mpl::int_<2>
        >::type
    >;

static_assert(
    std::is_same<
        mpl::deref<
            past_middle<mpl::vector<char, short, int, long>>::type
        >::type,
        int
    >::value
);

static_assert(
    std::is_same<
        mpl::deref<
            past_middle<mpl::vector<char, int, long>>::type
        >::type,
        int
    >::value
);


/*
 * double_first_half accepts a RandomAccessSequence of integral_c<>
 */
template <class Seq>
using double_first_half =
    mpl::joint_view<
        mpl::transform_view<
            mpl::iterator_range<
                typename mpl::begin<Seq>::type,
                typename past_middle<Seq>::type
            >,
            mpl::multiplies<mpl::int_<2>, _>
        >,
        mpl::iterator_range<
            typename past_middle<Seq>::type,
            typename mpl::end<Seq>::type
        >
    >;

static_assert(
    mpl::equal<
        double_first_half<mpl::vector_c<int, 2, 3, 4, 5>>::type,
        mpl::vector_c<int, 4, 6, 4, 5>
    >()
);

int main()
{
}
