#include "myLongDouble.h"

void myLongDouble::initFromString(const std::string& s)
{
    size_t index;

    if (s[0] == '-')
    {
        sign = -1;
        index = 1;
    }
    else
    {
        sign = 1;
        index = 0;
    }

    exponent = s.length() - index;

    while (index < s.length())
    {
        if (s[index] == '.')
        {
            exponent = sign == 1 ? index : index - 1;
        }
        else
        {
            digits.push_back(s[index] - '0');
        }

        index++;
    }
}

void myLongDouble::removeZeroes()
{
    size_t n = std::max((long) 1, exponent);

    while (digits.size() > n && digits[digits.size() - 1] == 0)
    {
        digits.erase(digits.end() - 1);
    }

    while (digits.size() > 1 && digits[0] == 0)
    {
        digits.erase(digits.begin());
        exponent--;
    }

    while (digits.size() > 1 && digits[digits.size() - 1] == 0)
    {
        digits.erase(digits.end() - 1);
    }

    if (isZero())
    {
        exponent = 1;
        sign = 1;
    }

    normalize();
}

void myLongDouble::normalize()
{
    size_t start = std::max(exponent, (long) 0);
    size_t realDigits = digits.size() - start;

    if (realDigits >= divDigits)
    {
        size_t count = 0;
        size_t maxCount = 0;

        size_t i = start;

        while(i < digits.size())
        {
            count = 0;

            while (i < digits.size() && digits[i] == 9)
            {
                count++;
                i++;
            }

            if (count > maxCount)
                maxCount = count;

            i++;
        }

        if (maxCount > divDigits * 4 / 5)
        {
            i = digits.size() - 1;

            do {
                count = 0;

                while (i > 0 && digits[i] != 9)
                    i--;

                while (i > 0 && digits[i] == 9) {
                    count++;
                    i--;
                }
            } while (count != maxCount);

            digits.erase(digits.begin() + i + 1, digits.end());
            digits[i]++;
        }
    }
}

myLongDouble::myLongDouble()
{
    sign = 1;
    digits = std::vector<int>(1, 0);
    exponent = 1;
}

myLongDouble::myLongDouble(const myLongDouble& x)
{
    sign = x.sign;
    exponent = x.exponent;
    digits = std::vector<int>(x.digits.size());

    for (size_t i = 0; i < x.digits.size(); i++)
        digits[i] = x.digits[i];
}

myLongDouble::myLongDouble(long double value)
{
    std::stringstream ss;
    ss << std::setprecision(15) << value;

    initFromString(ss.str());
    removeZeroes();
}

myLongDouble::myLongDouble(const std::string& s)
{
    initFromString(s);
    removeZeroes();
}

myLongDouble& myLongDouble::operator=(const myLongDouble& x)
{
    if (this == &x)
    {
        return *this;
    }

    sign = x.sign;
    exponent = x.exponent;
    digits = std::vector<int>(x.digits.size());

    for (size_t i = 0; i < x.digits.size(); i++)
    {
        digits[i] = x.digits[i];
    }

    return *this;
}

bool myLongDouble::operator>(const myLongDouble& x) const
{
    if (sign != x.sign)
    {
        return sign > x.sign;
    }

    if (exponent != x.exponent)
    {
        return (exponent > x.exponent) ^ (sign == -1);
    }

    std::vector<int> d1(digits);
    std::vector<int> d2(x.digits);
    size_t size = std::max(d1.size(), d2.size());

    while (d1.size() != size)
    {
        d1.push_back(0);
    }

    while (d2.size() != size)
    {
        d2.push_back(0);
    }

    for (size_t i = 0; i < size; i++)
    {
        if (d1[i] != d2[i])
        {
            return (d1[i] > d2[i]) ^ (sign == -1);
        }
    }

    return false;
}

bool myLongDouble::operator<(const myLongDouble& x) const
{
    return !(*this > x || *this == x);
}

bool myLongDouble::operator>=(const myLongDouble& x) const
{
    return *this > x || *this == x;
}

bool myLongDouble::operator<=(const myLongDouble& x) const
{
    return *this < x || *this == x;
}

bool myLongDouble::operator==(const myLongDouble& x) const
{
    if (sign != x.sign)
    {
        return false;
    }

    if (exponent != x.exponent)
    {
        return false;
    }

    if (digits.size() != x.digits.size())
    {
        return false;
    }

    for (size_t i = 0; i < digits.size(); i++)
    {
        if (digits[i] != x.digits[i])
        {
            return false;
        }
    }

    return true;
}

bool myLongDouble::operator!=(const myLongDouble& x) const
{
    return !(*this == x);
}

myLongDouble myLongDouble::operator-() const
{
    myLongDouble res(*this);
    res.sign = -sign;

    return res;
}

