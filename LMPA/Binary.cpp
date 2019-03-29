//
// Created by Lars on 13/01/2019.
//

#include "Binary.h"

/**
 * \brief Zero-Initialization of Binary with specified precision.
 */
Binary::Binary(size_type prec, bool) noexcept : base_type(prec, 0), precision(prec) {

}

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
            bool tempadd = static_cast<bool>(*riter >> (sizeof(value_type) * 8 - 1));
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
                *iter |= 1ULL << (sizeof(value_type) * 8 - 1);
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

Binary& Binary::operator+=(const Binary& other) {
    auto leftiter = this->rbegin();
    auto rightiter = other.rbegin();

    bool add = false;
    while (leftiter != this->rend() && rightiter != other.rend()) {
        // check for overflow by adding leftiter and rightiter without their respective first bit
        // then check if overflow will occur in the first bit addition
        auto mask = static_cast<value_type>(~(1ULL << (sizeof(value_type) * 8 - 1)));
        bool firstbitadd = ((*leftiter & mask) + (*rightiter & mask) + add) >> (sizeof(value_type) * 8 - 1);
        bool overflow = (firstbitadd + get_value_bit(0, *leftiter) + get_value_bit(0, *rightiter) > 1);
        *leftiter += *rightiter + add;
        add = overflow;
        ++leftiter;
        ++rightiter;
    }

    return *this;
}

//Binary& Binary::operator-=(const Binary& other) {
//    return <#initializer#>;
//}
//
//Binary& Binary::operator*=(const Binary& other) {
//    return <#initializer#>;
//}
//
//Binary& Binary::operator/=(const Binary& other) {
//    return <#initializer#>;
//}

/**
 * \brief Returns the nth bit.
 */
bool Binary::get_bit(Binary::size_type index) const noexcept(false) {
    indexCheck(index);
    size_type idx = index / (sizeof(value_type) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(value_type) * 8 - index % (sizeof(value_type) * 8) - 1;
    return static_cast<bool>((*this)[idx] & 1ULL << invertedbitidx);
}

/**
 * \brief Sets the nth bit to val.
 */
void Binary::set_bit(Binary::size_type index, bool val) noexcept(false) {
    indexCheck(index);
    size_type idx = index / (sizeof(value_type) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(value_type) * 8 - index % (sizeof(value_type) * 8) - 1;
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
    size_type idx = index / (sizeof(value_type) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(value_type) * 8 - index % (sizeof(value_type) * 8) - 1;
    (*this)[idx] ^= 1ULL << invertedbitidx;
    return static_cast<bool>((*this)[idx] & 1ULL << invertedbitidx);
}

/**
 * \brief Flips all bits.
 */
void Binary::flip() {
    for (value_type& item : *this) {
        item = ~item;
    }
}

void Binary::indexCheck(size_type index) const noexcept(false) {
    if (index >= size() * sizeof(value_type) * 8) {
        throw std::out_of_range("Invalid index " + std::to_string(index) + " given to Binary with precision " + std::to_string(precision) + ".");
    }
}

/**
 * \brief Returns the nth bit of a single value_type (i.e. not a Binary class object).
 */
bool Binary::get_value_bit(size_type index, value_type value) const {
    return static_cast<bool>(value & 1ULL << (sizeof(value_type) * 8 - index - 1));
}
