#include "binary_tree_insert.h"
#include "views.h"

#include <boost/mpl/equal.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/iterator_range.hpp>

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

/// Invariant: It always points to the current position within the tree.
template<
      class Tree
    , class Value
    , class It = total_order_iterator<number<1>, Tree>
    >
struct binary_tree_search;

/// Value matches: return the iterator
/// (The pattern matching sucks here a bit, so I needed to specify verbosily
/// the iterator here also).
template <class Value, class Left, class Right, class Tree, class ...Stack>
struct binary_tree_search<tree<Value, Left, Right>, Value, total_order_iterator<number<1>, Tree, Stack...>>
    : total_order_iterator<number<1>, Tree, Stack...>
{};

template <class Value, class It>
struct binary_tree_search<tree<>, Value, It>
    : total_order_end_iterator
{};

template <class Node, class Left, class Right, class Value, class Tree, class ...Stack>
struct binary_tree_search<tree<Node, Left, Right>, Value, total_order_iterator<number<1>, Tree, Stack...>>
    : mpl::if_<
        typename mpl::less<Value, Node>::type,
        binary_tree_search<
            Left, 
            Value, 
            total_order_iterator<
                number<1>, Left, 
                number<1>, Tree, 
                Stack...
            >
        >,
        binary_tree_search<
            Right, 
            Value, 
            total_order_iterator<
                number<1>, Right, 
                number<2>, Tree, 
                Stack...
            >
        >
    >::type
{};


using bst =
    mpl::copy<
        mpl::vector_c<int, 17, 25, 10, 2, 11>,
        binary_tree_inserter<tree<>>
    >::type;


static_assert(
    mpl::equal<
        inorder_view<bst>,
        mpl::vector_c<int, 2, 10, 11, 17, 25>,
        mpl::equal_to<_, _>
    >(),
    ""
);


using pos1 = binary_tree_search<bst, number<11>>::type;
using pos2 = binary_tree_search<bst, number<20>>::type;

static_assert(!std::is_same<pos1, total_order_end_iterator>(), "");
static_assert(std::is_same<pos2, total_order_end_iterator>(), "");

/// Refer to this:
/// bst =
///      17
///     /  \
///   10    25
///  /  \
/// 2    11
static_assert(
    mpl::equal<
        mpl::iterator_range<pos1, total_order_end_iterator>,
        mpl::vector_c<int, 11, 11, 11, 10, 17, 25, 25, 25, 17>,
        mpl::equal_to<_, _>
    >(),
    ""
);

int main()
{
}
