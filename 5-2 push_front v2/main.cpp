#include "tiny.hpp"

/*
 * mpl::push_front pushes the values one step right and puts the T
 * into the first position. 
 * <!> Does not cause a compilation error if
 * tiny is full.
 */
template <>
struct mpl::push_front_impl<tiny_tag>
{
    template <class Tiny, class T>
    struct apply
        : tiny<T, typename Tiny::t0, typename Tiny::t1>
    {
        static_assert(std::is_same<typename Tiny::t2, none>::value);
    };
};

static_assert(
    std::is_same<
        mpl::push_front<tiny<int>, long>::type,
        tiny<long, int>
    >::value
);

/* Won't compile
static_assert(
    std::is_same<
        mpl::push_front<tiny<short, int, long>, char>::type,
        tiny<char, short, int>
    >::value
);
*/

int main()
{
}
