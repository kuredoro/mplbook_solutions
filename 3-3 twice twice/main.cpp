#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;


template <class F, class X>
struct twice
    : mpl::apply<F, typename mpl::apply<F, X>::type>
{};

typedef typename twice<boost::add_pointer<_>, int>::type once;
typedef twice<boost::add_pointer<_>, once> four_pointers;

int main()
{
    static_assert(
        boost::is_same<
            four_pointers::type,
            int****
        >::value
    );
    return 0;
}
