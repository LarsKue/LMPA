//
// Created by Lars on 13/01/2019.
//

#include "Binary.h"


#include <algorithm> // reverse
#include <limits> // size_type max

namespace {
    // Binary sign definitions
    constexpr Binary::_type negative = 1;
    constexpr Binary::_type positive = 0;
}


/// Constructors ///

/**
 * \brief Default Constructor. Generates a standard 32 bit precision number of value 0
 */
Binary::Binary() noexcept : digits(32, 0) {
}

/**
 * \brief Constructor with user-specified precision. The object's value will be 0.
 */
Binary::Binary(size_type precision) noexcept : digits(precision, 0) {
}

/**
 * \brief Constructor with user-specified bits, precision is deduced automatically.
 * Will assume bits are in Two's-Complement.
 */
Binary::Binary(const container_type& d) noexcept : digits(d) {
    // assumes d already contains the sign
    _precision = digits.size();
}

/**
 * \brief Constructor with user-specified bits and a separate sign.
 * Will assume bits are in Two's-Complement.
 */
Binary::Binary(const container_type& d, int sgn) noexcept(false) : digits(d) {
    if (sgn == 1) { digits.insert(std::begin(digits), positive); }
    else if (sgn == -1) { digits.insert(std::begin(digits), negative); }
    else {
        throw std::runtime_error("Invalid sign given to Binary Constructor: "
                                 + std::to_string(sgn) + ". Expected either 1 or -1.");
    }
    _precision = digits.size();
}

/**
 * \brief Constructor with user-specified bits and a separate sign.
 * Will assume bits are in Two's-Complement.
 */
Binary::Binary(const container_type& d, _type sgn) noexcept : digits(d) {
    digits.insert(std::begin(digits), sgn);
    _precision = digits.size();
}


/// Utility ///

/**
 * \brief Will set the precision to the given size without any further checks for value.
 */
void Binary::set_precision(size_type prec) {
    if (prec == precision()) { return; }
    if (prec > precision()) {
        // increasing precision won't alter value
        digits.insert(std::begin(digits), prec - precision(), this->sign());
    } else {
        // will shrink the binary without checking for ones, thus possibly altering the value
        digits.erase(std::begin(digits), std::begin(digits) + precision() - prec);
    }

    _precision = prec;
}

/**
 * \brief Will lower the Binary's precision as much as possible without altering its value.
 * Calling this function on a Binary with 0 precision yields undefined behavior.
 */
void Binary::shrink_to_fit() {
    // remove all but one of the leading values that are the same as the sign
    auto iter = std::begin(digits);
    while (iter != std::end(digits) && *(++iter) == sign());
    if (iter != std::begin(digits) + 1) {
        digits.erase(std::begin(digits) + 1, iter);
        _precision = digits.size();
    }
}

/**
 * \brief Ensures the precision is at least n without altering the value.
 */
void Binary::reserve(size_type n) {
    if (precision() < n) {
        digits.insert(std::begin(digits), n - precision(), this->sign());
        _precision = n;
    }
}

/**
 * \brief Flips all bits.
 */
void Binary::flip() {
    for (auto& b : digits) {
        b = static_cast<_type>(!b);
    }
}

/**
 * \brief Sets the object's value to 0 without altering its precision.
 */
void Binary::clear() {
    for (auto& b : digits) {
        b = 0;
    }
}


/// Assignment ///

/**
 * \brief Assignment Operator. Will promote the assigned-to object accordingly.
 */
Binary& Binary::operator=(const Binary& b) {
    if (this == &b) {
        return *this;
    }

    // must not demote precision
    if (this->precision() <= b.precision()) {
        this->digits = b.digits;
    } else {
        for (size_type i = 0; i < b.precision(); ++i) {
            // copy from right to left
            *(std::end(this->digits) - i - 1) = *(std::end(b.digits) - i - 1);
        }
        for (size_type i = b.precision(); i < this->precision(); ++i) {
            // buffer with b's sign
            *(std::end(this->digits) - i - 1) = b.sign();
        }
    }

    this->_precision = std::max(b.precision(), this->precision());
    return *this;
}

/**
 * \brief Move assignment operator. Will promote the assigned-to object accordingly.
 */
