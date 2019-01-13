//
// Created by Lars on 14/12/2018.
//

#include "LMPA.h"
#include <algorithm> // reverse

#include <utility>
#include <limits> // size_t max


// TODO: USE ITERATORS INSTEAD OF

/// Binary Implementation ///

namespace {
    // Binary sign definitions
    constexpr bool negative = true;
    constexpr bool positive = false;
}


/// Constructors ///

Binary::Binary() noexcept : digits(32, 0) {
    // generates a standard 32 bit precision number of value 0
}

Binary::Binary(std::size_t precision) noexcept : digits(precision, 0) {
    // generates a Binary with specific precision of value 0
}

Binary::Binary(const std::vector<bool>& d) noexcept {
    // assumes d already contains the sign
    digits = d;
    _precision = digits.size();
}

Binary::Binary(const std::vector<bool>& d, int sgn) noexcept(false) {
    digits.reserve(d.size() + 1);
    digits = d;
    if (sgn == 1) { digits.insert(digits.begin(), positive); }
    else if (sgn == -1) { digits.insert(digits.begin(), negative); }
    else {
        throw std::runtime_error("Invalid sign given to Binary Constructor: "
                                 + std::to_string(sgn) + ". Expected either 1 or -1.");
    }
    _precision = digits.size();
}

Binary::Binary(const std::vector<bool>& d, bool sgn) noexcept {
    digits = d;
    digits.insert(digits.begin(), sgn);
    _precision = digits.size();
}


/// Utility ///

void Binary::set_precision(std::size_t prec) {
    if (prec == precision()) { return; }
    if (prec > precision()) {
        // increasing precision won't alter value
        digits.insert(digits.begin(), prec - precision(), this->sign());
    } else {
        // will shrink the binary without checking for ones, thus possibly altering the value
        digits.erase(digits.begin(), digits.begin() + (precision() - prec));
    }

    _precision = prec;
}

/**
 * \brief will lower the Binary's precision as much as possible without altering its value
 * Calling this function on a Binary with 0 precision yields undefined behavior
 */
void Binary::shrink_to_fit() {
    // remove all but one of the leading values that are the same as the sign
    auto iter = digits.begin();
    while (iter != digits.end() && *(++iter) == sign());
    if (iter != digits.begin() + 1) {
        digits.erase(digits.begin() + 1, iter);
        _precision = digits.size();
    }
}

/**
 * \brief ensures the precision is at least n without altering the value
 */
void Binary::reserve(std::size_t n) {
    if (precision() < n) {
        digits.insert(digits.begin(), n - precision(), this->sign());
        _precision = n;
    }
}


/// Assignment ///

Binary& Binary::operator=(const Binary& b) {
    if (this == &b) {
        return *this;
    }

    // must not demote precision
    if (this->precision() <= b.precision()) {
        this->digits = b.digits;
    } else {
        for (std::size_t i = 0; i < b.precision(); ++i) {
            // copy from right to left
            this->digits[this->precision() - i - 1] = b.digits[b.precision() - i - 1];
        }
        for (std::size_t i = b.precision(); i < this->precision(); ++i) {
            // buffer with b's sign
            this->digits[this->precision() - i - 1] = b.sign();
        }
    }

    this->_precision = std::max(b.precision(), this->precision());
    return *this;
}

Binary& Binary::operator=(Binary&& b) noexcept {
    if (this == &b) {
        return *this;
    }

    // must not demote precision
    if (this->precision() <= b.precision()) {
        this->digits = std::move(b.digits);
    } else {
        for (std::size_t i = 0; i < b.precision(); ++i) {
            // copy from right to left
            this->digits[this->precision() - i - 1] = b.digits[b.precision() - i - 1];
        }
        for (std::size_t i = b.precision(); i < this->precision(); ++i) {
            // buffer with b's sign
            this->digits[this->precision() - i - 1] = b.sign();
        }
        b.digits.clear();
    }
    this->_precision = std::max(b.precision(), this->precision());

    b._precision = 0;

    return *this;
}

