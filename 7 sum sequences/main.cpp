#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/unpack_args.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

using v1 = mpl::vector_c<int, 1, 2, 3, 4>;
using v2 = mpl::vector_c<int, 4, 3, 2, 1>;
using v3 = mpl::vector_c<int, 1, 5, 5, 1>;
using result = mpl::vector_c<int, 6, 10, 10, 6>;

using v4 =
    mpl::transform_view<
        mpl::zip_view<mpl::vector<v1, v2, v3>>,
        mpl::plus<
            mpl::at<_, mpl::int_<0>>,
            mpl::at<_, mpl::int_<1>>,
            mpl::at<_, mpl::int_<2>>
        >
    >;

static_assert(mpl::equal<v4, result, mpl::equal_to<_, _>>(), "");

using v5 =
    mpl::transform_view<
        mpl::zip_view<mpl::vector<v1, v2, v3>>,
        mpl::unpack_args<mpl::plus<_, _, _>>
    >;

static_assert(mpl::equal<v5, result, mpl::equal_to<_, _>>(), "");

int main()
{
}
