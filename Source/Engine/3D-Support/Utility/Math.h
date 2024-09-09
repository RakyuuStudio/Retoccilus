#include <limits>

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename Tp>
    inline Tp gcd(Tp a, Tp b) {
        while (true) {
            if (a == static_cast<Tp>(0)) {
                return b;
            }
            b %= a;

            if (b == static_cast<Tp>(0)) {
                return a;
            }
            a %= b;
        }
    }

    template<typename Tp>
    inline Tp lcm(Tp a, Tp b) {
        const auto c = gcd(a, b);
        if (!c) {
            return c;
        }

        return a / c * b;
    }

    template<typename Tp>
    inline Tp eps() {
        return std::numeric_limits<Tp>::epsilon();
    }

    template<typename Tp>
    inline Tp rPi() {
        return static_cast<Tp>(3.14159265358979323846);
    }
}