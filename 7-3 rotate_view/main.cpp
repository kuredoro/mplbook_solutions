#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/iterator_range.hpp>
namespace mpl = boost::mpl;

template <class Seq, class BeginIt>
using rotate_view =
    mpl::joint_view<
        mpl::iterator_range<
            BeginIt,
            typename mpl::end<Seq>::type
        >,
        mpl::iterator_range<
            typename mpl::begin<Seq>::type,
            BeginIt
        >
    >;

using v = mpl::vector_c<int, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4>;
using view =
    rotate_view<
        v,
        mpl::advance_c<mpl::begin<v>::type, 5>::type
    >;

static_assert(
    mpl::equal<
        view,
        mpl::range_c<int, 0, 10>
    >(),
    ""
);

int main()
{
}
