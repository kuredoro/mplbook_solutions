#pragma once

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/plus.hpp>

namespace mpl = boost::mpl;

template <class T>
struct tree_size;

template <class Node, class Left, class Right>
struct tree
{
    using size = tree_size<tree<Node, Left, Right>>;
    using left = Left;
    using right = Right;
    using payload = Node;
};

template<int N>
using number = mpl::integral_c<int, N>;

template <class T>
struct tree_size
    : number<1>
{};

template <class Node, class Left, class Right>
struct tree_size<tree<Node, Left, Right>>
    : mpl::plus<typename mpl::plus<tree_size<Left>, tree_size<Right>>::type, number<1>>
{};

static_assert(tree_size<tree<int, int, int>>::value == 3, "");
static_assert(tree_size<tree<int, tree<int, int, int>, int>>::value == 5, "");
