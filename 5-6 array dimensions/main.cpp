#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <type_traits>

namespace mpl = boost::mpl;

template <class T, class State = mpl::vector<>>
struct dimensions
    : mpl::iterator_range<                  // make it non-extensible
        typename mpl::begin<State>::type,
        typename mpl::end<State>::type
      >
{};

template <class SubArray, class State, int N>
struct dimensions<SubArray[N], State>
    : dimensions<
        SubArray, 
        typename mpl::push_front<State, mpl::int_<N>>::type
      >
{};


using seq = dimensions<char [10][5][2]>;

static_assert(mpl::size<seq>::value == 3);
static_assert(mpl::at_c<seq, 0>::type::value == 2);
static_assert(mpl::at_c<seq, 1>::type::value == 5);
static_assert(mpl::at_c<seq, 2>::type::value == 10);

int main()
{
}
