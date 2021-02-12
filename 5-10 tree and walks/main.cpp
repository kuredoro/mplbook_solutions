#include "tree.h"
#include "total_order.h"

#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>

/// n_order_iterator is equivalent to total_order_iterator, except that it
/// skips the total_order_iterators whose VisitCound doesn't equal to Order.
template <class Order, class It>
struct n_order_iterator;

template <class Order>
struct n_order_iterator<Order, total_order_end_iterator>
{
    using category = mpl::forward_iterator_tag;
    using type = n_order_iterator<Order, total_order_end_iterator>;
};

/// If orders agree, then stop the inheritance recursion.
template <class Order, class ...Stack>
struct n_order_iterator<Order, total_order_iterator<Order, Stack...>>
{
    using category = mpl::forward_iterator_tag;
    using type = n_order_iterator<Order, total_order_iterator<Order, Stack...>>;
};

/// If orders differ, skip to the next iterator.
template <class Order, class ItOrder, class ...Stack>
struct n_order_iterator<Order, total_order_iterator<ItOrder, Stack...>>
    : n_order_iterator<
        Order, 
        typename mpl::next<
            total_order_iterator<ItOrder, Stack...>
        >::type
    >
{};

// -----------------------------------------------------------------------------

template <class Order, class It>
struct mpl::next<n_order_iterator<Order, It>>
    : n_order_iterator<Order, typename mpl::next<It>::type>
{};

template <class Order, class It>
struct mpl::deref<n_order_iterator<Order, It>>
{
    using type = typename mpl::deref<It>::type;
};

// -----------------------------------------------------------------------------

struct n_order_view_tag{};

template <class Order, class Tree>
struct n_order_view
{
    using tag = n_order_view_tag;
    using type = n_order_view<Order, Tree>;
    using order = Order;
    using tree = Tree;
};

template <>
struct mpl::begin_impl<n_order_view_tag>
{
    template <class View>
    struct apply
        : n_order_iterator<typename View::order, typename mpl::begin<total_order_view<typename View::tree>>::type>
    {};
};

template <>
struct mpl::end_impl<n_order_view_tag>
{
    template <class View>
    struct apply
        : n_order_iterator<typename View::order, total_order_end_iterator>
    {};
};

// -----------------------------------------------------------------------------

template <class Tree>
using preorder_view = n_order_view<number<1>, Tree>;

template <class Tree>
using inorder_view = n_order_view<number<2>, Tree>;

template <class Tree>
using postorder_view = n_order_view<number<3>, Tree>;

// -----------------------------------------------------------------------------

using treeSeq =
    tree<
        double,
        tree<void*, int, long>,
        char
    >;

static_assert(
    mpl::equal<
        preorder_view<treeSeq>,
        mpl::vector<double, void*, int, long, char>
    >(),
    ""
);

static_assert(
    mpl::equal<
        inorder_view<treeSeq>,
        mpl::vector<int, void*, long, double, char>
    >(),
    ""
);

static_assert(
    mpl::equal<
        postorder_view<treeSeq>,
        mpl::vector<int, long, void*, char, double>
    >(),
    ""
);

int main()
{
}
