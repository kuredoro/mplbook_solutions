#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector_c.hpp>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

int main()
{
    static_assert(
        mpl::equal<
            mpl::transform<
                mpl::vector_c<int, 1, 2, 3>,
                mpl::plus<_1, mpl::int_<1>>
            >::type,
            mpl::vector_c<int, 2, 3, 4>
        >::value
    );

    return 0;
}