myLongDouble myLongDouble::operator+(const myLongDouble& x) const
{
    if (sign == x.sign)
    {
        long exp1 = exponent;
        long exp2 = x.exponent;
        long exp = std::max(exp1, exp2);

        std::vector<int> d1(digits);
        std::vector<int> d2(x.digits);

        while (exp1 != exp)
        {
            d1.insert(d1.begin(), 0);
            exp1++;
        }

        while (exp2 != exp)
        {
            d2.insert(d2.begin(), 0);
            exp2++;
        }

        size_t size = std::max(d1.size(), d2.size());

        while (d1.size() != size)
        {
            d1.push_back(0);
        }

        while (d2.size() != size)
        {
            d2.push_back(0);
        }

        size_t len = 1 + size;

        myLongDouble res;

        res.sign = sign;
        res.digits = std::vector<int>(len, 0);

        for (size_t i = 0; i < size; i++)
        {
            res.digits[i + 1] = d1[i] + d2[i];
        }

        for (size_t i = len - 1; i > 0; i--)
        {
            res.digits[i - 1] += res.digits[i] / 10;
            res.digits[i] %= 10;
        }

        res.exponent = exp + 1;
        res.removeZeroes();

        return res;
    }

    if (sign == -1)
    {
        return x - (-(*this));
    }

    return *this - (-x);
}

myLongDouble myLongDouble::operator-(const myLongDouble& x) const
{
    if (sign == 1 && x.sign == 1)
    {
        bool cmp = *this > x;

        long exp1 = cmp ? exponent : x.exponent;
        long exp2 = cmp ? x.exponent : exponent;
        long exp = std::max(exp1, exp2);

        std::vector<int> d1(cmp ? digits : x.digits);
        std::vector<int> d2(cmp ? x.digits : digits);

        while (exp1 != exp)
        {
            d1.insert(d1.begin(), 0);
            exp1++;
        }

        while (exp2 != exp)
        {
            d2.insert(d2.begin(), 0);
            exp2++;
        }

        size_t size = std::max(d1.size(), d2.size());

        while (d1.size() != size)
        {
            d1.push_back(0);
        }

        while (d2.size() != size)
        {
            d2.push_back(0);
        }

        size_t len = 1 + size;

        myLongDouble res;

        res.sign = cmp ? 1 : -1;
        res.digits = std::vector<int>(len, 0);

        for (size_t i = 0; i < size; i++)
        {
            res.digits[i + 1] = d1[i] - d2[i];
        }

        for (size_t i = len - 1; i > 0; i--)
        {
            if (res.digits[i] < 0)
            {
                res.digits[i] += 10;
                res.digits[i - 1]--;
            }
        }

        res.exponent = exp + 1;
        res.removeZeroes();

        return res;
    }

    if (sign == -1 && x.sign == -1)
    {
        return (-x) - (-(*this));
    }

    return *this + (-x);
}

myLongDouble myLongDouble::operator*(const myLongDouble& x) const
{
    size_t len = digits.size() + x.digits.size();

    myLongDouble res;

    res.sign = sign * x.sign;
    res.digits = std::vector<int>(len, 0);
    res.exponent = exponent + x.exponent;

    for (size_t i = 0; i < digits.size(); i++)
    {
        for (size_t j = 0; j < x.digits.size(); j++)
        {
            res.digits[i + j + 1] += digits[i] * x.digits[j];
        }
    }

    for (size_t i = len - 1; i > 0; i--)
    {
        res.digits[i - 1] += res.digits[i] / 10;
        res.digits[i] %= 10;
    }

    res.removeZeroes();

    return res;
}

myLongDouble myLongDouble::operator/(const myLongDouble& x) const
{
    myLongDouble res = *this * x.inverse();

    size_t intPart = std::max((long)0, exponent);

    if (intPart > res.digits.size() - 1)
    {
        return res;
    }

    size_t i = res.digits.size() - 1 - intPart;
    size_t n = std::max((long) 0, res.exponent);

    if (i > n && res.digits[i] == 9)
    {
        while (i > n && res.digits[i] == 9)
        {
            i--;
        }

        if (res.digits[i] == 9)
        {
            res.digits.erase(res.digits.begin() + n, res.digits.end());
            res = res + res.sign;
        }
        else
        {
            res.digits.erase(res.digits.begin() + i + 1, res.digits.end());
            res.digits[i]++;
        }
    }

    return res;
}

myLongDouble& myLongDouble::operator+=(const myLongDouble& x)
{
    return (*this = *this + x);
}

myLongDouble& myLongDouble::operator-=(const myLongDouble& x)
{
    return (*this = *this - x);
}

myLongDouble& myLongDouble::operator*=(const myLongDouble& x)
{
    return (*this = *this * x);
}

