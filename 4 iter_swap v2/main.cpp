#include <boost/mpl/if.hpp>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <cassert>
#include <vector>

namespace mpl = boost::mpl;

struct fast_swap
{
    template <class ForwardIterator1, class ForwardIterator2>
    static void do_it(ForwardIterator1 i1, ForwardIterator2 i2)
    {
        std::swap(*i1, *i2);
    }
};

struct reliable_swap
{
    template <class ForwardIterator1, class ForwardIterator2>
    static void do_it(ForwardIterator1 i1, ForwardIterator2 i2)
    {
        typename std::iterator_traits<ForwardIterator1>::value_type tmp = *i1;
        *i1 = *i2;
        *i2 = tmp;
    }
};

/*! Effect: Swaps the values the two iterators point to.
 */
template <class ForwardIterator1, class ForwardIterator2>
void iter_swap(ForwardIterator1 i1, ForwardIterator2 i2)
{
    using traits1 = std::iterator_traits<ForwardIterator1>;
    using traits2 = std::iterator_traits<ForwardIterator2>;
    using v1      = typename traits1::value_type;
    using v2      = typename traits2::value_type;
    using r1      = typename traits1::reference;
    using r2      = typename traits2::reference;

    /* When do we choose fast_swap?
     * First, the signature of the std::swap accepts two references
     * to the same type.
     * Our iter_swap supports swapping iterators of different value types...
     * Additionally, dereferencing an iterator might not return a reference.
     * vector<bool> iterators return a proxy reference which is a plain struct.
     * This means that std::swap will take references to two temporary objects,
     * resulting in UB.
     */
    const bool useSwap = std::is_same<v1, v2>::value &&
                             std::is_reference<r1>::value &&
                             std::is_reference<r2>::value;

    mpl::if_<
        mpl::bool_<useSwap>, 
        fast_swap, 
        reliable_swap
    >::type::do_it(i1, i2);
}

int main()
{
    std::vector<int> a{1, 2, 3},
                     b{10, 20, 30};

    ::iter_swap(a.begin(), b.begin());
    assert(a[0] == 10 && b[0] == 1);

    std::vector<bool> x{true, false},
                      y{false, true};

    ::iter_swap(x.begin(), y.begin());
    assert(x[0] == false && y[0] == true);
}
