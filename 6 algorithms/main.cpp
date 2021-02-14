#include <boost/mpl/fold.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/times.hpp>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

static_assert(
    mpl::fold<mpl::vector_c<int, 1, 2, 3>, mpl::int_<0>, mpl::plus<_1, _2>>::type::value == 6,
    ""
);

using seq =
    mpl::vector<
        mpl::vector_c<int, 1, 2, 3>,
        mpl::vector_c<int, 4, 5, 6>,
        mpl::vector_c<int, 7, 8, 9>
    >;

using sum =
    mpl::transform<
        seq,
        mpl::front<_>,
        mpl::inserter<
            mpl::int_<0>,
            mpl::plus<_, _>
        >
    >::type;

static_assert(sum::value == 12, "");


using seq2 = mpl::vector_c<int, 1, 2, 3>;

using sum2 =
    mpl::reverse_fold<seq2, mpl::int_<0>, mpl::times<_, _>, mpl::plus<_, _>>::type;

static_assert(sum2::value == 36, ""); // (1 + 2 + 3) * 3 * 2 * 1

int main()
{
}