Binary& Binary::operator+=(const Binary& b) {
    // promote this to the higher precision of the two
    this->set_precision(std::max(this->precision(), b.precision()));

    // iterate from right to left, idx values always point one to the right of the current element
    std::size_t leftidx = this->precision();
    std::size_t rightidx = b.precision();

    bool add = false;

    while (leftidx > 1 && rightidx > 1) {
        bool sum = (this->digits[leftidx - 1] ^ b.digits[rightidx - 1]) ^ add;
        add = (this->digits[leftidx - 1] + b.digits[rightidx - 1] + add) > 1;
        this->digits[leftidx - 1] = sum;
        --leftidx;
        --rightidx;
    }

    // get the sign of the shorter number and "pad" the number with it
    // also need to decrement the corresponding index by one to prevent double calculation in below while statements
    bool buffer = (this->precision() > b.precision()) ? (b.digits[--rightidx]) : (this->digits[--leftidx]);

    while(leftidx > 0) {
        bool sum = this->digits[leftidx - 1] ^ add ^ buffer;
        add = (this->digits[leftidx - 1] + add + buffer) > 1;
        this->digits[leftidx - 1] = sum;
        --leftidx;
    }
    while (rightidx > 0) {
        bool sum = b.digits[rightidx - 1] ^ add ^ buffer;
        add = (b.digits[rightidx - 1] + add + buffer) > 1;
        this->digits[rightidx - 1] = sum;
        --rightidx;
    }

    return *this;
}

/// will promote acordingly
Binary& Binary::operator<<=(const std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        digits.emplace_back(0);
    }
    this->_precision = digits.size();
    return *this;
}

/// will demote accordingly
Binary& Binary::operator>>=(const std::size_t n) {
    if (n >= this->precision()) {
        digits.clear();
        this->_precision = 0;
        return *this;
    }

    for (std::size_t i = 0; i < n; ++i) {
        digits.pop_back();
    }
    this->_precision = digits.size();
    return *this;
}


/// Increment, Decrement ///

Binary Binary::operator++() {
    // prefix
    // add one to the vector
    for (std::size_t idx = digits.size(); idx > 0; --idx) {
        if (!digits[idx - 1]) {
            digits[idx - 1] = true;
            break;
        }
        digits[idx - 1] = false;
    }
    return *this;
}

Binary Binary::operator--() {
    // prefix
    // subtract one from the vector
    for (std::size_t idx = digits.size(); idx > 0; --idx) {
        if (digits[idx - 1]) {
            digits[idx - 1] = false;
            break;
        }
        digits[idx - 1] = true;
    }
    return *this;
}

const Binary Binary::operator++(int) {
    // postfix
    Binary result = *this;
    ++(*this);
    return result;
}

const Binary Binary::operator--(int) {
    // postfix
    Binary result = *this;
    ++(*this);
    return result;
}


/// Arithmetic ///

// does basically nothing
Binary Binary::operator+() const {
    return *this;
}

// inverts the sign
Binary Binary::operator-() const {
    std::vector<bool> result = digits;
    // flip all bits
    result.flip();

    // add one to the vector
    for (std::size_t idx = result.size(); idx > 0; --idx) {
        if (!result[idx - 1]) {
            result[idx - 1] = true;
            break;
        }
        result[idx - 1] = false;
    }

    return Binary(result);

}

