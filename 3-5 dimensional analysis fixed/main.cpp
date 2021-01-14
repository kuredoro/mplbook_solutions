#include <iostream>
#include "units.hpp"

int main()
{
    units::quantity<float, units::mass> m{2.0f};
    units::quantity<float, units::acceleration> a{10.0f};

    std::cout << "mass:         " << m.value() << " kilograms\n";
    std::cout << "acceleration: " << a.value() << " m/s^2\n";

    units::quantity<float, units::force> f = m * a;
    units::quantity<float, units::mass> m2 = f / a;

    float roundingError = std::abs((m - m2).value());

    std::cout << "rounding error: " << roundingError << "\n";

    // Faulty example
    f = f + m * a;
    return 0;
}