myLongDouble& myLongDouble::operator/=(const myLongDouble& x)
{
    return (*this = *this / x);
}

myLongDouble myLongDouble::operator++(int)
{
    myLongDouble res(*this);
    *this = *this + 1;

    return res;
}

myLongDouble myLongDouble::operator--(int)
{
    myLongDouble res(*this);
    *this = *this - 1;

    return res;
}

myLongDouble& myLongDouble::operator++()
{
    return (*this = *this + 1);
}

myLongDouble& myLongDouble::operator--()
{
    return (*this = *this - 1);
}

myLongDouble myLongDouble::inverse() const
{
    if (isZero())
    {
        throw std::string("myLongDouble myLongDouble::inverse() - division by zero!");
    }

    myLongDouble x(*this);
    x.sign = 1;

    myLongDouble d("1");

    myLongDouble res;
    res.sign = sign;
    res.exponent = 1;
    res.digits = std::vector<int>();

    while (x < 1)
    {
        x.exponent++;
        res.exponent++;
    }

    while (d < x)
    {
        d.exponent++;
    }

    res.exponent -= d.exponent - 1;

    size_t numbers = 0;
    size_t intPart = std::max((long) 0, res.exponent);
    size_t maxNumbers = divDigits + intPart;

    do
    {
        int div = 0;

        while (d >= x)
        {
            div++;
            d -= x;
        }

        d.exponent++;
        d.removeZeroes();

        res.digits.push_back(div);
        numbers++;
    } while (!d.isZero() && numbers < maxNumbers);

    return res;
}

myLongDouble myLongDouble::sqrt() const
{
    if (sign == -1)
    {
        throw std::string("myLongDouble myLongDouble::sqrt() - number is negative");
    }

    if (isZero())
    {
        return 0;
    }

    myLongDouble x0;
    myLongDouble p("0.5");
    myLongDouble xk("0.5");
    myLongDouble eps;
    eps.digits = std::vector<int>(1, 1);
    eps.exponent = 1 - sqrtDigits;

    do
    {
        x0 = xk;
        xk = p * (x0 + *this / x0);
    } while ((x0 - xk).abs() > eps);

    xk.digits.erase(xk.digits.begin() + std::max((long) 0, xk.exponent) + sqrtDigits, xk.digits.end());
    xk.removeZeroes();

    return xk;
}

myLongDouble myLongDouble::pow(const myLongDouble& n) const
{
    if (!n.isInteger())
    {
        throw std::string("myLongDouble myLongDouble::power(const myLongDouble& n) - n is not integer!");
    }

    myLongDouble res("1");
    myLongDouble a = n.sign == 1 ? *this : this->inverse();
    myLongDouble power = n.abs();

    while (power > 0)
    {
        if (power.isOdd())
        {
            res *= a;
        }

        a *= a;
        power /= 2;

        if (!power.isInteger())
        {
            power.digits.erase(power.digits.end() - 1);
        }
    }

    return res;
}

myLongDouble myLongDouble::abs() const
{
    myLongDouble res(*this);
    res.sign = 1;

    return res;
}

bool myLongDouble::isInteger() const
{
    if (exponent < 0)
    {
        return false;
    }

    return digits.size() <= (size_t) exponent;
}

bool myLongDouble::isEven() const
{
    if (!isInteger())
    {
        return false;
    }

    if (digits.size() == (size_t) exponent)
    {
        return digits[digits.size() - 1] % 2 == 0;
    }

    return true;
}

bool myLongDouble::isOdd() const
{
    if (!isInteger())
    {
        return false;
    }

    if (digits.size() == (size_t) exponent)
    {
        return digits[digits.size() - 1] % 2 == 1;
    }

    return false;
}

bool myLongDouble::isZero() const
{
    return digits.size() == 1 && digits[0] == 0;
}

std::ostream& operator<<(std::ostream& os, const myLongDouble& value)
{
    if (value.sign == -1)
    {
        os << '-';
    }

    if (value.exponent > 0)
    {
        size_t i = 0;
        size_t e = value.exponent;

        while (i < value.digits.size() && i < e)
        {
            os << value.digits[i++];
        }

        while (i < e)
        {
            os << "0";
            i++;
        }

        if (i < value.digits.size())
        {
            os << ".";

            while (i < value.digits.size())
            {
                os << value.digits[i++];
            }
        }
    }
    else if (value.exponent == 0)
    {
        os << "0.";

        for (size_t i = 0; i < value.digits.size(); i++)
        {
            os << value.digits[i];
        }
    }
    else
    {
        os << "0.";

        for (long i = 0; i < -value.exponent; i++)
        {
            os << "0";
        }

        for (size_t i = 0; i < value.digits.size(); i++)
        {
            os << value.digits[i];
        }
    }

    return os;
}

int main()
{

}