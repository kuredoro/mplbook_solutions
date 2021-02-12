#include "tree.h"

#include <type_traits>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/advance.hpp>

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;


template <class ...Stack>
struct inorder_iterator;

/// These are metaprogramming values
/// They are the terminating cases for the inheritance recursion used below.

template <class ...Stack>
struct inorder_iterator
{
    using category = mpl::forward_iterator_tag;
    using type = inorder_iterator<Stack...>;
};

/// The nodes that are visited twice are the ones we iterate on.
/// Hence iterators pointing to these nodes should be metavalues.
template <class Leaf, class ...Stack>
struct inorder_iterator<number<1>, Leaf, Stack...>
{
    using category = mpl::forward_iterator_tag;
    using type = inorder_iterator<number<1>, Leaf, Stack...>;
};

template <class Leaf, class ...Stack>
struct inorder_iterator<number<0>, Leaf, Stack...>
    : inorder_iterator<number<1>, Leaf, Stack...>
{};


template <class Node, class Left, class Right, class ...Stack>
struct inorder_iterator<number<0>, tree<Node, Left, Right>, Stack...>
    : inorder_iterator<
        number<0>, Left, 
        number<0>, tree<Node, Left, Right>, 
        Stack...
    >
{};

template <class TraversedTree, class VisitCount, class Tree, class ...Stack>
struct inorder_iterator<number<2>, TraversedTree, VisitCount, Tree, Stack...>
    : inorder_iterator<typename mpl::plus<VisitCount, number<1>>::type, Tree, Stack...>
{};

// -------------------------------------------------------------------------

//template <class ...Stack>
//struct deref;

template <class Leaf, class ...Stack>
struct mpl::deref<inorder_iterator<number<1>, Leaf, Stack...>>
{
    using type = Leaf;
};

template <class Node, class Left, class Right, class ...Stack>
struct mpl::deref<inorder_iterator<number<1>, tree<Node, Left, Right>, Stack...>>
{
    using type = Node;
};

// -------------------------------------------------------------------------

template <class Leaf, class ...Stack>
struct mpl::next<inorder_iterator<number<1>, Leaf, Stack...>>
    : inorder_iterator<number<2>, Leaf, Stack...>
{};

template <class Node, class Left, class Right, class ...Stack>
struct mpl::next<inorder_iterator<number<1>, tree<Node, Left, Right>, Stack...>>
    : inorder_iterator<number<0>, Right, number<1>, tree<Node, Left, Right>, Stack...>
{};



struct inorder_view_tag {};

template <class Tree>
struct inorder_view
{
    using tag = inorder_view_tag;
    using tree = Tree;
    using type = inorder_view<Tree>;
};


template <>
struct mpl::begin_impl<inorder_view_tag>
{
    template <class View>
    struct apply
        : inorder_iterator<number<0>, typename View::tree>
    {};
};

template <>
struct mpl::end_impl<inorder_view_tag>
{
    template <class View>
    struct apply
        : inorder_iterator<number<2>, typename View::tree>
    {};
};

template <>
struct mpl::size_impl<inorder_view_tag>
{
    template <class View>
    struct apply
        : tree_size<typename View::tree>::type
    {};
};


static_assert(
    mpl::equal<
        mpl::vector<int, void*, long, double, char>,
        inorder_view<treeSeq>
    >::type::value,
    ""
);

int main()
{
}
