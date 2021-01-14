#include <boost/mpl/multiplies.hpp>
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
                mpl::multiplies<_1, _1>
            >::type,
            mpl::vector_c<int, 1, 4, 9>
        >::value
    );

    return 0;
}
