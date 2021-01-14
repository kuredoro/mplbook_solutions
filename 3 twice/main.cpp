#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;


// ---------
// Version 1
// ---------
template <class F, class X>
struct twice1
    : F::template apply<
        typename F::template apply<X>::type
      >
{};

// ---------
// Version 2 (using our custom apply1)
// ---------
template <class UnaryMetafunctionClass, class Arg>
struct apply1
    : UnaryMetafunctionClass::template apply<Arg>
{};

template <class F, class X>
struct twice2
    : apply1<F, typename apply1<F, X>::type>
{};

// ---------
// Version 3 (using apply1 and lambda)
// ---------
template <class F, class X>
struct twice3
    : apply1<
        typename mpl::lambda<F>::type,
        typename apply1<
            typename mpl::lambda<F>::type,
            X
        >::type
    >
{};

// ---------
// Version 4 (using apply)
// ---------
template <class F, class X>
struct twice4
    : mpl::apply<F, typename mpl::apply<F, X>::type>
{};

// ---------
// Utilities for testing purposes
// ---------
template <class F, class X>
using twice = twice3<F, X>;

struct add_pointer_f
{
    template <class T>
    struct apply
    {
        typedef T* type;
    };
};

template <class T>
struct two_pointers
    : twice2<
        typename mpl::lambda<boost::add_pointer<_1>>::type, 
        T
    >
{};

int main()
{
    static_assert(
        boost::is_same<
            twice2<add_pointer_f, int>::type,
            int**
        >::value
    );

    static_assert(
        boost::is_same<
            two_pointers<int>::type,
            int**
        >::value
    );

    static_assert(
        boost::is_same<
            twice3<boost::add_pointer<_1>, int>::type,
            int**
        >::value
    );

    static_assert(
        boost::is_same<
            twice4<boost::add_pointer<_1>, int>::type,
            int**
        >::value
    );
    return 0;
}
