//
// Created by Lars on 24/01/2019.
//

#include "Bool_Container.h"

#include <bitset>

/**
 * \brief Shifts the entire container n bits to the left.
 */
Bool_Container& Bool_Container::operator<<=(const size_type n) {
    if (n == 0 || empty()) { return *this; }
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
Bool_Container& Bool_Container::operator>>=(const size_type n) {
    if (n == 0 || empty()) { return *this; }
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
Bool_Container Bool_Container::operator<<(const size_type n) const {
    Bool_Container result(*this);
    result <<= n;
    return result;
}

/**
 * \brief Shifts a copy of the entire container n bits to the right.
 */
Bool_Container Bool_Container::operator>>(const size_type n) const {
    Bool_Container result(*this);
    result >>= n;
    return result;
}

/**
 * \brief Returns the nth bit.
 */
bool Bool_Container::get_bit(const Bool_Container::size_type n) const {
    if (n > size() * sizeof(value_type) * 8) { return false; }
    size_type idx = n / (sizeof(value_type) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(value_type) * 8 - n % (sizeof(value_type) * 8) - 1;
    return static_cast<bool>((*this)[idx] & 1ULL << invertedbitidx);
}

/**
 * \brief Sets the nth bit to val.
 */
void Bool_Container::set_bit(const Bool_Container::size_type n, bool val) {
    if (n > size() * sizeof(value_type) * 8) { return; }
    size_type idx = n / (sizeof(value_type) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(value_type) * 8 - n % (sizeof(value_type) * 8) - 1;
    if (val) {
        (*this)[idx] |= 1ULL << invertedbitidx;
    } else {
        (*this)[idx] &= ~(1ULL << invertedbitidx);
    }
}

/**
 * \brief Toggles the nth bit and returns its new value.
 */
bool Bool_Container::toggle_bit(const Bool_Container::size_type n) {
    if (n > size() * sizeof(value_type) * 8) { return false; }
    size_type idx = n / (sizeof(value_type) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(value_type) * 8 - n % (sizeof(value_type) * 8) - 1;
    (*this)[idx] ^= 1ULL << invertedbitidx;
    return static_cast<bool>((*this)[idx] & 1ULL << invertedbitidx);
}

/**
 * \brief Addition Assignment Operator. Precision of the Assigned-To Object is unchanged.
 */
Bool_Container& Bool_Container::operator+=(const Bool_Container& b) {
    auto leftiter = this->rbegin();
    auto rightiter = b.rbegin();

    bool add = false;
    while (leftiter != this->rend() && rightiter != b.rend()) {
        // get first bit of value at riter
        bool tempadd = add ^ static_cast<bool>(*leftiter >> (sizeof(value_type) * 8 - 1)) ^ static_cast<bool>(*rightiter >> (sizeof(value_type) * 8 - 1));
        *leftiter += *rightiter;
        *leftiter |= add;
        add = tempadd;
        ++leftiter;
        ++rightiter;
    }

    return *this;
}

Bool_Container::Bool_Container(Bool_Container::size_type precision, bool) noexcept : base_type(precision, 0) {

}

Bool_Container& Bool_Container::operator*=(const Bool_Container& b) {



    return *this;
}
