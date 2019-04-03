//
// Created by Lars on 13/01/2019.
//

#include "Binary.h"
#include <cmath> // ceil
#include <limits> // overflow checking

/**
 * \brief Zero-Initialization of Binary with specified precision (in bytes)
 */
Binary::Binary(size_type prec, bool) noexcept : base_type(prec, 0) {}

/* Consider:
 *  Change logic to use a reversed binary vector (i.e. lowest elements are in the front)
 *  this might make some of the complexities better (e.g. precision changing)
 */


/**
 * \brief Adds a Binary onto another. Binary segments are guaranteed not to overflow.
 * Will truncate the right-hand-side if necessary.
 */
Binary& Binary::operator+=(const Binary& other) {
    // TODO: Write Macro for Warnings
#ifndef LMPA_DISABLE_WARNINGS
    if (precision() < other.precision()) {
        std::cerr << "Warning: Truncation of Binary in Assignment to lower precision Binary." << std::endl;
    }
#endif
    auto leftiter = this->rbegin();
    auto rightiter = other.rbegin();

    bool add = false;
    while (leftiter != this->rend() && rightiter != other.rend()) {
        // check for overflow
        large_segment sum = *leftiter + *rightiter + add;
        *leftiter = static_cast<segment>(sum);
        add = sum > std::numeric_limits<segment>::max();
        std::cout << (*this) << std::endl;

        ++leftiter;
        ++rightiter;
    }

    while (leftiter != this->rend()) {
        large_segment sum = *leftiter + add;
        *leftiter = static_cast<segment>(sum);
        add = sum > std::numeric_limits<segment>::max();
        std::cout << (*this) << std::endl;

        ++leftiter;
    }

    return *this;
}


Binary& Binary::operator-=(const Binary& other) {
    (*this) += -other;
    return *this;
}


Binary& Binary::operator*=(const Binary& other) {
    // TODO: Negative Number Multiplication
#ifndef LMPA_DISABLE_WARNINGS
    if (precision() < other.precision()) {
        std::cerr << "Warning: Truncation of Binary in Assignment to lower precision Binary." << std::endl;
    }
#endif

    // TODO: Fix

    Binary result(size(), true);
    for (size_type i = 0; i < size(); ++i) {
        size_type idx = size() - i - 1;
        segment add = 0;
        for (size_type j = 0; j < other.size(); ++j) {
            size_type jdx = other.size() - j - 1;
            large_segment intermediate = (*this)[idx] * other[jdx] + add;
            add = static_cast<segment>(intermediate >> sizeof(segment) * 8);
            int testidx = static_cast<int>(result.size() - i - j - 1);
            if (testidx >= 0) {
                result.at(result.size() - i - j - 1) += static_cast<segment>(intermediate);
            } else {
                break;
            }
        }
    }
    std::cout << this << std::endl;
    *this = result;
    std::cout << this << std::endl;

    return *this;
}


//Binary& Binary::operator/=(const Binary& other) {
//    return <#initializer#>;
//}


/**
 * \brief Shifts the entire container n bits to the left.
 */
Binary& Binary::operator<<=(Binary::size_type n) {
    // TODO: Optimize by making the shift in one go rather than splitting
    if (empty()) { return *this; }
    for (size_type i = 0; i < n; ++i) {
        auto riter = rbegin();
        bool add = false;
        while (riter != rend()) {
            // get first bit of value at riter
            bool tempadd = static_cast<bool>(*riter >> (sizeof(segment) * 8 - 1));
            *riter <<= 1;
            if (add) {
                *riter |= 1;
            }
            add = tempadd;
            ++riter;
        }
    }
    return *this;
}


/**
 * \brief Shifts the entire container n bits to the right.
 */
Binary& Binary::operator>>=(Binary::size_type n) {
    // TODO: Optimize by making the shift in one go rather than splitting
    if (empty()) { return *this; }
    for (size_type i = 0; i < n; ++i) {
        auto iter = begin();
        bool add = false;
        while (iter != end()) {
            bool tempadd = static_cast<bool>(*iter & 1);
            *iter >>= 1;
            if (add) {
                *iter |= 1ULL << (sizeof(segment) * 8 - 1);
            }
            add = tempadd;
            ++iter;
        }
    }
    return *this;
}


/**
 * \brief Shifts a copy of the entire container n bits to the left.
 */