Binary Binary::operator+(const Binary& b) const {
    // iterate from right to left, idx values always point one to the right of the current element
    std::size_t leftidx = this->precision();
    std::size_t rightidx = b.precision();

    std::vector<bool> result;
    result.reserve(std::max(this->precision(), b.precision()));

    bool add = false;

    while (leftidx > 1 && rightidx > 1) {
        bool sum = (this->digits[leftidx - 1] ^ b.digits[rightidx - 1]) ^ add;
        add = (this->digits[leftidx - 1] + b.digits[rightidx - 1] + add) > 1;
        result.emplace_back(sum);
        --leftidx;
        --rightidx;
    }

    // get the sign of the shorter number and "pad" the number with it
    // also need to decrement the corresponding index by one to prevent double calculation in below while statements
    // FIXME: Possible bug with equivalent precisions? Why should only one index be reduced, why should it be reduced at all? need to investigate
    bool buffer = (this->precision() > b.precision()) ? (b.digits[--rightidx]) : (this->digits[--leftidx]);

    while(leftidx > 0) {
        bool sum = this->digits[leftidx - 1] ^ add ^ buffer;
        add = (this->digits[leftidx - 1] + add + buffer) > 1;
        result.emplace_back(sum);
        --leftidx;
    }
    while (rightidx > 0) {
        bool sum = b.digits[rightidx - 1] ^ add ^ buffer;
        add = (b.digits[rightidx - 1] + add + buffer) > 1;
        result.emplace_back(sum);
        --rightidx;
    }

    // Consider removing this shrink call as it may be superfluous?
    result.shrink_to_fit();
    std::reverse(result.begin(), result.end());
    return Binary(result);
}

Binary Binary::operator-(const Binary& b) const {
    return *this + (-b);
}

Binary Binary::operator*(const Binary& b) const {
    std::size_t rightidx = b.precision();

    std::vector<std::vector<bool>> sums;

    // collect all vectors to add
    while (rightidx > 0) {
        if (b.digits[rightidx - 1]) {
            std::vector<bool> temp = digits;
            // add zeroes to end of vector corresponding with current index
            temp.resize(temp.size() + b.precision() - rightidx, false);
            // add vector to sums
            sums.emplace_back(temp);
        }
        --rightidx;
    }

    // add all vectors (need to have resulting vector of appropriate size from the beginning)
    std::vector<bool> result(std::max(this->precision(), b.precision()), 0);

    for (const auto& vec : sums) {
        std::size_t leftidx = result.size();
        rightidx = vec.size();

        bool add = false;
        while (leftidx > 1 && rightidx > 1) {
            bool sum = add ^ result[leftidx - 1] ^ vec[rightidx - 1];
            add = (add + result[leftidx - 1] + vec[rightidx - 1]) > 1;
            result[leftidx - 1] = sum;
            --rightidx;
            --leftidx;
        }

        // get the sign of the shorter number and "pad" the number with it
        bool buffer = (this->precision() > vec.size()) ? (vec[0]) : (this->sign());

        while(leftidx > 0) {
            bool sum = this->digits[leftidx - 1] ^ add ^ buffer;
            add = (this->digits[leftidx - 1] + add + buffer) > 1;
            result[leftidx - 1] = sum;
            --leftidx;
        }

    }

    return Binary(result);
}

std::ostream& operator<<(std::ostream& stream, const Binary& b) {
    // show as regular binary number with sign (instead of two's complement)
    if (b.sign()) {
        stream << "-" << -b;
        return stream;
    }

    stream << "0b";
    for (std::size_t i = 0; i < b.digits.size() - 1; ++i) {
        stream << b.digits[i + 1];
    }
    return stream;
}

bool Binary::operator==(const Binary& b) const {
    if (this->sign() != b.sign()) {
        return false;
    }
    std::size_t leftidx = this->precision();
    std::size_t rightidx = b.precision();
    while (leftidx > 1 && rightidx > 1) {
        if (this->digits[leftidx - 1] != b.digits[rightidx - 1]) {
            return false;
        }
        --leftidx;
        --rightidx;
    }
    while (leftidx > 1) {
        if (this->digits[leftidx - 1] != 0) {
            return false;
        }
        --leftidx;
    }

    while (rightidx > 1) {
        if (b.digits[rightidx - 1] != 0) {
            return false;
        }
        --rightidx;
    }

    return true;
}

bool Binary::operator!=(const Binary& b) const {
    return !(*this == b);
}

