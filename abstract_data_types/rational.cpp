#include "rational.h"
#include <iostream>
#include <sstream>

int gcd(int a, int b) {
    int temp;
    while (b) {
        a %= b;
        temp = a;
        a = b;
        b = temp;
    }
    return a;
}

Rational::Rational(const int numerator): Rational(numerator, 1) { }

Rational::Rational(const int numerator, const int denominator)
    : num(numerator), den(denominator) {
    normal();
}

Rational& Rational::operator+=(const Rational& rhs) {
    int temp = gcd(den, rhs.den);
    num = num * (rhs.den / temp) + rhs.num * (den / temp);
    den = den * rhs.den / temp;
    normal();
    return *this;
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res += rhs;
    return res;
}

Rational& Rational::operator-=(const Rational& rhs) {
    int temp = gcd(den, rhs.den);
    num = num * (rhs.den / temp) - rhs.num * (den / temp);
    den = den * rhs.den / temp;
    normal();
    return *this;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res -= rhs;
    return res;
}

Rational& Rational::operator*=(const Rational& rhs) {
    num = num * rhs.num;
    den = den * rhs.den;
    normal();
    return *this;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res *= rhs;
    return res;
}

Rational& Rational::operator/=(const Rational& rhs) {
    if (rhs.num == 0) {
        throw std::runtime_error("null division");
    }
    num *= rhs.den;
    normal();
    den *= rhs.num;
    normal();
    return *this;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational res = lhs;
    res /= rhs;
    return res;
}

Rational operator+(const Rational& lhs, const int& rhs) {
    return (lhs + Rational(rhs));
}
Rational operator-(const Rational& lhs, const int& rhs) {
    return (lhs - Rational(rhs));
}
Rational operator*(const Rational& lhs, const int& rhs) {
    return (lhs * Rational(rhs));
}
Rational operator/(const Rational& lhs, const int& rhs) {
    return (lhs / Rational(rhs));
}

Rational operator+(const int& lhs, const Rational& rhs) {
    return (Rational(lhs) + rhs);
}
Rational operator-(const int& lhs, const Rational& rhs) {
    return (Rational(lhs) - rhs);
}
Rational operator*(const int& lhs, const Rational& rhs) {
    return (Rational(lhs) * rhs);
}
Rational operator/(const int& lhs, const Rational& rhs) {
    return (Rational(lhs) / rhs);
}

void Rational::normal() {
    if (den == 0) { num = 0; }
    if (!(num * den)) { den = 1; }
    if (den < 0) {
        den *= -1;
        num *= -1;
    }
    int mult = gcd(den, abs(num));
    den /= mult;
    num /= mult;
}

std::ostream& Rational::writeTo(std::ostream& ostrm) {
    if (num != 0) {
        ostrm << num << seperator << den;
    } else {
        ostrm << 0;
    }
    return ostrm;
}

std::istream& Rational::readFrom(std::istream& istrm) {
    int numerator(0);
    char comma(0);
    int denumerator(1);
    istrm >> numerator >> comma >> denumerator;
    if (Rational::seperator == seperator) {
        num = numerator;
        den = denumerator;
        normal();
    } else if (istrm.good()) {
        istrm.setstate(std::ios_base::failbit);
    }
    return istrm;
}