Binary Binary::operator<<(Binary::size_type n) const {
    Binary result(*this);
    result <<= n;
    return result;
}


/**
 * \brief Shifts a copy of the entire container n bits to the right.
 */
Binary Binary::operator>>(Binary::size_type n) const {
    Binary result(*this);
    result >>= n;
    return result;
}


Binary Binary::operator+() const {
    return *this;
}


Binary Binary::operator-() const {
    return ++(this->flipped());
}

/**
 * \brief Increments the Binary. Binary segments are guaranteed not to overflow.
 * \return 
 */
Binary& Binary::operator++() {
    auto riter = this->rbegin();
    while (riter != this->rend()) {
        large_segment sum = *riter + 1u;
        *riter = static_cast<segment>(sum);
        if (sum <= std::numeric_limits<segment>::max()) {
            break;
        }

        ++riter;
    }
    return *this;
}


Binary& Binary::operator--() {
    auto riter = this->rbegin();
    while (riter != this->rend()) {
        if (*riter > 0) {
            --(*riter);
            break;
        }
        *riter = ~(*riter);

        ++riter;
    }
    return *this;
}


/**
 * \brief Checks the sign of the Binary.
 */
bool Binary::sign() {
    return get_value_bit(0, (*this)[0]);
}


/**
 * \brief Unsafely sets the precision to the specified one (in bytes). This may lower the object's value,
 * similar to static_cast on integral types.
 */
void Binary::set_precision(Binary::size_type prec) {
    if (prec < size()) {
        this->erase(begin(), begin() + size() - prec);
    } else if (prec > size()) {
        this->insert(begin(), prec - size(), (sign() ? ~0u : 0u));
    }
}


/**
 * \brief Safely promotes the Binary upwards to at least the specified precision.
 */
void Binary::promote(Binary::size_type prec) {
    if (prec > size()) {
        this->insert(begin(), prec - size(), (sign() ? ~0u : 0u));
    }
}


/**
 * \brief Safely shrinks the Binary to the minimum size needed to represent its current value.
 */
void Binary::shrink() {
    while ((*this)[0] == (sign() ? ~0u : 0u)) {
        this->erase(begin());
    }
}


/**
 * \brief Returns the nth bit.
 */
bool Binary::get_bit(Binary::size_type index) const noexcept(false) {
    indexCheck(index);
    size_type idx = index / (sizeof(segment) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(segment) * 8 - index % (sizeof(segment) * 8) - 1;
    return static_cast<bool>((*this)[idx] & 1ULL << invertedbitidx);
}


/**
 * \brief Sets the nth bit to val.
 */
void Binary::set_bit(Binary::size_type index, bool val) noexcept(false) {
    indexCheck(index);
    size_type idx = index / (sizeof(segment) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(segment) * 8 - index % (sizeof(segment) * 8) - 1;
    if (val) {
        (*this)[idx] |= 1ULL << invertedbitidx;
    } else {
        (*this)[idx] &= ~(1ULL << invertedbitidx);
    }
}


/**
 * \brief Toggles the nth bit and returns the new value.
 */
bool Binary::toggle_bit(Binary::size_type index) noexcept(false) {
    indexCheck(index);
    size_type idx = index / (sizeof(segment) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(segment) * 8 - index % (sizeof(segment) * 8) - 1;
    (*this)[idx] ^= 1ULL << invertedbitidx;
    return static_cast<bool>((*this)[idx] & 1ULL << invertedbitidx);
}


/**
 * \brief Flips all bits.
 */
void Binary::flip() {
    for (segment& item : *this) {
        item = ~item;
    }
}

/**
 * \brief Returns a copy of the Binary with all bits flipped.
 */
Binary Binary::flipped() const {
    Binary result(*this);
    result.flip();
    return result;
}


/**
 * \brief Checks if the given index is within appropriate range for the Binary.
 */
void Binary::indexCheck(size_type index) const noexcept(false) {
    if (index >= size() * sizeof(segment) * 8) {
        throw std::out_of_range("Invalid index " + std::to_string(index) + " given to Binary with precision " + std::to_string(precision()) + ".");
    }
}


/**
 * \brief Returns the nth bit of a single segment (i.e. not a Binary class object).
 */
bool Binary::get_value_bit(size_type index, segment value) const {
    return static_cast<bool>(value & 1ULL << (sizeof(segment) * 8 - index - 1));
}
