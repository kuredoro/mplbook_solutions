#pragma once

#include <boost/mpl/int_fwd.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/transform.hpp>

namespace mpl = boost::mpl;

#include <boost/mpl/placeholders.hpp>
using namespace mpl::placeholders;

namespace units
{

typedef mpl::vector_c<int,0,0,0,0,0,0,0> scalar;

typedef mpl::vector_c<int,1,0,0,0,0,0,0> mass;          // kilogram
typedef mpl::vector_c<int,0,1,0,0,0,0,0> length;        // meters
typedef mpl::vector_c<int,0,0,1,0,0,0,0> time;          // seconds
typedef mpl::vector_c<int,0,0,0,1,0,0,0> charge;        // ampere
typedef mpl::vector_c<int,0,0,0,0,1,0,0> temperature;   // kelvin
typedef mpl::vector_c<int,0,0,0,0,0,1,0> intensity;     // candela
typedef mpl::vector_c<int,0,0,0,0,0,0,1> substance;     // mole

// base dimension:        m l  t ...
typedef mpl::vector_c<int,0,1,-1,0,0,0,0> velocity;
typedef mpl::vector_c<int,0,1,-2,0,0,0,0> acceleration;
typedef mpl::vector_c<int,1,1,-1,0,0,0,0> momentum;
typedef mpl::vector_c<int,1,1,-2,0,0,0,0> force;

// An integral wrapper type, parametrized on the dimension unit
template <class T, class Dimension>
struct quantity
{
    explicit quantity(T x) : m_value(x)
    {}

    template <class OtherDimension>
    quantity(quantity<T, OtherDimension> const& rhs) : m_value(rhs.value())
    {
        static_assert(mpl::equal<Dimension, OtherDimension>::type::value, "trying to assign a value of different dimension");
    }

    T value() const
    {
        return m_value;
    }

private:
    T m_value;
};

template <class T, class D1, class D2>
auto operator+(const quantity<T, D1>& x, const quantity<T, D2>& y)
{
    static_assert(mpl::equal<D1, D2>::value, "trying to sum quantities of different dimensions");
    return quantity<T, D1>{x.value() + y.value()};
}

template <class T, class D1, class D2>
auto operator-(const quantity<T, D1>& x, const quantity<T, D2>& y)
{
    static_assert(mpl::equal<D1, D2>::value, "trying to subtract quantities of different dimensions");
    return quantity<T, D1>{x.value() - y.value()};
}

// Transformations using a metafunction class
struct plus_f
{
    template <class A, class B>
    struct apply 
        : mpl::plus<A, B> {};
};

template <class T, class D1, class D2>
auto operator*(const quantity<T, D1>& x, const quantity<T, D2>& y)
{
    typedef typename mpl::transform<D1, D2, plus_f>::type dim;
    return quantity<T, dim>{x.value() * y.value()};
}

// Transform using placeholder expression
template <class D1, class D2>
struct DivideDimensions
    : mpl::transform<D1, D2, mpl::minus<_1, _2>> {};

template <class T, class D1, class D2>
auto operator/(const quantity<T, D1>& x, const quantity<T, D2>& y)
{
    typedef typename DivideDimensions<D1, D2>::type dim;
    return quantity<T, dim>{x.value() / y.value()};
}

} // namespace units
