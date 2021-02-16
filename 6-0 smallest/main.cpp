#include <type_traits>
#include <limits>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/min.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/front.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

template <class Seq>
using smallest =
    mpl::copy<
        Seq,
        mpl::inserter<
            typename mpl::front<Seq>::type,
            mpl::if_<
                mpl::greater<
                    mpl::sizeof_<_1>,
                    mpl::sizeof_<_2>
                >,
                _2,
                _1
            >
        >
    >;

using vec = mpl::vector<int[2], char, double&>;

static_assert(
    std::is_same<
        smallest<vec>::type,
        char
    >(),
    ""
);

/// Alternatively, smallest could be implemented via fold.
/// In my vision, mpl::fold will clearly represent what this
/// metafunction actually does
/// without any obfuscation in the form of mpl::inserter

int main()
{
}
