#pragma once

#include <cmath>
#include <unordered_map>
#include <functional>

namespace easing {

inline double easeLinear(double t) {
    return t;
}

// Sine
inline double easeInSine(double t) {
    return 1.0 - std::cos((t * M_PI) / 2.0);
}
inline double easeOutSine(double t) {
    return std::sin((t * M_PI) / 2.0);
}
inline double easeInOutSine(double t) {
    return -0.5 * (std::cos(M_PI * t) - 1.0);
}

// Quad
inline double easeInQuad(double t) {
    return t * t;
}
inline double easeOutQuad(double t) {
    return t * (2.0 - t);
}
inline double easeInOutQuad(double t) {
    t *= 2.0;
    if (t < 1.0) return 0.5 * t * t;
    t -= 1.0;
    return -0.5 * (t * (t - 2.0) - 1.0);
}

// Cubic
inline double easeInCubic(double t) {
    return t * t * t;
}
inline double easeOutCubic(double t) {
    t -= 1.0;
    return t * t * t + 1.0;
}
inline double easeInOutCubic(double t) {
    t *= 2.0;
    if (t < 1.0) return 0.5 * t * t * t;
    t -= 2.0;
    return 0.5 * (t * t * t + 2.0);
}

// Quart
inline double easeInQuart(double t) {
    return t * t * t * t;
}
inline double easeOutQuart(double t) {
    t -= 1.0;
    return 1.0 - t * t * t * t;
}
inline double easeInOutQuart(double t) {
    t *= 2.0;
    if (t < 1.0) return 0.5 * t * t * t * t;
    t -= 2.0;
    return -0.5 * (t * t * t * t - 2.0);
}

// Quint
inline double easeInQuint(double t) {
    return t * t * t * t * t;
}
inline double easeOutQuint(double t) {
    t -= 1.0;
    return t * t * t * t * t + 1.0;
}
inline double easeInOutQuint(double t) {
    t *= 2.0;
    if (t < 1.0) return 0.5 * t * t * t * t * t;
    t -= 2.0;
    return 0.5 * (t * t * t * t * t + 2.0);
}

// Exponential
inline double easeInExpo(double t) {
    return (t == 0.0) ? 0.0 : std::pow(2.0, 10.0 * (t - 1.0));
}
inline double easeOutExpo(double t) {
    return (t == 1.0) ? 1.0 : 1.0 - std::pow(2.0, -10.0 * t);
}
inline double easeInOutExpo(double t) {
    if (t == 0.0) return 0.0;
    if (t == 1.0) return 1.0;
    t *= 2.0;
    if (t < 1.0) return 0.5 * std::pow(2.0, 10.0 * (t - 1.0));
    return 0.5 * (2.0 - std::pow(2.0, -10.0 * (t - 1.0)));
}

// Circular
inline double easeInCirc(double t) {
    return 1.0 - std::sqrt(1.0 - t * t);
}
inline double easeOutCirc(double t) {
    t -= 1.0;
    return std::sqrt(1.0 - t * t);
}
inline double easeInOutCirc(double t) {
    t *= 2.0;
    if (t < 1.0) return -0.5 * (std::sqrt(1.0 - t * t) - 1.0);
    t -= 2.0;
    return 0.5 * (std::sqrt(1.0 - t * t) + 1.0);
}

// Back
inline double easeInBack(double t) {
    const double s = 1.70158;
    return t * t * ((s + 1.0) * t - s);
}
inline double easeOutBack(double t) {
    const double s = 1.70158;
    t -= 1.0;
    return t * t * ((s + 1.0) * t + s) + 1.0;
}
inline double easeInOutBack(double t) {
    const double s = 1.70158 * 1.525;
    t *= 2.0;
    if (t < 1.0) return 0.5 * (t * t * ((s + 1.0) * t - s));
    t -= 2.0;
    return 0.5 * (t * t * ((s + 1.0) * t + s) + 2.0);
}

// Elastic
inline double easeInElastic(double t) {
    if (t == 0.0) return 0.0;
    if (t == 1.0) return 1.0;
    const double p = 0.3;
    const double s = p / 4.0;
    t -= 1.0;
    return -std::pow(2.0, 10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p);
}
inline double easeOutElastic(double t) {
    if (t == 0.0) return 0.0;
    if (t == 1.0) return 1.0;
    const double p = 0.3;
    const double s = p / 4.0;
    return std::pow(2.0, -10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p) + 1.0;
}
inline double easeInOutElastic(double t) {
    if (t == 0.0) return 0.0;
    if (t == 1.0) return 1.0;
    const double p = 0.45;
    const double s = p / 4.0;
    t = t * 2.0 - 1.0;
    if (t < 0.0)
        return -0.5 * std::pow(2.0, 10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p);
    return 0.5 * std::pow(2.0, -10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p) + 1.0;
}

// Bounce
inline double easeOutBounce(double t) {
    if (t < 1.0 / 2.75) {
        return 7.5625 * t * t;
    } else if (t < 2.0 / 2.75) {
        t -= 1.5 / 2.75;
        return 7.5625 * t * t + 0.75;
    } else if (t < 2.5 / 2.75) {
        t -= 2.25 / 2.75;
        return 7.5625 * t * t + 0.9375;
    } else {
        t -= 2.625 / 2.75;
        return 7.5625 * t * t + 0.984375;
    }
}
inline double easeInBounce(double t) {
    return 1.0 - easeOutBounce(1.0 - t);
}
inline double easeInOutBounce(double t) {
    if (t < 0.5)
        return 0.5 * easeInBounce(t * 2.0);
    return 0.5 * easeOutBounce(t * 2.0 - 1.0) + 0.5;
}

const std::unordered_map<int, std::function<double(double)>> easingFunctions = {
        {0, easeLinear},
        {1, easeInSine},
        {2, easeOutSine},
        {3, easeInOutSine},
        {4, easeInQuad},
        {5, easeOutQuad},
        {6, easeInOutQuad},
        {7, easeInCubic},
        {8, easeOutCubic},
        {9, easeInOutCubic},
        {10, easeInQuart},
        {11, easeOutQuart},
        {12, easeInOutQuart},
        {13, easeInQuint},
        {14, easeOutQuint},
        {15, easeInOutQuint},
        {16, easeInExpo},
        {17, easeOutExpo},
        {18, easeInOutExpo},
        {19, easeInCirc},
        {20, easeOutCirc},
        {21, easeInOutCirc},
        {22, easeInBack},
        {23, easeOutBack},
        {24, easeInOutBack},
        {25, easeInElastic},
        {26, easeOutElastic},
        {27, easeInOutElastic},
        {28, easeInBounce},
        {29, easeOutBounce},
        {30, easeInOutBounce}
    };

} // namespace Easing
