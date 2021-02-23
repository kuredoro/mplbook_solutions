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


/// is_same produces a valid mpl integral constant,
/// unlike std::is_same which produces std::integral_constant
/// that's missing a `tag` field and whole bunch of other stuff
/// See https://www.boost.org/doc/libs/1_75_0/libs/mpl/doc/refmanual/integral-constant.html
/// and https://en.cppreference.com/w/cpp/types/integral_constant
template <class A, class B>
using is_same =
    mpl::bool_<
        std::is_same<A, B>::value
    >;


template <class X, class Denumerator>
using is_divisible =
    mpl::equal_to<
        mpl::modulus<X, Denumerator>,
        mpl::int_<0>
    >;

// -----------------------------------------------------------------------------

/// is_not_divisible_by_any is not a type alias because if it was, it would be
/// impossible to use it inside any other lambda if N to be a placeholder.
/// The placeholders from the outer lambda and this lambda would be merged
/// together inside is_divisible and on lambda's call both of them would be
/// filled with the first argument. But what _we_ want is only the first
/// placeholder to be replaced -- N.
template <class N, class Divs>
struct is_not_divisible_by_any
{
    using type =
        typename is_same<
            typename mpl::find_if<
                Divs,
                is_divisible<N, _1>
            >::type,
            typename mpl::end<Divs>::type
        >::type;

    static constexpr bool value = type::value;
};


static_assert(
    is_not_divisible_by_any<mpl::int_<5>, mpl::vector_c<int, 2, 3>>::value,
    ""
);

static_assert(
    is_not_divisible_by_any<mpl::int_<7>, mpl::vector_c<int, 2, 3>>::value,
    ""
);

static_assert(
    mpl::equal<
        mpl::transform<
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


/// So, um, even this code won't print any warnings. (Although book says it
/// should). Also, there's no mpl::print in the latest (1.75) reference.
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
