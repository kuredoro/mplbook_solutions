/*
 * I don't know how they want me to plug type traits here. A simple
 * overload seems to be the best solution.
 */
#include <boost/type_traits/is_same.hpp>
#include <cassert>

using namespace boost;

template <class Target, class Source>
inline Target polymorphic_downcast(Source* x)
{
    assert(dynamic_cast<Target>(x) == x);
    return static_cast<Target>(x);
}

template <class Target, class Source>
inline Target polymorphic_downcast(Source& x)
{
    assert(&dynamic_cast<Target>(x) == &x);
    return static_cast<Target>(x);
}

struct A 
{
    virtual ~A() = default;
};

struct B : A {};

int main()
{
    B b;

    A* aPtr = &b;
    [[maybe_unused]]
    B* bPtr = polymorphic_downcast<B*>(aPtr);

    A& aRef = b;
    [[maybe_unused]]
    B& bRef = polymorphic_downcast<B&>(aRef);
    return 0;
}
