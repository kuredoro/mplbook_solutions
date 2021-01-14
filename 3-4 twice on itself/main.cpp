#include <boost/mpl/lambda.hpp>
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

struct add_pointer_f
    : mpl::lambda<boost::add_pointer<_>>::type
{};

int main()
{
    static_assert(
        boost::is_same<
            twice<twice<add_pointer_f, _1>, int>::type,
            int****
        >::value
    );

    static_assert(
        boost::is_same<
            twice<
                twice<
                    mpl::lambda<
                        boost::add_pointer<_>
                    >::type,
                    _1
                >, 
                int
            >::type,
            int****
        >::value
    );
    return 0;
}
