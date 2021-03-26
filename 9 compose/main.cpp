template <class R, class F, class G>
class compose_fg
{
public:
    compose_fg(F const& f, G const& g)
        : f(f), g(g)
    {}

    template <class T>
    R operator()(T const& x) const
    {
        return f(g(x));
    }

private:
    F f;
    G g;
};

#include <functional>
#include <algorithm>
#include <cmath>

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
    return 0;
}
