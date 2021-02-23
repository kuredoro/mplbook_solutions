#include <type_traits>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/transform.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;


template <class X, class Denumerator>
using is_divisible =
    mpl::equal_to<
        mpl::modulus<X, Denumerator>,
        mpl::int_<0>
    >;

/// When I wrote this, there was just is_not_divisible_by_any, which was a type
/// alias. The problem that I had is that this type alias was used in another
/// lambda (e.g., gen_primes_less_than) which defined N to be a placeholder.
/// It essentially made it impossible to resolve the lambda's placeholders,
/// and the mpl::protect could be of no help, since the two placeholders
/// were intertwined within one template instantiation.
///
/// Then it turned that std::integral_constant doesn't interoperate with MPL...
/// But due to this the code needed to be changed to this version.
/// It turned out that this constexpr bool does a great job protecting the
/// _1 placeholder from any other lambdas into which this template might be
/// nested.
template <class N, class Divs>
constexpr bool is_not_divisible_by_any_v =
    std::is_same<
        typename mpl::find_if<
            Divs,
            is_divisible<N, _1>
        >::type,
        typename mpl::end<Divs>::type
    >::value;


static_assert(
    is_not_divisible_by_any_v<mpl::int_<5>, mpl::vector_c<int, 2, 3>>,
    ""
);

static_assert(
    is_not_divisible_by_any_v<mpl::int_<7>, mpl::vector_c<int, 2, 3>>,
    ""
);

// -----------------------------------------------------------------------------

template <class N, class Divs>
struct is_not_divisible_by_any
{
    /// Soo... std::integral_constant is not a model of mpl's integral constant
    /// since it doesn't have a `tag` alias inside of it.
    /// https://www.boost.org/doc/libs/1_75_0/libs/mpl/doc/refmanual/integral-constant.html
    /// Hence, I'm wrapping its raw boolean value into a bool_, so that the mpl::equal
    /// succeeds.
    using type = mpl::bool_<is_not_divisible_by_any_v<N, Divs>>;
};


static_assert(
    mpl::equal<
        mpl::transform<
            //mpl::range<int, 2, 4>,
            mpl::vector_c<int, 2, 3, 4, 5, 6, 7, 8, 9, 10>,
            is_not_divisible_by_any<_, mpl::vector_c<int, 2, 3>>
        >::type,
        mpl::vector_c<bool, false, false, false, true, false, true, false, false, false>,
        mpl::equal_to<_, _>
    >(),
    ""
);

// -----------------------------------------------------------------------------

template <int Last>
using gen_primes_less_than =
    typename mpl::fold<
        mpl::range_c<int, 2, Last>,
        mpl::vector<>,
        mpl::eval_if<
            is_not_divisible_by_any<_2, _1>,
            mpl::push_back<_1, _2>,
            _1
        >
    >::type;

static_assert(
    mpl::equal<
        gen_primes_less_than<3>,
        mpl::vector_c<int, 2>
    >(),
    ""
);

static_assert(
    mpl::equal<
        gen_primes_less_than<10>,
        mpl::vector_c<int, 2, 3, 5, 7>
    >(),
    ""
);

static_assert(
    mpl::equal<
        gen_primes_less_than<30>,
        mpl::vector_c<int, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29>
    >(),
    ""
);


/// So, um, even this code won't print any warnings.
/// Also, there's no mpl::print in the latest (1.75) reference.
///
/// #include <boost/mpl/range_c.hpp>
/// #include <boost/mpl/fold.hpp>
/// #include <boost/mpl/placeholders.hpp>
/// using namespace mpl::placeholders;
/// 
/// template <class T, class U>
/// struct plus_dbg
/// {
///     using type =
///         typename mpl::print<
///             typename mpl::plus<T, U>::type
///         >::type;
/// };
/// 
/// using sum =
///     mpl::fold<
///         mpl::range_c<int, 1, 6>,
///         mpl::int_<0>,
///         plus_dbg<_1, _2>
///     >::type;

int main()
{
}
