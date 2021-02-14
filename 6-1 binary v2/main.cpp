#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/times.hpp>
namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

/// The indirection via digits_impl is needed to correctly handle the
/// case of digits<0>
template <int64_t N>
struct digits_impl
    : mpl::push_back<typename digits_impl<N / 10>::type, mpl::int_<N % 10>>::type
{};

template <>
struct digits_impl<0>
    : mpl::vector<>
{};


/// digits<N> is semantically equivalent to the mpl::vector whose elements
/// are ihe digits of N in the same order.
template <int64_t N>
struct digits
    : digits_impl<N>
{};

template <>
struct digits<0>
    : mpl::vector_c<int, 0>
{};

static_assert(
    mpl::equal<
        digits<1234567890>,
        mpl::vector_c<int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0>,
        mpl::equal_to<_, _>
    >(),
    ""
);

static_assert(
    mpl::equal<
        digits<0>,
        mpl::vector_c<int, 0>,
        mpl::equal_to<_, _>
    >(),
    ""
);

// -----------------------------------------------------------------------------

template <int64_t N>
using binary =
    typename mpl::fold<digits<N>, mpl::int_<0>, mpl::plus<mpl::times<_1, mpl::int_<2>>, _2>>::type;

static_assert(binary<0>::value == 0, "");
static_assert(binary<1>::value == 1, "");
static_assert(binary<10>::value == 2, "");
static_assert(binary<11>::value == 3, "");
static_assert(binary<100>::value == 4, "");
static_assert(binary<101>::value == 5, "");
static_assert(binary<110>::value == 6, "");
static_assert(binary<111>::value == 7, "");

/// The amount of code this version of binary's takin' is amazingly small.
/// It was possible via reuse of fold algorithm, the sructure of binary v1
/// resembled.

/// So... algorithm reuse! Yay Yay!

int main()
{
}
