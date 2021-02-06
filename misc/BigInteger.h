#include <iostream>
#include <vector>
#include <string>

class BigInteger {
public:
    BigInteger();
    BigInteger(int input);
    BigInteger(const std::string& input);

    void dump( std::ostream &os) {
        os << *this;
    }

    explicit operator bool();

    friend BigInteger operator+(const BigInteger& a, const BigInteger& b);
    BigInteger& operator+=(const BigInteger& a);

    friend BigInteger  operator-(const BigInteger& a, const BigInteger& b);
    BigInteger& operator-=(const BigInteger& a);

    friend BigInteger  operator*(const BigInteger& a, const BigInteger& b);
    BigInteger  operator*(int a) const;
    BigInteger& operator*=(const BigInteger& a);

    friend const BigInteger  operator/(const BigInteger& a, const BigInteger& b);
    BigInteger& operator/=(const BigInteger& a);

    friend const BigInteger  operator%(const BigInteger& a, const BigInteger& b);
    BigInteger& operator%=(const BigInteger& a);

    BigInteger operator-() const;

    BigInteger& operator++();
    BigInteger  operator++(int);

    BigInteger& operator--();
    BigInteger  operator--(int);

    bool operator==(const BigInteger& a) const;
    bool operator!=(const BigInteger& a) const;
    bool operator<(const BigInteger& a)  const;
    bool operator>(const BigInteger& a)  const;
    bool operator<=(const BigInteger& a) const;
    bool operator>=(const BigInteger& a) const;


    friend std::istream& operator>>(std::istream& in, BigInteger& num);
    friend std::ostream& operator<<(std::ostream& out, const BigInteger& num);

    std::string toString() const;
    std::vector<int> data;
    size_t base_pow;
    size_t base;

    std::pair<BigInteger, BigInteger> split(int m) const;
    BigInteger& add(const BigInteger& a);
    BigInteger& sub(const BigInteger& a);
    BigInteger abs() const;
    BigInteger add_zeros(int count) const;

    bool negative;
};

std::string BigInteger::toString() const {
    std::string result;
    if (data.empty()) {
        return "0";
    }
    if (negative) {
        result += "-";
    }
    result += std::to_string(data[data.size() - 1]);
    for (int i = int(data.size()) - 2; i >= 0; --i) {
        size_t zero_count = 0;
        size_t current_div = base / 10;
        if (data[i] == 0) {
            zero_count = base_pow - 1;
        } else {
            while ((data[i] / current_div) == 0) {
                zero_count++;
                current_div /= 10;
            }
        }
        result += std::string(zero_count, '0') + std::to_string(data[i]);
    }
    return result;
}

BigInteger::BigInteger(const std::string& input) : BigInteger() {
    int len = input.length();
    negative = (input[0] == '-');

    if (input == "-0" || input == "0") {
        data.push_back(0);
        negative = false;
        return;
    }

    while ((!negative && len > 0) || (negative && (len > 1))) {
        int length = 0;
        int num = 0;
        int pow = 1;
        for (int i = int(len - 1); i >= 0 && i >= len - int(base_pow); --i) {
            if (input[i] == '-') {
                break;
            }
            num += (input[i] - '0') * pow;
            pow *= 10;
            ++length;
        }
        data.push_back(num);
        len -= base_pow;
    }
}

std::ostream &operator<<(std::ostream &out, const BigInteger &num) {
    out << num.toString();
    return out;
}

std::istream &operator>>(std::istream &in, BigInteger &num) {
    std::string s;
    in >> s;
    BigInteger tmp(s);
    num = tmp;
    return in;
}

BigInteger::BigInteger() : base_pow(1), base(int(10)), negative(false) {}

BigInteger& BigInteger::add(const BigInteger& a) {
    int carry = 0;
    for (size_t i = 0; i < std::max(data.size(), a.data.size()) || carry; ++i) {
        if (i == data.size()) {
            data.push_back(0);
        }
        if (i < a.data.size()) {
            data[i] += carry + a.data[i];
        } else {
            data[i] += carry;
        }
        carry = (data[i] >= int(base));
        if (carry) {
            data[i] -= base;
        }
    }
    return *this;
}

