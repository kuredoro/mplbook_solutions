#pragma once

#include <boost/mpl/integral_c.hpp>

namespace mpl = boost::mpl;

template <class ...Args>
struct tree;

template <>
struct tree<>
{
    using type = tree;
};

template <class Node, class Left, class Right>
struct tree<Node, Left, Right>
{
    using type = tree;

    using left = Left;
    using right = Right;
    using payload = Node;
};

template<int N>
using number = mpl::integral_c<int, N>;
