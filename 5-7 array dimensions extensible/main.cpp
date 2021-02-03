#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <type_traits>

namespace mpl = boost::mpl;

template <class T, class State = mpl::vector<>>
struct dimensions
    : State
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
static_assert(
        mpl::at_c<mpl::push_back<seq, mpl::int_<0>>::type, 3>::type::value == 0
);
static_assert(
        mpl::at_c<mpl::push_front<seq, mpl::int_<0>>::type, 0>::type::value == 0
);

int main()
{
}
