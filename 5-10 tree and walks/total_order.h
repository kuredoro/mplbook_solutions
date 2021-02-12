#include "tree.h"

#include <boost/mpl/plus.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector/vector30.hpp>

template <class ...Stack>
struct total_order_iterator;

template <class Tree, class ...Stack>
struct total_order_iterator<number<1>, Tree, Stack...>
{
    using category = mpl::forward_iterator_tag;
    using type = total_order_iterator<number<1>, Tree, Stack...>;
};

template <class Tree, class ...Stack>
struct total_order_iterator<number<2>, Tree, Stack...>
{
    using category = mpl::forward_iterator_tag;
    using type = total_order_iterator<number<2>, Tree, Stack...>;
};

template <class Tree, class ...Stack>
struct total_order_iterator<number<3>, Tree, Stack...>
{
    using category = mpl::forward_iterator_tag;
    using type = total_order_iterator<number<3>, Tree, Stack...>;
};

/// Past-the-end iterator
template <>
struct total_order_iterator<number<4>>
{
    using type = total_order_iterator<number<4>>;
};

using total_order_end_iterator = total_order_iterator<number<4>>;


/// Traverse the left child of the tree as the first thing when we visit a
/// node.
template <class Node, class Left, class Right, class ...Stack>
struct mpl::next<total_order_iterator<number<1>, tree<Node, Left, Right>, Stack...>>
    : total_order_iterator<number<1>, Left, number<1>, tree<Node, Left, Right>, Stack...>
{};

/// The leaf nodes don't have any children, but to implement pre/in/post-order iterators
/// as filters of total_order_iterators whose VisitCount is a fixed value,
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

// The routine for backtracking to the parent if leaf is redundant here,
// since the implementation given in this file doesn't care if
// the node is internal or a leaf.

/// After traversing the left child the next thing to do is to traverse the right one.
template <class Node, class Left, class Right, class ...Stack>
struct mpl::next<total_order_iterator<number<2>, tree<Node, Left, Right>, Stack...>>
    : total_order_iterator<number<1>, Right, number<2>, tree<Node, Left, Right>, Stack...>
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
    : total_order_end_iterator
{};

// -----------------------------------------------------------------------------

template <class VisitCount, class Node, class Left, class Right, class ...Stack>
struct mpl::deref<total_order_iterator<VisitCount, tree<Node, Left, Right>, Stack...>>
{
    using type = Node;
};

template <class VisitCount, class Leaf, class ...Stack>
struct mpl::deref<total_order_iterator<VisitCount, Leaf, Stack...>>
{
    using type = Leaf;
};

// -----------------------------------------------------------------------------

struct total_order_view_tag{};

/// A sequence to quickly test out how the total_order_iterator behaves.
template <class Tree>
struct total_order_view
{
    using tag = total_order_view_tag;
    using type = total_order_view<Tree>;
    using tree = Tree;
};


template <>
struct mpl::begin_impl<total_order_view_tag>
{
    template <class View>
    struct apply
        : total_order_iterator<number<1>, typename View::tree>
    {};
};

template <>
struct mpl::end_impl<total_order_view_tag>
{
    template <class View>
    struct apply
        : total_order_end_iterator
    {};
};


using treeSmall = tree<int, short, long>;

static_assert(
    mpl::equal<
        total_order_view<treeSmall>,
        mpl::vector<int, short, short, short, int, long, long, long, int>
    >(),
    ""
);

using treeBig = 
    tree<
        int,
        tree<short, char, unsigned short>, 
        tree<long, unsigned, unsigned long>
    >;

static_assert(
    mpl::equal<
        total_order_view<treeBig>,
        mpl::vector21<int, short, char, char, char, short, unsigned short, unsigned short, unsigned short, short, int, long, unsigned, unsigned, unsigned, long, unsigned long, unsigned long, unsigned long, long, int>
    >(),
    ""
);
