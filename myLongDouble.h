#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class myLongDouble {
    const size_t divDigits = 1000;
    const size_t sqrtDigits = 100;

    int sign;
    std::vector<int> digits;
    long exponent;

    void initFromString(const std::string& s);
    void removeZeroes();
    void normalize();

public:
    myLongDouble();
    myLongDouble(const myLongDouble& x);
    myLongDouble(long double value);
    myLongDouble(const std::string& s);

    myLongDouble& operator=(const myLongDouble& x);

    bool operator>(const myLongDouble& x) const;
    bool operator<(const myLongDouble& x) const;
    bool operator>=(const myLongDouble& x) const;
    bool operator<=(const myLongDouble& x) const;
    bool operator==(const myLongDouble& x) const;
    bool operator!=(const myLongDouble& x) const;

    myLongDouble operator-() const;

    myLongDouble operator+(const myLongDouble& x) const;
    myLongDouble operator-(const myLongDouble& x) const;
    myLongDouble operator*(const myLongDouble& x) const;
    myLongDouble operator/(const myLongDouble& x) const;

    myLongDouble& operator+=(const myLongDouble& x);
    myLongDouble& operator-=(const myLongDouble& x);
    myLongDouble& operator*=(const myLongDouble& x);
    myLongDouble& operator/=(const myLongDouble& x);

    myLongDouble operator++(int);
    myLongDouble operator--(int);

    myLongDouble& operator++();
    myLongDouble& operator--();

    myLongDouble inverse() const;
    myLongDouble sqrt() const;
    myLongDouble pow(const myLongDouble &n) const;
    myLongDouble abs() const;

    bool isInteger() const;
    bool isEven() const;
    bool isOdd() const;
    bool isZero() const;

    friend std::ostream& operator<<(std::ostream& os, const myLongDouble& value);
};