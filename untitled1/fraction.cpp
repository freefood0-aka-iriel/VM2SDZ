#include <string>
#include <stdexcept>
#include <cmath>
#include <numeric>
#include "fraction.h"
using namespace std;

const Fraction Fraction::zero(0,1);

Fraction::Fraction(int num, int den) {
    if (den == 0) {
        throw "Division by zero";
    }
    if (den < 0) {
        num = -num;
        den = -den;
    }
    int common = gcd(num, den);
    numerator = num / common;
    denominator = den / common;
}

Fraction Fraction::operator+(const Fraction& other) const {
    int new_num = numerator * other.denominator + other.numerator * denominator;
    int new_den = denominator * other.denominator;
    return Fraction(new_num, new_den);
}

Fraction Fraction::operator-(const Fraction& other) const {
    int new_num = numerator * other.denominator - other.numerator * denominator;
    int new_den = denominator * other.denominator;
    return Fraction(new_num, new_den);
}

Fraction Fraction::operator*(const Fraction& other) const {
    int new_num = numerator * other.numerator;
    int new_den = denominator * other.denominator;
    return Fraction(new_num, new_den);
}

Fraction Fraction::operator/(const Fraction& other) const {
    if (other.numerator == 0) {
        throw "Division by zero";
    }
    int new_num = numerator * other.denominator;
    int new_den = denominator * other.numerator;
    return Fraction(new_num, new_den);
}

bool Fraction::operator<(const Fraction& other) const {
    return tofloat() < other.tofloat();
}

bool Fraction::operator>(const Fraction& other) const {
    return tofloat() > other.tofloat();
}

bool Fraction::operator==(const Fraction& other) const {
    return numerator*other.denominator == other.numerator*denominator;
}

bool Fraction::isZero() const {
    return numerator == 0;
}

string Fraction::toMixedFraction() const {
    if (numerator == 0) return "0,0,1";

    int integer = numerator / denominator;
    int remainder = numerator % denominator;
    if (remainder < 0) {
        integer -= 1;
        remainder += denominator;
    }

    string result = to_string(integer);
    result += "," + to_string(abs(remainder)) + "," + to_string(denominator);

    return result;
}
