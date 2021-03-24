#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/mpl/placeholders.hpp>
using namespace boost::mpl::placeholders;

#include "type_name.hpp"

namespace mpl = boost::mpl;

/// Generalized visitation function object
struct visit_type
{
    template <class Visitor>
    void operator()(Visitor) const
    {
        Visitor::visit();
    }
};

template <class T>
struct print_visitor
{
    static void visit()
    {
        std::cout << type_name<T>() << '\n';
    }
};

int main()
{
    using v = mpl::vector<char, short, int, unsigned, long, long long, int64_t*>;

    /// So now wrap<T> becomes print_visitor and the logic to process this type
    /// is contained in argumentless print_visitor::visit()
    mpl::for_each<v, print_visitor<_>>(visit_type());
}
