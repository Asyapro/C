
#include <iosfwd>

class Complex {
 public:
    Complex() {}
    explicit Complex(const double real);
    Complex(const double real, const double imaginary);
    bool operator==(const Complex& rhs) const {
        return (fabs(re - rhs.re) < 1e-9) && (fabs(im - rhs.im) < 1e-9);
    }
    bool operator!=(const Complex& rhs) const { return !operator==(rhs); }
    Complex& operator=(const Complex& rhs) = default;
    Complex& operator+=(const Complex& rhs);
    Complex& operator+=(const double rhs) { return operator+=(Complex(rhs)); }
    Complex& operator-=(const Complex& rhs);
    Complex& operator-=(const double rhs) { return operator-=(Complex(rhs)); }
    Complex& operator*=(const Complex& rhs);
    Complex& operator*=(const double rhs) { return operator*=(Complex(rhs)); }
    Complex& operator/=(const Complex& rhs);
    Complex& operator/=(const double rhs) { return operator/=(Complex(rhs)); }

    std::ostream& writeTo(std::ostream& ostrm);
    std::istream& readFrom(std::istream& istrm);

    double re{ 0.0 };
    double im{ 0.0 };

    static const char leftBrace{ '{' };
    static const char seperator{ ',' };
    static const char rightBrace{ '}' };
};

Complex operator+(const Complex& lhs, const Complex& rhs);
Complex operator-(const Complex& lhs, const Complex& rhs);
Complex operator*(const Complex& lhs, const Complex& rhs);
Complex operator/(const Complex& lhs, const Complex& rhs);

Complex operator+(const Complex& lhs, const double& rhs);
Complex operator-(const Complex& lhs, const double& rhs);
Complex operator*(const Complex& lhs, const double& rhs);
Complex operator/(const Complex& lhs, const double& rhs);

Complex operator+(const double& lhs, const Complex& rhs);
Complex operator-(const double& lhs, const Complex& rhs);
Complex operator*(const double& lhs, const Complex& rhs);
Complex operator/(const double& lhs, const Complex& rhs);

inline std::ostream& operator<<(std::ostream& ostrm, Complex& rhs) {
    return rhs.writeTo(ostrm);
}
inline std::istream& operator>>(std::istream& istrm, Complex& rhs) {
    return rhs.readFrom(istrm);
}