Binary& Binary::operator=(Binary&& b) noexcept {
    if (this == &b) {
        return *this;
    }

    // must not demote precision
    if (this->precision() <= b.precision()) {
        this->digits = std::move(b.digits);
    } else {
        for (size_type i = 0; i < b.precision(); ++i) {
            // copy from right to left
            *(std::end(this->digits) - i - 1) = *(std::end(b.digits) - i - 1);
        }
        for (size_type i = b.precision(); i < this->precision(); ++i) {
            // buffer with b's sign
            *(std::end(this->digits) - i - 1) = b.sign();
        }
        b.digits.clear();
    }
    this->_precision = std::max(b.precision(), this->precision());

    b._precision = 0;

    return *this;
}

/**
 * \brief Addition Assignment Operator. Will promote the assigned-to object accordingly.
 */
Binary& Binary::operator+=(const Binary& b) {
    // promote this to the higher precision of the two
    this->reserve(std::max(this->precision(), b.precision()));

    // iterate from right to left, iterators must always point one to the right of the current object
    auto leftiter = std::end(this->digits);
    auto rightiter = std::end(b.digits);

    bool add = false;

    while (leftiter > std::begin(this->digits) + 1 && rightiter > std::begin(b.digits) + 1) {
        _type sum = *(leftiter - 1) ^ *(rightiter - 1) ^ add;
        add = ( *(leftiter - 1) + *(rightiter - 1) + add ) > 1;
        *(leftiter - 1) = sum;
        --leftiter;
        --rightiter;
    }

    while (leftiter > std::begin(this->digits)) {
        // this can be of higher precision than b, but not the other way around
        _type sum = *(leftiter - 1) ^ add ^ b.sign();
        add = (*(leftiter - 1) + add + b.sign()) > 1;
        *(leftiter - 1) = sum;
        --leftiter;
    }

    return *this;
}

/**
 * \brief Subtraction Assignment Operator. See Implementation for operator+=
 */
Binary& Binary::operator-=(const Binary& b) {
    return *this += (-b);
}

/**
 * \brief Multiplication Assignment Operator. Will promote the assigned-to object accordingly.
 */
Binary& Binary::operator*=(const Binary& b) {
    // promote this to the higher precision of the two
    this->reserve(std::max(this->precision(), b.precision()));

    auto rightiter = std::end(b.digits);

    std::vector<container_type> sums;
    sums.reserve(b.precision()); // worst-case allocation (b is all ones)

    // collect all vectors to add and bring them in appropriate form
    while (rightiter > std::begin(b.digits) + 1) {
        if (*(rightiter - 1)) {
            container_type temp(std::begin(digits) + (std::end(b.digits) - rightiter), std::end(digits));
            // TODO: Test
            // add zeroes to end of vector corresponding with current index
            temp.insert(std::end(temp), static_cast<size_type>(std::end(b.digits) - rightiter), 0);
            // add vector to sums
            sums.emplace_back(temp);
        }
        --rightiter;
    }

    sums.shrink_to_fit();

    // clear the original digits before adding to them
    this->clear();

    // add the vectors
    for (const auto& vec : sums) {
        // this will always be shorter than, or of equivalent length as vec
        auto leftiter = std::end(digits);
        rightiter = std::end(vec);

        bool add = false;
        while (leftiter > std::begin(digits) && rightiter > std::begin(vec)) {
            _type sum = *(leftiter - 1) ^ *(rightiter - 1) ^ add;
            add = (*(leftiter - 1) + *(rightiter - 1) + add) > 1;
            *(leftiter - 1) = sum;
            --leftiter;
            --rightiter;
        }
    }

    return *this;
}

/**
 * \brief Left-Shift Assignment Operator. Will promote the assigned-to object by n digits.
 */
Binary& Binary::operator<<=(const size_type n) {
    digits.insert(std::end(digits), n, 0);
    this->_precision = digits.size();
    return *this;
}

/**
 * \brief Right-Shift Assignment Operator. Will not alter the object's precision.
 * If n is greater than the object's precision, the behavior is undefined.
 */
Binary& Binary::operator>>=(const size_type n) {
    digits.erase(std::begin(digits) + n, std::end(digits));
    digits.insert(std::begin(digits), n, 0);
    return *this;
}


/// Increment, Decrement ///

/**
 * \brief Prefix-Increment Operator.
 */
Binary Binary::operator++() {
    // prefix
    // add one to the vector
    for (auto iter = std::end(digits); iter-- > std::begin(digits); /* no increment */) {
        if (!(*iter)) {
            *iter = 1;
            break;
        }
        *iter = 0;
    }
    return *this;
}

