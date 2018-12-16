//
// Created by Lars on 14/12/2018.
//

#include "LMPA.h"

LMPA::LMPA() noexcept {
    pre_digits = "0";
}

LMPA::LMPA(std::string n, std::size_t prec) noexcept(false) {
    // check the string for validity
    if (!checkString(n)) {
        throw std::runtime_error("Invalid String given for Multiple Precision Number Initialization.");
    }
    // allows for multiple minuses and pluses, e.g. "--+-2" evaluates to sign = -1; pre_digits = "2"
    // as well as leading zeroes (note "-0-2" evaluates to sign = 1; pre_digits = "2")
    while (n[0] == '-' || n[0] == '+' || n[0] == '0') {
        if (n[0] == '-') {
            sign *= -1;
        }
        n.erase(0, 1); // erase the first character from n
    }

    precision = prec;

    std::size_t decimalpos = n.find('.');
    if (decimalpos == std::string::npos) {
        pre_digits = std::move(n);
        pre_length = pre_digits.length();
        return;
    }
    pre_digits = n.substr(0, decimalpos);
    std::string post_decimal_digit_result = n.substr(decimalpos + 1, std::min(n.length() - decimalpos - 1, precision));
    if (precision > n.length() - decimalpos - 1) {
        // add tailing zeroes
        post_decimal_digit_result.reserve(precision);
        while (post_decimal_digit_result.length() < precision) {
            post_decimal_digit_result += '0';
        }
    }
    post_digits = post_decimal_digit_result;
    pre_length = pre_digits.length();
}

LMPA::LMPA(const LMPA&) {
    std::cout << "debug" << std::endl;
}

int LMPA::toDigit(const char a) {
    return a - '0';
}

char LMPA::toChar(const int a) {
    if (a > 9 || a < 0) {
        std::cerr << "Attempting conversion of invalid int to character: " << a << "!" << std::endl;
        return '0';
    }
    return static_cast<char>(a + '0');
}

LMPA LMPA::operator+() const {
    return LMPA(this->getString(), this->precision);
}

LMPA LMPA::operator-() const {
    return LMPA("-" + this->getString(), this->precision);
}

int LMPA::insertDigit(std::string& result, int digit) {
    if (digit > 9) {
        result.insert(0, 1, toChar(digit - 10));
        return 1;
    } else if (digit < -9) {
        result.insert(0, 1, toChar(abs(digit + 10)));
        return -1;
    } else {
        result.insert(0, 1, toChar(abs(digit)));
        return 0;
    }
}

LMPA LMPA::operator+(const LMPA& m) {
    std::string pre_result;
    std::string post_result;
    pre_result.reserve(std::max(m.pre_length, this->pre_length));

    std::size_t maxprec = std::max(this->precision, m.precision);
    post_result.reserve(maxprec);

    std::size_t i = 0;

    // -1, 0 or 1
    int add_subtract = 0;

    // TODO: write this better / optimize

    // resolve rightmost digits from number with higher precision first
    if (this->precision > m.precision) {
        while (i <= m.precision) {
            int resultingDigit = add_subtract + this->sign * toDigit(this->post_digits[this->precision - i - 1]);
            add_subtract = insertDigit(post_result, resultingDigit);
            ++i;
        }
    }

    if (m.precision > this->precision) {
        while (i <= this->precision) {
            int resultingDigit = add_subtract + m.sign * toDigit(m.post_digits[m.precision - i - 1]);
            add_subtract = insertDigit(post_result, resultingDigit);
            ++i;
        }
    }


    while (i < maxprec) {
        int resultingDigit = add_subtract + this->sign * toDigit(this->post_digits[maxprec - i - 1]) + m.sign * toDigit(m.post_digits[maxprec - i - 1]);
        add_subtract = insertDigit(post_result, resultingDigit);
        ++i;
    }

    i = 0;

    // resolve rightmost digits from integer part first
    while (i < std::min(this->pre_length, m.pre_length)) {
        int resultingDigit = add_subtract + this->sign * toDigit(this->pre_digits[this->pre_length - i - 1]) +
                             m.sign * toDigit(m.pre_digits[m.pre_length - i - 1]);
        add_subtract = insertDigit(pre_result, resultingDigit);
        ++i;
    }

    // resolve remaining pre_digits from longer number
    // TODO: optimize as to break out once add_subtract hits 0
    //  (only copying the larger string remainder rather than inserting every digit one by one)
    if (this->pre_length > m.pre_length) {
        while (i < this->pre_length) {
            int resultingDigit = add_subtract + this->sign * toDigit(this->pre_digits[this->pre_length - i - 1]);
            add_subtract = insertDigit(pre_result, resultingDigit);
            ++i;
        }
    } else if (this->pre_length < m.pre_length) {
        while (i < m.pre_length) {
            int resultingDigit = add_subtract + m.sign * toDigit(m.pre_digits[m.pre_length - i - 1]);
            add_subtract = insertDigit(pre_result, resultingDigit);
            ++i;
        }
    }

    if (add_subtract != 0) {
        pre_result.insert(0, 1, '1');
    }

    // drop unused reserved memory
    pre_result.shrink_to_fit();

    // determine outcome sign
    int sgn;
    if (this->pre_digits >= m.pre_digits) {
        sgn = this->sign;
    } else {
        sgn = m.sign;
    }

    if (post_result.length() == 0) {
        if (sgn == 1) {
            return LMPA(pre_result, 0);
        }
        return LMPA("-" + pre_result, 0);
    }

    if (sgn == 1) {
        return LMPA(pre_result + "." + post_result, post_result.length());
    }
    return LMPA("-" + pre_result + "." + post_result, post_result.length());

}

LMPA LMPA::operator-(const LMPA& m) {
    return *this + (-m);
}

std::ostream& operator<<(std::ostream& stream, const LMPA& m) {
    if (m.sign == -1) { stream << "-"; }
    stream << m.pre_digits;
    if (m.precision > 0) {
        stream << "." << m.post_digits;
    }
    return stream;
}

/** Checks a string for number initialization validity.
 * Note that e.g. ".02" is an invalid string, whereas "0.02" is valid.
 * @return whether the string is valid
 */
bool LMPA::checkString(const std::string& s) {
    bool reachedNums = false;
    bool reachedDecimal = false;
    for (const auto c : s) {
        // check if c is a sign
        if (c == '-' || c == '+') {
            if (reachedNums) { return false; }
            continue;
        }
        // check if c is a number
        if (c >= '0' && c <= '9') {
            reachedNums = true;
            continue;
        }
        if (c == '.') {
            if (!reachedNums) { return false; }
            if (reachedDecimal) { return false; }
            reachedDecimal = true;
            continue;
        }
        return false;
    }
    // only strings with actual numbers in them are valid
    return reachedNums;
}

void LMPA::setPrecision(std::size_t prec) {
    if (prec < precision) {
        post_digits = post_digits.substr(0, prec);
    } else {
        post_digits.reserve(prec);
        for (std::size_t i = precision; i < prec; ++i) {
            post_digits += '0';
        }
    }
    precision = prec;
}

std::string LMPA::getString() const {
    std::string result;
    if (sign == -1) { result += '-'; }
    result += pre_digits;
    result += '.';
    result += post_digits;
    return result;
}


