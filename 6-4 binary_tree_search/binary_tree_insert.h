#include "tree.h"
#include <type_traits>

#include <boost/mpl/copy.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/less.hpp>

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

template <class Tree, class Value>
struct binary_tree_insert;

template <class Value>
struct binary_tree_insert<tree<>, Value>
    : tree<Value, tree<>, tree<>>
{};

template <class Node, class Left, class Right, class Value>
struct binary_tree_insert<tree<Node, Left, Right>, Value>
    : mpl::if_<
        mpl::less<Value, Node>,
        tree<Node, typename binary_tree_insert<Left, Value>::type, Right>,
        tree<Node, Left, typename binary_tree_insert<Right, Value>::type>
    >::type
{};

static_assert(
    std::is_same<
        tree<int, tree<>, tree<>>,
        binary_tree_insert<tree<>, int>::type
    >(),
    ""
);

// -----------------------------------------------------------------------------

template <class InitValue>
using binary_tree_inserter =
    mpl::inserter<
        InitValue,
        binary_tree_insert<_1, _2>
    >;
