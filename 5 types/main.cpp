#include <boost/mpl/advance_fwd.hpp>
#include <boost/mpl/push_front_fwd.hpp>
#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/push_front.hpp>

namespace mpl = boost::mpl;

using types = mpl::vector<char, short, int, long, float, double>;

using long_it = mpl::find<types, long>::type;
static_assert(std::is_same<mpl::deref<long_it>::type, long>::value);
static_assert(!std::is_same<long_it, mpl::end<types>::type>::value);

using float_it = mpl::next<long_it>::type;
static_assert(std::is_same<mpl::deref<float_it>::type, float>::value);

using not_found = mpl::find<types, int*>::type;
static_assert(std::is_same<not_found, mpl::end<types>::type>::value);

using double_it = mpl::advance<long_it, mpl::int_<2>>::type;
static_assert(std::is_same<mpl::deref<double_it>::type, double>::value);
static_assert(mpl::distance<long_it, double_it>::value == 2);
static_assert(mpl::distance<float_it, long_it>::value == -1);

using types2 = mpl::push_front<types, int>::type;

int main()
{
}
