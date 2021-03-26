/// Efficiency issue: if F or G are empty classes, by the C++ standard, they
/// should still occupy at least one byte of space. So, in the example down
/// there, where we use empty std::negate<T> the compose_fg::f will be 1 byte,
/// then maybe 3 byets for alignment for function pointer and then 8 bytes for
/// the pointer itself. Total 12 bytes. If F would not to be allocated, the
/// size could be only 8 bytes.
///
/// We'll use emply structure selection and empty base optimization (EBO).
/// EBO allows the address of an empty base subobject to be the same as any
/// other subobject, given no other subobjects of *the same type* share
/// the same address.
///
/// Structure selection is a struct that encapsulates the variable part of
/// our type. Here, it will be storage.

#include <type_traits>

template <class F, bool F_empty, class G, bool G_empty>
struct storage;

template <class R, class F, class G>
class compose_fg
    : storage<
        F, std::is_empty<F>::value,
        G, std::is_empty<G>::value
    >
{
    using base =
        storage<
            F, std::is_empty<F>::value,
            G, std::is_empty<G>::value
        >;

public:
    compose_fg(F const& f, G const& g)
        : base(f, g)
    {}

    template <class T>
    R operator()(T const& x) const
    {
        F const& f = this->get_f();
        G const& g = this->get_g();

        return f(g(x));
    }
};

template <class F, class G>
struct storage<F, false, G, false>
{
protected:

    storage(F const& f, G const& g)
        : f(f), g(g)
    {}

    F const& get_f() const
    {
        return f;
    }

    G const& get_g() const
    {
        return g;
    }

private:
    F f;
    G g;
};

template <class F, class G>
struct storage<F, true, G, false> : private F
{
protected:

    storage(F const&, G const& g)
        : g(g)
    {}

    F const& get_f() const
    {
        return *this;
    }

    G const& get_g() const
    {
        return g;
    }

private:
    G g;
};

template <class F, class G>
struct storage<F, false, G, true> : private G
{
protected:

    storage(F const& f, G const&)
        : f(f)
    {}

    F const& get_f() const
    {
        return f;
    }

    G const& get_g() const
    {
        return *this;
    }

private:
    F f;
};

template <class F, class G>
struct storage<F, true, G, true> : private F, private G
{
protected:

    storage(F const&, G const&)
    {}

    F const& get_f() const
    {
        return *this;
    }

    G const& get_g() const
    {
        return *this;
    }
};

#include <functional>
#include <algorithm>
#include <cmath>
#include <iostream>

float input[5] = {0.0, 0.1, 0.2, 0.3, 0.4};
float output[5];

float sin_squared(float x)
{
    return std::pow(std::sin(x), 2);
}

/// --- Can't deduce types --- (althoug maybe CTAD can help)
/// From std reference:
/// std::negate<T> is Unary function object class whose call returns the result of negating its argument (as returned by the unary operator -).
[[maybe_unused]]
float* ignored = std::transform(
    input, input+5, output,
    compose_fg<float, std::negate<float>, float(*)(float)>( //< WTF?
        std::negate<float>(), &sin_squared
        ));

/// Solution: object generator -- a function that will be able to deduce
/// types.
template <class R, class F, class G>
compose_fg<R, F, G> compose(F const& f, G const& g)
{
    return compose_fg<R, F, G>(f, g);
}

/// Much better!
[[maybe_unused]]
float* ignored2 = std::transform(
    input, input+5, output,
    compose<float>(std::negate<float>(), &sin_squared));

int main()
{
    std::cout << "(1) &sin_squared: " << sizeof &sin_squared << '\n';
    std::cout << "(2) std::negate<float>: " << sizeof std::negate<float>() << '\n';
    std::cout << "1, 1: " << sizeof compose<float>(&sin_squared, &sin_squared) << '\n';
    std::cout << "2, 1: " << sizeof compose<float>(std::negate<float>(), &sin_squared) << '\n';
    std::cout << "1, 2: " << sizeof compose<float>(&sin_squared, std::negate<float>()) << '\n';

    /// Corner case if F and G are of same type
    std::cout << "2, 2: " << sizeof compose<float>(std::multiplies<float>(), std::negate<float>()) << '\n';
    return 0;
}
