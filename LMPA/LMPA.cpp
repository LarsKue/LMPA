//
// Created by Lars on 14/12/2018.
//

#include "LMPA.h"








/// LMPA Implementation ///

//LMPA::LMPA() noexcept {
//    pre_digits = "0";
//}
//
//// TODO: update
//LMPA::LMPA(std::string n, size_typeype prec) noexcept(false) {
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
//    size_typeype decimalpos = n.find('.');
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
//LMPA::LMPA(container_type n) noexcept {
//    precision = 0;
//    pre_digits = std::move(n);
//}
//
//LMPA::LMPA(container_type pre, container_type post, size_typeype prec) noexcept {
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
//    size_typeype minSize = std::min(this->pre_digits.size(), m.pre_digits.size());
//    size_typeype maxSize = std::max(this->pre_digits.size(), m.pre_digits.size());
//    size_typeype i = 0;
//
//    container_type result;
//    result.reserve(maxSize);
//
//    int add_subtract = 0;
//
//    // insert elements at the back of the vector and reverse at the end
//    while (i < minSize) {
//        int sum = this->pre_digits[i] + m.pre_digits[i] + add_subtract;
//        add_subtract = this->pre_digits[i] && m.pre_digits[i];
//        result.emplace_back(static_cast<value_type>(sum % 2));
//        ++i;
//    }
//
//    if (this->pre_digits.size() > m.pre_digits.size()) {
//        while (i < maxSize) {
//            int sum = this->pre_digits[i] + add_subtract;
//            add_subtract = this->pre_digits[i] && add_subtract;
//            result.emplace_back(static_cast<value_type>(sum % 2));
//            ++i;
//        }
//    }
//    if (m.pre_digits.size() > this->pre_digits.size()) {
//        while (i < maxSize) {
//            int sum = m.pre_digits[i] + add_subtract;
//            add_subtract = m.pre_digits[i] && add_subtract;
//            result.emplace_back(static_cast<value_type>(sum % 2));
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
//value_type LMPA::checkString(const std::string& s) {
//    value_type reachedNums = false;
//    value_type reachedDecimal = false;
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
//void LMPA::set_precision(size_typeype prec) {
//    if (prec < precision) {
//        post_digits = post_digits.substr(0, prec);
//    } else {
//        post_digits.reserve(prec);
//        for (size_typeype i = precision; i < prec; ++i) {
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
//container_type LMPA::toBinary(const std::string& s) {
//    while ()
//    return container_type();
//}




