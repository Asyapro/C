#include "complex.h"
#include <iostream>
#include <sstream>

Complex::Complex(const double real) : Complex(real, 0) { }

Complex::Complex(const double real, const double imaginary)
    : re(real), im(imaginary) { }

Complex& Complex::operator+=(const Complex& rhs) {
    re += rhs.re; im += rhs.im;
    return *this;
}

Complex operator+(const Complex& lhs, const Complex& rhs) {
    Complex res = lhs;
    res += rhs;
    return res;
}

Complex& Complex::operator-=(const Complex& rhs) {
    re -= rhs.re; im -= rhs.im;
    return *this;
}

Complex operator-(const Complex& lhs, const Complex& rhs) {
    Complex res = lhs;
    res -= rhs;
    return res;
}

Complex& Complex::operator*=(const Complex& rhs) {
    double tempre = re * rhs.re - im * rhs.im;
    double tempim = re * rhs.im + rhs.re * im;
    re = tempre;
    im = tempim;
    return *this;
}

Complex operator*(const Complex& lhs, const Complex& rhs) {
    Complex res = lhs;
    res *= rhs;
    return res;
}

Complex& Complex::operator/=(const Complex& rhs) {
    if (rhs.im == 0.0 && rhs.re == 0) {
        throw std::runtime_error("null division");
    }
    double tre = re, tim = im;
    re = (tre * rhs.re + tim * rhs.im) / (rhs.re * rhs.re + rhs.im * rhs.im);
    im = (rhs.re * tim - tre * rhs.im) / (rhs.re * rhs.re + rhs.im * rhs.im);
    return *this;
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
    Complex res = lhs;
    res /= rhs;
    return res;
}

Complex operator+(const Complex& lhs, const double &rhs) {
    return (lhs + Complex(rhs));
}
Complex operator-(const Complex& lhs, const double &rhs) {
    return operator-(lhs, Complex(rhs));
}
Complex operator*(const Complex& lhs, const double &rhs) {
    return operator*(lhs, Complex(rhs));
}
Complex operator/(const Complex& lhs, const double &rhs) {
    return operator/(lhs, Complex(rhs));
}

Complex operator+(const double &rhs, const Complex& lhs) {
    return operator+(lhs, Complex(rhs));
}
Complex operator-(const double &rhs, const Complex& lhs) {
    return operator-(lhs, Complex(rhs));
}
Complex operator*(const double &rhs, const Complex& lhs) {
    return operator*(lhs, Complex(rhs));
}
Complex operator/(const double &rhs, const Complex& lhs) {
    return operator/(lhs, Complex(rhs));
}

std::ostream& Complex::writeTo(std::ostream& ostrm) {
    ostrm << leftBrace << re << seperator << im << rightBrace;
    return ostrm;
}

std::istream& Complex::readFrom(std::istream& istrm) {
    char leftBrace(0);
    double real(0.0);
    char comma(0);
    double imaginary(0.0);
    char rightBrace(0);
    istrm >> leftBrace >> real >> comma >> imaginary >> rightBrace;
    if (istrm.good() && Complex::leftBrace == leftBrace
            && Complex::seperator == seperator
            && Complex::rightBrace == rightBrace) {
        re = real;
        im = imaginary;
    } else if (istrm.good()) {
        istrm.setstate(std::ios_base::failbit);
    }
    return istrm;
}