/**
 * \brief Prefix-Decrement Operator.
 */
Binary Binary::operator--() {
    // prefix
    // subtract one from the vector
    for (auto iter = std::end(digits); iter-- > std::begin(digits); /* no increment */) {
        if (*iter) {
            *iter = 0;
            break;
        }
        *iter = 1;
    }
    return *this;
}

/**
 * \brief Postfix-Increment Operator.
 */
const Binary Binary::operator++(int) {
    // postfix
    Binary result = *this;
    ++(*this);
    return result;
}

/**
 * \brief Postfix-Decrement Operator.
 */
const Binary Binary::operator--(int) {
    // postfix
    Binary result = *this;
    --(*this);
    return result;
}


/// Arithmetic ///

/**
 * \brief This does nothing.
 */
Binary Binary::operator+() const {
    return *this;
}

/**
 * \brief Inerts the sign of a copy of the Binary.
 */
Binary Binary::operator-() const {
    container_type result = digits;
    // flip all bits
    for (auto& b : result) {
        b = static_cast<_type>(!b);
    }

    // add one to the vector
    for (auto iter = std::end(result); iter-- > std::begin(result); /* no increment */) {
        if (!*iter) {
            *iter = 1;
            break;
        }
        *iter = 0;
    }

    return Binary(result);

}

/**
 * \brief Addition Operator. The result will be of the maximum precision of the two arguments.
 */
Binary Binary::operator+(const Binary& b) const {
    // iterate from right to left, iterators must always point one to the right of the current element
    auto leftiter = std::end(digits);
    auto rightiter = std::end(b.digits);

    container_type result;
    result.reserve(std::max(this->precision(), b.precision()));

    bool add = false;

    while (leftiter > std::begin(digits) + 1 && rightiter > std::begin(digits) + 1) {
        result.emplace_back(*(leftiter - 1) ^ *(rightiter - 1) ^ add);
        add = (*(leftiter - 1) + *(rightiter - 1) + add) > 1;
        --leftiter;
        --rightiter;
    }

    // get the sign of the shorter number and "pad" the number with it
    // also need to decrement the corresponding index by one to prevent double calculation in below while statements
    _type buffer = (this->precision() > b.precision()) ? (*(--rightiter)) : (*(--leftiter));

    while (leftiter > std::begin(digits)) {
        result.emplace_back(*(leftiter - 1) ^ buffer ^ add);
        add = (*(leftiter - 1) + buffer + add) > 1;
        --leftiter;
    }

    while (rightiter > std::begin(b.digits)) {
        result.emplace_back(*(rightiter - 1) ^ buffer ^ add);
        add = (*(rightiter - 1) + buffer + add);
        --rightiter;
    }

    std::reverse(std::begin(result), std::end(result));
    return Binary(result);
}

/**
 * \brief Subtraction Operator. See Implementation for operator+
 */
Binary Binary::operator-(const Binary& b) const {
    return *this + (-b);
}

/**
 * \brief Multiplication Operator. The result will be of the maximum precision of the two arguments.
 */
Binary Binary::operator*(const Binary& b) const {
    auto rightiter = std::end(b.digits);

    std::vector<container_type> sums;
    sums.reserve(b.precision()); // worst-case allocation (b is all ones)

    // collect all vectors to add and bring them in appropriate form
    while (rightiter > std::begin(b.digits) + 1) {
        if (*(rightiter - 1)) {
            container_type temp(std::begin(digits) + (std::end(b.digits) - rightiter), std::end(digits));
            // add zeroes to end of vector corresponding with current index
            temp.insert(std::end(temp), static_cast<size_type>(std::end(b.digits) - rightiter), 0);
            // if temp is still smaller than the larger of the two precisions, pad it with its sign
            if (temp.size() < b.precision()) {
                temp.insert(std::begin(temp), b.precision() - temp.size(), this->sign());
            }
            // add vector to sums
            sums.emplace_back(temp);
        }
        --rightiter;
    }

    sums.shrink_to_fit();

    // add all vectors (need to have resulting vector of appropriate size from the beginning)
    container_type result(std::max(this->precision(), b.precision()), 0);

    for (const auto& vec : sums) {
        // result will always be shorter than, or of equivalent length as vec
        auto leftiter = std::end(result);
        rightiter = std::end(vec);

        bool add = false;
        while (leftiter > std::begin(result) && rightiter > std::begin(vec)) {
            _type sum = *(leftiter - 1) ^ *(rightiter - 1) ^ add;
            add = (*(leftiter - 1) + *(rightiter - 1) + add) > 1;
            *(leftiter - 1) = sum;
            --leftiter;
            --rightiter;
        }
    }

    return Binary(result);
}