BigInteger &BigInteger::sub(const BigInteger &a) {
    int carry = 0;
    for (size_t i = 0; i< a.data.size() || carry; ++i) {
        if (i < a.data.size()) {
            data[i] -= carry + a.data[i];
        } else {
            data[i] -= carry;
        }
        carry = (data[i] < 0);
        if (carry)  {
            data[i] += base;
        }
    }
    while (data.size() > 1 && data[data.size() - 1] == 0) {
        data.pop_back();
    }
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &a) {
    return *this += -a;
}

bool BigInteger::operator<(const BigInteger &a) const {

    bool reverse = false;
    if (!negative && a.negative) return false;
    if (negative && !a.negative) return true;


    if (negative && a.negative) reverse = true;

    if (data.size() < a.data.size()) return !reverse;
    if (data.size() > a.data.size()) return reverse;
    for (size_t i(data.size()); i > 0; --i) {
        if (data[i - 1] < a.data[i - 1]) return !reverse;
        if (data[i - 1] > a.data[i - 1]) return reverse;
    }
    return false;
}

bool BigInteger::operator==(const BigInteger &a) const {
    return (!(*this < a) && !(a < *this));
}

bool BigInteger::operator!=(const BigInteger &a) const {
    return !(*this == a);
}

bool BigInteger::operator>(const BigInteger &a) const {
    return a < *this;
}

bool BigInteger::operator<=(const BigInteger &a) const {
    return ((*this < a) || (*this == a));
}

bool BigInteger::operator>=(const BigInteger &a) const {
    return a <= *this;
}

BigInteger &BigInteger::operator+=(const BigInteger &a) {
    if ((negative && a.negative) || (!negative && !a.negative)) {
        return add(a);
    }
    if (abs() > a.abs()) {

        if (data.size() == 1 && data[0] == 0) { negative = false; }
        return sub(a);
    } else {
        BigInteger tmp = a;
        tmp.sub(*this);
        *this = tmp;
        if (data.size() == 1 && data[0] == 0) { negative = false; }
        return *this;
    }
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
    BigInteger tmp = a;
    tmp += b;
    return tmp;
}

BigInteger BigInteger::operator-() const {
    BigInteger tmp = *this;
    if (tmp.data.size() == 1 && tmp.data[0] == 0) {
        return tmp;
    }
    tmp.negative = !tmp.negative;
    return tmp;
}

BigInteger BigInteger::abs() const {
    BigInteger result = *this;
    result.negative = false;
    return result;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b) {
    BigInteger tmp = a;
    tmp -= b;
    return tmp;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    BigInteger result;
    bool is_negative = false;
    if (b.negative && !a.negative) {
        is_negative = true;
    }
    else if (b.negative && a.negative) {
        is_negative = false;
    }
    else if (!b.negative && !a.negative) {
        is_negative = false;
    }
    else if (!b.negative && a.negative) {
        is_negative = true;
    }
    if (a.data.size() == 1) {
        result = b * a.data[0];
        result.negative = is_negative;
        return result;
    }
    if (b.data.size() == 1) {
        result = a * b.data[0];
        result.negative = is_negative;
        return result;
    }
    int median = std::min(a.data.size(), b.data.size());
    median = int(median / 2);
    std::pair<BigInteger, BigInteger> num1 = a.split(median);
    std::pair<BigInteger, BigInteger> num2 = b.split(median);

    BigInteger z0 = num1.second * num2.second;
    BigInteger z1 = (num1.second + num1.first) * (num2.second + num2.first);
    BigInteger z2 = num1.first * num2.first;

    result = z2.add_zeros(median * 2) + (z1 - z2 - z0).add_zeros(median) + z0;
    result.negative = is_negative;
    if (result.data.size() == 1 && result.data[0] == 0) { result.negative = false; }
    return result;
}

BigInteger BigInteger::operator*(int a) const {
    int carry = 0;
    bool negative = false;
    if (a < 0 && !this->negative) {
        a = -a;
        negative = true;
    }
    else if (a < 0 && this->negative) {
        a = -a;
        negative = false;
    }
    else if (a >= 0 && !this->negative) {
        negative = false;
    }
    else if (a >= 0 && this->negative) {
        negative = true;
    }
    BigInteger result = *this;
    for (size_t i = 0; i < result.data.size() || carry; ++i) {
        if (i == result.data.size())
            result.data.push_back(0);
        int val = carry + result.data[i] * a;
        result.data[i] = int(val % base);
        carry = int (val / base);
    }
    while (result.data.size() > 1 && result.data[result.data.size() - 1] == 0) {
        result.data.pop_back();
    }
    result.negative = negative;
    if (data.size() == 1 && data[0] == 0) { negative = false; }
    return result;
}


