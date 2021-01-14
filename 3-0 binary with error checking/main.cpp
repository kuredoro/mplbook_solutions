#include <boost/mpl/plus.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/mpl/int.hpp>

namespace mpl = boost::mpl;

// binary<N> = 2 * binary<N / 10> + N % 10
template <int N>
struct binary
    : mpl::plus<
        typename mpl::shift_left<
            typename binary<N / 10>::type,
            mpl::int_<1>
        >::type,
        mpl::int_<N % 10>
    >
{
    static_assert(N % 10 == 0 || N % 10 == 1, "binary representation contains foreign digits");
};

template <>
struct binary<0>
    : mpl::int_<0>
{};

int main()
{
    static_assert(binary<0>::value == 0);
    static_assert(binary<1>::value == 1);
    static_assert(binary<10>::value == 2);
    static_assert(binary<11>::value == 3);
    static_assert(binary<100>::value == 4);
    static_assert(binary<101>::value == 5);
    static_assert(binary<110>::value == 6);
    static_assert(binary<111>::value == 7);
    static_assert(binary<1000>::value == 8);

    // Throws error
    //static_assert(binary<1234>::value == 8);
    return 0;
}