/**
 * \brief Stream Output Operator. Will either print Two's-Complement or a Signed Binary, depending on printmode.
 */
std::ostream& operator<<(std::ostream& stream, const Binary& b) {
    if (b.printmode == Binary::PrintModes::Signed) {
        // show as regular binary number with sign instead of first bit
        if (b.sign()) {
            stream << "-" << -b;
            return stream;
        }

        stream << "0b";
        for (auto iter = std::begin(b.digits) + 1; iter < std::end(b.digits); ++iter) {
            stream << static_cast<bool>(*iter);
        }
        return stream;
    }
    // show as twos complement
    stream << "0b";
    for (const auto digit : b.digits) {
        stream << static_cast<bool>(digit);
    }
    return stream;
}

/// Logical ///

/**
 * \brief Comparison Operator. Compares bit by bit.
 */
bool Binary::operator==(const Binary& b) const {
    if (this->sign() != b.sign()) {
        return false;
    }

    auto leftiter = std::end(this->digits);
    auto rightiter = std::end(b.digits);

    while (leftiter-- > std::begin(this->digits) + 1 && rightiter-- > std::begin(b.digits) + 1) {
        if (*leftiter != *rightiter) {
            return false;
        }
    }
    while (leftiter-- > std::begin(this->digits) + 1) {
        if (*leftiter) {
            return false;
        }
    }

    while (rightiter-- > std::begin(b.digits) + 1) {
        if (*rightiter) {
            return false;
        }
    }

    return true;
}

/**
 * \brief Anti-Comparison-Operator. See Implementation for operator==
 */
bool Binary::operator!=(const Binary& b) const {
    return !(*this == b);
}

/**
 * \brief Left-Hand-Comparison Operator. Compares bit by bit.
 */
bool Binary::operator<(const Binary& b) const {
    // compare signs first
    if (!this->sign() && b.sign()) {
        return false;
    } else if (this->sign() && !b.sign()) {
        return true;
    }

    auto leftiter = std::begin(this->digits);
    auto rightiter = std::begin(b.digits);

    // these while loops account for leading zeroes
    while (this->precision() - (leftiter - std::begin(digits)) > b.precision()) {
        // this->digits is longer than b.digits
        if (*leftiter) {
            return this->sign();
        }
        ++leftiter;
    }

    while (b.precision() - (rightiter - std::begin(b.digits)) > this->precision()) {
        // b.digits is longer than this->digits
        if (*rightiter) {
            return !this->sign();
        }
        ++rightiter;
    }

    while (leftiter < std::end(this->digits)) {
        // iterators are synchronized
        if (*leftiter != *rightiter) {
            return this->sign() ^ (*leftiter < *rightiter);
        }
        ++leftiter;
        ++rightiter;
    }

    return false;
}

// Todo: optimize by writing out the entire algorithm
/**
 * \brief Right-Hand-Comparison Operator. See Implementation for operator== and operator<
 */
bool Binary::operator>(const Binary& b) const {
    return (!(*this < b) && !(*this == b));
}

/**
 * \brief Left-Hand-Equality-Comparison Operator. See Implementation for operator>
 */
bool Binary::operator<=(const Binary& b) const {
    return !(*this > b);
}

/**
 * \brief Right-Hand-Equality-Comparison Operator. See Implementation for operator<
 */
bool Binary::operator>=(const Binary& b) const {
    return !(*this < b);
}

/**
 * \brief Left-Shift-Operator. Will Promote the copy by n digits.
 */
Binary Binary::operator<<(const size_type n) const {
    container_type result = this->digits;
    result.insert(result.end(), n, 0);
    return Binary(result);
}

/**
 * \brief Right-Shift-Operator. Will keep the copy's precision the same as the object's.
 * If n is greater than the object's precision, the behavior is undefined.
 */
Binary Binary::operator>>(const size_type n) const {
    container_type result = this->digits;
    result.erase(std::end(result) - n, std::end(result));
    result.insert(std::begin(result), n, 0);
    return Binary(result);
}