BigInteger &BigInteger::operator++() {
    return *this += 1;
}

BigInteger::BigInteger(int input) : BigInteger() {
    if (input == 0) {
        data.push_back(0);
        return;
    }
    if (input < 0) {
        negative = true;
        input = -input;
    }

    while (input > 0) {
        data.push_back((int) (input % base));
        input /= base;
    }
}

BigInteger BigInteger::operator++(int) {
    BigInteger tmp = *this;
    *this += 1;
    return tmp;
}

BigInteger &BigInteger::operator--() {
    return *this -= 1;
}


BigInteger BigInteger::operator--(int) {
    BigInteger tmp = *this;
    *this -= 1;
    return tmp;
}

std::pair<BigInteger, BigInteger> BigInteger::split(int m) const {
    BigInteger left, right;
    left.data.resize(data.size() - m);
    right.data.resize(m);
    for (int i = 0; i < m; ++i) {
        right.data[i] = data[i];
    }
    for (int i = 0; i < int(data.size()) - m; ++i) {
        left.data[i] = data[i + m];
    }
    return std::make_pair(left, right);
}

BigInteger BigInteger::add_zeros(int count) const {
    BigInteger result;
    result.data.resize(data.size() + count);
    result.negative = negative;
    for (int i = 0; i < count; ++i) {
        result.data[i] = 0;
    }
    for (int i = count; i < int(data.size()) + count; ++i) {
        result.data[i] = data[i - count];
    }
    return result;
}

BigInteger::operator bool() {
    return *this != 0;
}

BigInteger& BigInteger::operator*=(const BigInteger &a) {
    return *this = *this * a;
}

const BigInteger operator/(const BigInteger& a, const BigInteger& b) {

    bool is_negative = false;
    if (!a.negative && b.negative) is_negative = true;
    else if (a.negative && !b.negative) is_negative = true;


    else if (a.negative && b.negative) is_negative = false;
    if (int(a.data.size()) - int(b.data.size()) < 0) {
        return 0;
    }
    if (int(a.data.size()) - int(b.data.size()) < 0) {
        return 0;
    }

    int len = a.data.size() - b.data.size();
    BigInteger result;
    BigInteger dividend = a.abs();
    dividend.negative = false;
    for (int i = len; i >= 0; --i) {
        BigInteger cur = b.abs().add_zeros(i);
        BigInteger cur1 = cur;
        int cnt = 0;
        while (dividend - cur1 >= 0) {
            cur1 += cur;
            cnt++;
        }
        dividend = (dividend - (cur * cnt));
        result.data.push_back(cnt);
    }
    for (int i = 0; i < int(result.data.size()) / 2; ++i) {
        std::swap(result.data[i], result.data[result.data.size() - i - 1]);
    }
    while (result.data.size() > 1 && result.data[result.data.size() - 1] == 0) {
        result.data.pop_back();
    }
    result.negative = is_negative;
    if (result.data.size() == 1 && result.data[0] == 0) {
        return 0;
    }
    return result;
}

const BigInteger operator%(const BigInteger& a, const BigInteger& b) {
    if (a.abs() < b.abs()) {
        return a;
    }
    bool is_negative = false;
    if (a.negative) is_negative = true;

    if (int(a.data.size()) - int(b.data.size()) < 0) {
        return 0;
    }

    int len = a.data.size() - b.data.size();
    BigInteger dividend = a.abs();
    dividend.negative = false;
    for (int i = len; i >= 1; --i) {
        BigInteger cur = b.abs().add_zeros(i);
        BigInteger cur1 = cur;
        int cnt = 0;
        while (dividend - cur1 >= 0) {
            cur1 += cur;
            cnt++;
        }
        dividend = (dividend - (cur * cnt));
    }
    BigInteger cur = b.abs();
    BigInteger cur1 = cur;
    int cnt = 0;
    while (dividend - cur1 >= 0) {
        cur1 += cur;
        cnt++;
    }
    dividend = (dividend - (cur * cnt));
    dividend.negative = is_negative;
    if (dividend.data.size() == 1 && dividend.data[0] == 0) {
        return 0;
    }
    return dividend;
}

BigInteger &BigInteger::operator/=(const BigInteger &a) {
    return *this = *this / a;
}

BigInteger &BigInteger::operator%=(const BigInteger &a) {
    return *this = *this % a;
}

