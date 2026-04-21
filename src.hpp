#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <stdexcept>
#include <algorithm>

struct divided_by_zero final : std::exception {
public:
    divided_by_zero()   = default;
    ~divided_by_zero()  = default;
    const char *what() const noexcept override { return "Divided by zero!"; };
};

template <typename _Tp>
inline _Tp gcd(_Tp x, _Tp y) {
    while (y) {
        x %= y;
        std::swap(x, y);
    } return x;
}

template <typename _Tp>
inline _Tp fast_pow(_Tp x, unsigned long long y) {
    _Tp res = 1;
    while (y) {
        if (y & 1) res *= x;
        x *= x;
        y >>= 1;
    } return res;
}

template <typename _Tp>
struct fraction {
    _Tp num, den;

    void normalize() {
        if (den == 0) throw divided_by_zero();
        if (den < 0) {
            num = -num;
            den = -den;
        }
        if (num == 0) {
            den = 1;
        } else {
            _Tp a = num < 0 ? -num : num;
            _Tp b = den;
            _Tp common = gcd(a, b);
            num /= common;
            den /= common;
        }
    }

public:
    fraction() : num(0), den(1) {}
    fraction(_Tp x) : num(x), den(1) {}
    fraction(_Tp x, _Tp y) : num(x), den(y) { normalize(); }

    fraction operator + (const fraction &other) const {
        _Tp g = gcd(den, other.den);
        _Tp res_num = num * (other.den / g) + other.num * (den / g);
        _Tp res_den = (den / g) * other.den;
        return fraction(res_num, res_den);
    }
    fraction operator - (const fraction &other) const {
        _Tp g = gcd(den, other.den);
        _Tp res_num = num * (other.den / g) - other.num * (den / g);
        _Tp res_den = (den / g) * other.den;
        return fraction(res_num, res_den);
    }
    fraction operator * (const fraction &other) const {
        _Tp g1 = gcd(num < 0 ? -num : num, other.den);
        _Tp g2 = gcd(other.num < 0 ? -other.num : other.num, den);
        return fraction((num / g1) * (other.num / g2), (den / g2) * (other.den / g1));
    }
    fraction operator / (const fraction &other) const {
        if (other.num == 0) throw divided_by_zero();
        _Tp g1 = gcd(num < 0 ? -num : num, other.num < 0 ? -other.num : other.num);
        _Tp g2 = gcd(den, other.den);
        return fraction((num / g1) * (other.den / g2), (den / g2) * (other.num / g1));
    }
    fraction operator ^ (long long exp) const {
        if (exp == 0) return fraction(1);
        if (num == 0) {
            if (exp < 0) throw divided_by_zero();
            return fraction(0);
        }
        if (exp > 0) {
            return fraction(fast_pow(num, (unsigned long long)exp), fast_pow(den, (unsigned long long)exp));
        } else {
            unsigned long long uexp = -static_cast<unsigned long long>(exp);
            return fraction(fast_pow(den, uexp), fast_pow(num, uexp));
        }
    }

    fraction &operator += (const fraction &other) {
        *this = *this + other;
        return *this;
    }
    fraction &operator -= (const fraction &other) {
        *this = *this - other;
        return *this;
    }
    fraction &operator *= (const fraction &other) {
        *this = *this * other;
        return *this;
    }
    fraction &operator /= (const fraction &other) {
        *this = *this / other;
        return *this;
    }
    fraction &operator ^= (long long exp) {
        *this = *this ^ exp;
        return *this;
    }

    explicit operator double() const {
        return static_cast<double>(num) / static_cast<double>(den);
    }
    explicit operator bool() const {
        return num != 0;
    }

    friend bool operator == (const fraction &a, const fraction &b) {
        return a.num == b.num && a.den == b.den;
    }
    friend bool operator < (const fraction &a, const fraction &b) {
        // Since den > 0, a/b < c/d <=> ad < bc
        // We use the same guarantee that ad and bc are safe.
        return a.num * b.den < b.num * a.den;
    }

    friend std::ostream & operator << (std::ostream &os, const fraction &f) {
        os << f.num << "/" << f.den;
        return os;
    }
};

#endif