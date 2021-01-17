#include "units.hpp"

int main()
{
    units::quantity<float, units::mass> m{2.0f};
    units::quantity<float, units::acceleration> a{10.0f};

    units::quantity<float, units::force> f1 = m * a;
    auto f2 = f1 + m * a;
}
