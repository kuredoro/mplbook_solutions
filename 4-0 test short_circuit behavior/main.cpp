#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

namespace mpl = boost::mpl;

int main()
{
    static_assert(
        mpl::and_<mpl::bool_<false>, int>::value == false
    );

    static_assert(
        mpl::or_<mpl::bool_<true>, int>::value == true
    );
}
