#include <type_traits>
#include <boost/mpl/integral_c.hpp>
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

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

template <class T>
struct tree_size;

template <class Node, class Left, class Right>
struct tree
{
    using size = tree_size<tree<Node, Left, Right>>;
    using left = Left;
    using right = Right;
};

template<int N>
using number = mpl::integral_c<int, N>;

using treeSeq =
    tree<
        double,
        tree<void*, int, long>,
        char
    >;

template <class T>
struct tree_size
    : number<1>
{};

template <class Node, class Left, class Right>
struct tree_size<tree<Node, Left, Right>>
    : mpl::plus<typename mpl::plus<tree_size<Left>, tree_size<Right>>::type, number<1>>
{};

static_assert(tree_size<tree<int, int, int>>::value == 3);
static_assert(tree_size<treeSeq>::value == 5);


template <class ...Stack>
struct inorder_iterator
{
    using category = mpl::forward_iterator_tag;
    using type = inorder_iterator<Stack...>;
};

template <class Leaf, class ...Stack>
struct inorder_iterator<number<0>, Leaf, Stack...>
    : inorder_iterator<number<1>, Leaf, Stack...>
{
    using category = mpl::forward_iterator_tag;
    using type = typename inorder_iterator<number<1>, Leaf, Stack...>::type;
};

static_assert(
    std::is_same<
        inorder_iterator<number<0>, int>::type,
        inorder_iterator<number<1>, int>
    >(),
    ""
);

template <class Node, class Left, class Right, class ...Stack>
struct inorder_iterator<number<0>, tree<Node, Left, Right>, Stack...>
    : inorder_iterator<
        number<0>, Left, 
        number<0>, tree<Node, Left, Right>, 
        Stack...
    >
{
    using category = mpl::forward_iterator_tag;
    using type = 
        typename inorder_iterator<
            number<0>, Left, 
            number<0>, tree<Node, Left, Right>, 
            Stack...
        >::type;
};

template <class TraversedTree, class Branch, class Tree, class ...Stack>
struct inorder_iterator<number<2>, TraversedTree, Branch, Tree, Stack...>
    : inorder_iterator<typename mpl::plus<Branch, number<1>>::type, Tree, Stack...>
{
    using category = mpl::forward_iterator_tag;
    using type = typename inorder_iterator<typename mpl::plus<Branch, number<1>>::type, Tree, Stack...>::type;
};

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
    >::type::value
);

int main()
{
}