bool Binary::operator<(const Binary& b) const {
    // compare signs first
    if (!this->sign() && b.sign()) {
        return false;
    } else if (this->sign() && !b.sign()) {
        return true;
    }

    std::size_t leftidx = 0;
    std::size_t rightidx = 0;

    // these while loops account for leading zeroes
    while (this->precision() - leftidx > b.precision()) {
        // this->digits is longer than b.digits
        if (this->digits[leftidx] != 0) {
            return this->sign();
        }
        ++leftidx;
    }

    while (b.precision() - rightidx > this->precision()) {
        // b.digits is longer than this->digits
        if (b.digits[rightidx] != 0) {
            return !this->sign();
        }
        ++rightidx;
    }

    while (leftidx < this->precision()) {
        if (this->digits[leftidx] != b.digits[rightidx]) {
            return this->sign() ^ (this->digits[leftidx] < b.digits[rightidx]);
        }
        ++leftidx;
        ++rightidx;
    }

    return false;
}

// Todo: optimize by writing out the entire algorithm
bool Binary::operator>(const Binary& b) const {
    return (!(*this < b) && !(*this == b));
}

bool Binary::operator<=(const Binary& b) const {
    return !(*this > b);
}

bool Binary::operator>=(const Binary& b) const {
    return !(*this < b);
}

Binary Binary::operator<<(const std::size_t n) const {
    std::vector<bool> result = this->digits;
    result.insert(result.end(), n, 0);
    return Binary(result);
}

Binary Binary::operator>>(const std::size_t n) const {
    if (n >= this->precision()) {
        // return a binary with 0 precision
        return Binary(0);
    }
    std::vector<bool> result = this->digits;
    result.erase(result.end() - n, result.end());
    return Binary(result);
}




/// LMPA Implementation ///

