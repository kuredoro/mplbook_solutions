#include "tree.h"

#include <type_traits>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/advance.hpp>

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

template <class ...Stack>
struct total_order_iterator;

template <class Tree, class ...Stack>
struct total_order_iterator<number<1>, Tree, Stack...>
{
    using type = total_order_iterator<number<1>, Tree, Stack...>;
};

template <class Tree, class ...Stack>
struct total_order_iterator<number<2>, Tree, Stack...>
{
    using type = total_order_iterator<number<2>, Tree, Stack...>;
};

template <class Tree, class ...Stack>
struct total_order_iterator<number<3>, Tree, Stack...>
{
    using type = total_order_iterator<number<3>, Tree, Stack...>;
};

/// Past-the-end iterator
template <>
struct total_order_iterator<number<4>>
{
    using type = total_order_iterator<number<4>>;
};


/// Traverse the left child of the tree as the first thing when we visit a
/// node.
template <class Node, class Left, class Right, class ...Stack>
struct mpl::next<total_order_iterator<number<1>, tree<Node, Left, Right>, Stack...>>
    : total_order_iterator<number<1>, Left, number<1>, tree<Node, Left, Right>, Stack...>
{};

/// The leaf nodes don't have any children, but to implement pre/in/post-order iterators
/// as filterns of total_order_iterator whose VisitCount is a fixed value,
/// the leaf nodes must be visited with all possible visit counts: 1, 2, and 3
/// to be correctly handled.
template <class Leaf, class ...Stack>
struct mpl::next<total_order_iterator<number<1>, Leaf, Stack...>>
    : total_order_iterator<number<2>, Leaf, Stack...>
{};

template <class Leaf, class ...Stack>
struct mpl::next<total_order_iterator<number<2>, Leaf, Stack...>>
    : total_order_iterator<number<3>, Leaf, Stack...>
{};

/// All we can do when advancing from leaf is to come up to its parent.
template <class Leaf, class VisitCount, class Parent, class ...Stack>
struct mpl::next<total_order_iterator<number<3>, Leaf, VisitCount, Parent, Stack...>>
    : total_order_iterator<typename mpl::plus<VisitCount, number<1>>::type, Parent, Stack...>
{};

/// We know that if we visit a node twice then it has two children, so quite verbose
/// tree<Node, Lef, Right> in the template is not necessary.
/// After traversing the left child the next thing to do is to traverse the right one.
template <class Node, class ...Stack>
struct mpl::next<total_order_iterator<number<2>, Node, Stack...>>
    : total_order_iterator<number<1>, typename Node::right, number<2>, Node, Stack...>
{};

/// After traversing both children, the last thing is to come back to the parent,
/// increasing its visit count in the process.
template <class Node, class VisitCount, class Parent, class ...Stack>
struct mpl::next<total_order_iterator<number<3>, Node, VisitCount, Parent, Stack...>>
    : total_order_iterator<typename mpl::plus<VisitCount, number<1>>::type, Parent, Stack...>
{};

/// The root node has no parent, hence it's a corner case that results in the past-the-end
/// iterator.
template <class Root>
struct mpl::next<total_order_iterator<number<3>, Root>>
    : total_order_iterator<number<4>>
{};
