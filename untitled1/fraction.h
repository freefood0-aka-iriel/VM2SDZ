#pragma once
#include <string>

class Fraction {
private:
    int numerator;
    int denominator;
public:
    Fraction(int num, int den);
    Fraction operator+(const Fraction&) const;
    Fraction operator-(const Fraction&) const;
    Fraction operator*(const Fraction&) const;
    Fraction operator/(const Fraction&) const;
    bool operator<(const Fraction&) const;
    bool operator>(const Fraction&) const;
    bool operator==(const Fraction&) const;
    bool isZero() const;
    std::string toMixedFraction() const;
    inline double toDouble() const {
        return static_cast<double>(numerator) / denominator;
    }
    inline float tofloat() const {
        return static_cast<float>(numerator) / denominator;
    }
    static const Fraction zero;
};