//LMPA::LMPA() noexcept {
//    pre_digits = "0";
//}
//
//// TODO: update
//LMPA::LMPA(std::string n, std::size_t prec) noexcept(false) {
//    // check the string for validity
//    if (!checkString(n)) {
//        throw std::runtime_error("Invalid String given for Multiple Precision Number Initialization.");
//    }
//    // allows for multiple minuses and pluses, e.g. "--+-2" evaluates to sign = -1; pre_digits = "2"
//    // as well as leading zeroes (note "-0-2" evaluates to sign = 1; pre_digits = "2")
//    while (n[0] == '-' || n[0] == '+' || n[0] == '0') {
//        if (n[0] == '-') {
//            sign *= -1;
//        }
//        n.erase(0, 1); // erase the first character from n
//    }
//
//    precision = prec;
//
//    std::size_t decimalpos = n.find('.');
//    if (decimalpos == std::string::npos) {
//        pre_digits = std::move(n);
//        pre_length = pre_digits.length();
//        return;
//    }
//    pre_digits = n.substr(0, decimalpos);
//    std::string post_decimal_digit_result = n.substr(decimalpos + 1, std::min(n.length() - decimalpos - 1, precision));
//    if (precision > n.length() - decimalpos - 1) {
//        // add tailing zeroes
//        post_decimal_digit_result.reserve(precision);
//        while (post_decimal_digit_result.length() < precision) {
//            post_decimal_digit_result += '0';
//        }
//    }
//    post_digits = post_decimal_digit_result;
//    pre_length = pre_digits.length();
//}
//
//LMPA::LMPA(std::vector<bool> n) noexcept {
//    precision = 0;
//    pre_digits = std::move(n);
//}
//
//LMPA::LMPA(std::vector<bool> pre, std::vector<bool> post, std::size_t prec) noexcept {
//    precision = prec;
//    pre_digits = std::move(pre);
//    post_digits = std::move(post);
//}
//
//LMPA::LMPA(const LMPA&) {
//    std::cout << "debug" << std::endl;
//}
//
//int LMPA::toDigit(const char a) {
//    return a - '0';
//}
//
//char LMPA::toChar(const int a) {
//    if (a > 9 || a < 0) {
//        std::cerr << "Attempting conversion of invalid int to character: " << a << "!" << std::endl;
//        return '0';
//    }
//    return static_cast<char>(a + '0');
//}
//
//LMPA LMPA::operator+() const {
//    return LMPA(this->getString(), this->precision);
//}
//
//LMPA LMPA::operator-() const {
//    return LMPA("-" + this->getString(), this->precision);
//}
//
//int LMPA::insertDigit(std::string& result, int digit) {
//    if (digit > 9) {
//        result.insert(0, 1, toChar(digit - 10));
//        return 1;
//    } else if (digit < -9) {
//        result.insert(0, 1, toChar(abs(digit + 10)));
//        return -1;
//    } else {
//        result.insert(0, 1, toChar(abs(digit)));
//        return 0;
//    }
//}
//
//// TODO: Test
////  also add floating point support
//LMPA LMPA::operator+(const LMPA& m) {
//    std::size_t minSize = std::min(this->pre_digits.size(), m.pre_digits.size());
//    std::size_t maxSize = std::max(this->pre_digits.size(), m.pre_digits.size());
//    std::size_t i = 0;
//
//    std::vector<bool> result;
//    result.reserve(maxSize);
//
//    int add_subtract = 0;
//
//    // insert elements at the back of the vector and reverse at the end
//    while (i < minSize) {
//        int sum = this->pre_digits[i] + m.pre_digits[i] + add_subtract;
//        add_subtract = this->pre_digits[i] && m.pre_digits[i];
//        result.emplace_back(static_cast<bool>(sum % 2));
//        ++i;
//    }
//
//    if (this->pre_digits.size() > m.pre_digits.size()) {
//        while (i < maxSize) {
//            int sum = this->pre_digits[i] + add_subtract;
//            add_subtract = this->pre_digits[i] && add_subtract;
//            result.emplace_back(static_cast<bool>(sum % 2));
//            ++i;
//        }
//    }
//    if (m.pre_digits.size() > this->pre_digits.size()) {
//        while (i < maxSize) {
//            int sum = m.pre_digits[i] + add_subtract;
//            add_subtract = m.pre_digits[i] && add_subtract;
//            result.emplace_back(static_cast<bool>(sum % 2));
//            ++i;
//        }
//    }
//    result.shrink_to_fit();
//    std::reverse(result.begin(), result.end());
//    return LMPA(result);
//}
//
//LMPA LMPA::operator-(const LMPA& m) {
//    return *this + (-m);
//}
//
//std::ostream& operator<<(std::ostream& stream, const LMPA& m) {
//    if (m.sign == -1) { stream << "-"; }
//    stream << m.pre_digits;
//    if (m.precision > 0) {
//        stream << "." << m.post_digits;
//    }
//    return stream;
//}
//
///** Checks a string for number initialization validity.
// * Note that e.g. ".02" is an invalid string, whereas "0.02" is valid.
// * @return whether the string is valid
// */
//bool LMPA::checkString(const std::string& s) {
//    bool reachedNums = false;
//    bool reachedDecimal = false;
//    for (const auto c : s) {
//        // check if c is a sign
//        if (c == '-' || c == '+') {
//            if (reachedNums) { return false; }
//            continue;
//        }
//        // check if c is a number
//        if (c >= '0' && c <= '9') {
//            reachedNums = true;
//            continue;
//        }
//        if (c == '.') {
//            if (!reachedNums) { return false; }
//            if (reachedDecimal) { return false; }
//            reachedDecimal = true;
//            continue;
//        }
//        return false;
//    }
//    // only strings with actual numbers in them are valid
//    return reachedNums;
//}
//
//void LMPA::set_precision(std::size_t prec) {
//    if (prec < precision) {
//        post_digits = post_digits.substr(0, prec);
//    } else {
//        post_digits.reserve(prec);
//        for (std::size_t i = precision; i < prec; ++i) {
//            post_digits += '0';
//        }
//    }
//    precision = prec;
//}
//
//std::string LMPA::getString() const {
//    std::string result;
//    if (sign == -1) { result += '-'; }
//    result += pre_digits;
//    result += '.';
//    result += post_digits;
//    return result;
//}
//
//std::vector<bool> LMPA::toBinary(const std::string& s) {
//    while ()
//    return std::vector<bool>();
//}




