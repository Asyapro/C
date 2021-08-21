#include <iosfwd>

class Rational {
 public:
    Rational() {}
    explicit Rational(const int numerator);
    Rational(const int numerator, const int denominator);
    bool operator==(const Rational& rhs) const { return (num * rhs.den == rhs.den * num); }
    bool operator!=(const Rational& rhs) const { return !operator==(rhs); }
    bool operator>(const Rational& rhs) const { return (num * rhs.den - rhs.den * num > 0); }
    bool operator<(const Rational& rhs) const { return (num * rhs.den - rhs.den * num < 0); }
    bool operator>=(const Rational& rhs) const { return (num * rhs.den - rhs.den * num >= 0); }
    bool operator<=(const Rational& rhs) const { return (num * rhs.den - rhs.den * num <= 0); }
    Rational& operator=(const Rational& rhs) = default;
    Rational& operator+=(const Rational& rhs);
    Rational& operator+=(const int rhs) { return operator+=(Rational(rhs)); }
    Rational& operator-=(const Rational& rhs);
    Rational& operator-=(const int rhs) { return operator-=(Rational(rhs)); }
    Rational& operator*=(const Rational& rhs);
    Rational& operator*=(const int rhs) { return operator*=(Rational(rhs)); }
    Rational& operator/=(const Rational& rhs);
    Rational& operator/=(const int rhs) { return operator/=(Rational(rhs)); }

    void normal();

    std::ostream& writeTo(std::ostream& ostrm);
    std::istream& readFrom(std::istream& istrm);

    static const char seperator{ '/' };

    int num{ 0 };
    int den{ 1 };
};

Rational operator+(const Rational& lhs, const Rational& rhs);
Rational operator-(const Rational& lhs, const Rational& rhs);
Rational operator*(const Rational& lhs, const Rational& rhs);
Rational operator/(const Rational& lhs, const Rational& rhs);

Rational operator+(const Rational& lhs, const int& rhs);
Rational operator-(const Rational& lhs, const int& rhs);
Rational operator*(const Rational& lhs, const int& rhs);
Rational operator/(const Rational& lhs, const int& rhs);

Rational operator+(const int& lhs, const  Rational& rhs);
Rational operator-(const int& lhs, const Rational& rhs);
Rational operator*(const int& lhs, const Rational& rhs);
Rational operator/(const int& lhs, const Rational& rhs);

inline std::ostream& operator<<(std::ostream& ostrm, Rational& rhs) {
    return rhs.writeTo(ostrm);
}
inline std::istream& operator>>(std::istream& istrm, Rational& rhs) {
    return rhs.readFrom(istrm);
}
