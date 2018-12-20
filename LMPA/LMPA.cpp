//
// Created by Lars on 14/12/2018.
//

#include "LMPA.h"
#include <algorithm> // reverse

#include <utility>

/// Binary Implementation ///

Binary::Binary() noexcept {
    sign = positive;
    digits.emplace_back(0);
}

Binary::Binary(const std::vector<bool>& d) noexcept {
    sign = positive;
    digits = d;
    _precision = digits.size();
}

Binary::Binary(const std::vector<bool>& d, int sgn) noexcept(false) {
    if (sgn == 1) { sign = positive; }
    else if (sgn == -1) { sign = negative; }
    else {
        throw std::runtime_error("Invalid sign given to Binary Constructor: "
        + std::to_string(sgn) + ". Expected either 1 or -1.");
    }
    digits = d;
    _precision = digits.size();
}

Binary::Binary(const std::vector<bool>& d, bool sgn) noexcept {
    sign = sgn;
    digits = d;
    _precision = digits.size();
}

// does basically nothing
Binary Binary::operator+() const {
    return Binary(this->digits, this->sign);
}

// inverts the sign
Binary Binary::operator-() const {
    bool sgn = !sign;
    std::vector<bool> result = digits;
    // flip all bits
    result.flip();

    bool add = true;

    // add one to the vector
    for (std::size_t idx = result.size(); idx > 0; --idx) {
        if (!result[idx - 1]) {
            result[idx - 1] = true;
            add = false;
            break;
        }
        result[idx - 1] = false;
    }

    if (add) {
        sgn = !sgn;
    }

    return Binary(result, sgn);

}

Binary Binary::operator+(const Binary& b) const {
    std::size_t leftidx = this->_precision;
    std::size_t rightidx = b._precision;

    // result is automatically promoted to the higher precision
    std::vector<bool> result;
    result.reserve(std::max(this->_precision, b._precision));

    bool add = false;

    while (leftidx > 0 && rightidx > 0) {
        bool sum = (this->digits[leftidx - 1] ^ b.digits[rightidx - 1]) ^ add;
        add = (this->digits[leftidx - 1] + b.digits[rightidx - 1] + add) > 1;
        result.emplace_back(sum);
        --leftidx;
        --rightidx;
    }

    // buffer with "zeroes"
    while(leftidx > 0) {
        bool sum = this->digits[leftidx - 1] ^ add;
        add = (this->digits[leftidx - 1] + add) > 1;
        result.emplace_back(sum);
        --leftidx;
    }
    while (rightidx > 0) {
        bool sum = b.digits[rightidx - 1] ^ add;
        add = (b.digits[rightidx - 1] + add) > 1;
        result.emplace_back(sum);
        --rightidx;
    }

    bool resultsign = this->sign ^ b.sign ^ add;

    result.shrink_to_fit();
    std::reverse(result.begin(), result.end());
    return Binary(result, resultsign);
}

Binary Binary::operator-(const Binary& b) const {
    return *this + (-b);
}

std::ostream& operator<<(std::ostream& stream, const Binary& b) {
    // show as regular binary number with sign (instead of two's complement)
    if (b.sign) {
        stream << "-" << -b;
        return stream;
    }

    stream << "0b";
    for (const bool mB : b.digits) {
        stream << mB;
    }
    return stream;
}

bool Binary::operator==(const Binary& b) const {
    if (this->sign != b.sign) {
        return false;
    }
    std::size_t leftidx = this->_precision;
    std::size_t rightidx = b._precision;
    while (leftidx > 0 && rightidx > 0) {
        if (this->digits[leftidx - 1] != b.digits[rightidx - 1]) {
            return false;
        }
        --leftidx;
        --rightidx;
    }
    while (leftidx > 0) {
        if (this->digits[leftidx - 1] != 0) {
            return false;
        }
        --leftidx;
    }

    while (rightidx > 0) {
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
    if (!this->sign && b.sign) {
        return false;
    } else if (this->sign && !b.sign) {
        return true;
    }

    std::size_t leftidx = 0;
    std::size_t rightidx = 0;

    // these while loops account for leading zeroes
    while (this->_precision - leftidx > b._precision) {
        // this->digits is longer than b.digits
        if (this->digits[leftidx] != 0) {
            return this->sign;
        }
        ++leftidx;
    }

    while (b._precision - rightidx > this->_precision) {
        // b.digits is longer than this->digits
        if (b.digits[rightidx] != 0) {
            return !this->sign;
        }
        ++rightidx;
    }

    while (leftidx < this->_precision) {
        if (this->digits[leftidx] != b.digits[rightidx]) {
            return this->sign ^ (this->digits[leftidx] < b.digits[rightidx]);
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

void Binary::setPrecision(std::size_t prec) {
    if (prec == _precision) { return; }
    if (prec > _precision) {
        digits.insert(digits.begin(), prec - _precision, false);
    } else {
        // will shrink the binary without checking for ones
        digits.erase(digits.begin(), digits.begin() + (_precision - prec));
    }

    _precision = prec;
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
//void LMPA::setPrecision(std::size_t prec) {
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




