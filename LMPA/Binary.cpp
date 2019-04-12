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
 * \brief Adds a Binary onto another. Binary bytes are guaranteed not to overflow.
 * Will truncate the right-hand-side if necessary.
 */
Binary& Binary::operator+=(const Binary& other) {

    LMPA_WARNING(precision() < other.precision(), "Warning: Truncation of Binary in Assignment to lower precision Binary.")

    auto leftiter = this->rbegin();
    auto rightiter = other.rbegin();

    bool add = false;
    while (leftiter != this->rend() && rightiter != other.rend()) {
        auto temp = byte_addition(*leftiter, *rightiter, add);
        *leftiter = temp.first;
        add = temp.second;

        ++leftiter;
        ++rightiter;
    }

    while (add && leftiter != this->rend()) {
        // need to roll over the carried one
        auto temp = byte_addition(*leftiter, 0, add);
        *leftiter = temp.first;
        add = temp.second;

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

    LMPA_WARNING(precision() < other.precision(), "Warning: Truncation of Binary in Assignment to lower precision Binary.")

    Binary result(size(), true);

    for (auto leftiter = this->rbegin(); leftiter != this->rend(); ++leftiter) {
        // resolve individual byte multiplication
        for (auto rightiter = other.rbegin(); rightiter != other.rend(); ++rightiter) {
            auto multiplicationresults = byte_multiplication(*leftiter, *rightiter, 0);
            // resolve addition of current byte
            auto resultiter = result.rbegin() + (leftiter - this->rbegin()) + (rightiter - other.rbegin());
            byte add = multiplicationresults.second;
            if (resultiter < result.rend()) {
                auto additionresults = byte_addition(*resultiter, multiplicationresults.first, 0);
                *resultiter = additionresults.first;
                add += additionresults.second;
            }
            // resolve overflow
            while (++resultiter < result.rend()) {
                auto additionresults = byte_addition(*resultiter, add, 0);
                *resultiter = additionresults.first;
                add = additionresults.second;
            }
        }
    }

    *this = result;
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
            bool tempadd = static_cast<bool>(*riter >> (sizeof(byte) * 8 - 1));
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
                *iter |= 1ULL << (sizeof(byte) * 8 - 1);
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
 * \brief Increments the Binary. Binary bytes are guaranteed not to overflow.
 * \return 
 */
Binary& Binary::operator++() {
    auto riter = this->rbegin();
    while (riter != this->rend()) {
        long_byte sum = *riter + 1u;
        *riter = static_cast<byte>(sum);
        if (sum <= std::numeric_limits<byte>::max()) {
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
bool Binary::sign() const {
    return get_value_bit(0, this->at(0));
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
 * Will leave at least one byte of values.
 */
void Binary::shrink() {
    while (size() > 1 && this->at(0) == (sign() ? ~0u : 0u)) {
        this->erase(begin());
    }
}


/**
 * \brief Returns the nth bit.
 */
bool Binary::get_bit(Binary::size_type index) const noexcept(false) {
    size_type idx = index / (sizeof(byte) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(byte) * 8 - index % (sizeof(byte) * 8) - 1;
    return static_cast<bool>(this->at(idx) & 1ULL << invertedbitidx);
}


/**
 * \brief Sets the nth bit to val.
 */
void Binary::set_bit(Binary::size_type index, bool val) noexcept(false) {
    size_type idx = index / (sizeof(byte) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(byte) * 8 - index % (sizeof(byte) * 8) - 1;
    if (val) {
        this->at(idx) |= 1ULL << invertedbitidx;
    } else {
        this->at(idx) &= ~(1ULL << invertedbitidx);
    }
}


/**
 * \brief Toggles the nth bit and returns the new value.
 */
bool Binary::toggle_bit(Binary::size_type index) noexcept(false) {
    size_type idx = index / (sizeof(byte) * 8);
    // minus one at the end because indices start at 0
    size_type invertedbitidx = sizeof(byte) * 8 - index % (sizeof(byte) * 8) - 1;
    this->at(idx) ^= 1ULL << invertedbitidx;
    return static_cast<bool>(this->at(idx) & 1ULL << invertedbitidx);
}


/**
 * \brief Flips all bits.
 */
void Binary::flip() {
    for (byte& item : *this) {
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
 * \brief Returns the nth bit of a single byte (i.e. not a Binary class object).
 */
bool Binary::get_value_bit(size_type index, byte value) const {
    return static_cast<bool>(value & 1ULL << (sizeof(byte) * 8 - index - 1));
}


/**
 * \brief Returns the add and result bytes of the sum of two bytes with a previously known add byte
 */
std::pair<Binary::byte, Binary::byte> Binary::byte_addition(byte a, byte b, byte add) {
    long_byte intermediate = a + b + add;
    return {static_cast<byte>(intermediate), static_cast<byte>(intermediate >> sizeof(byte) * 8)};
}


/**
 * \brief Returns the add and result bytes of the product of two bytes with a previously known add byte
 */
std::pair<Binary::byte, Binary::byte> Binary::byte_multiplication(byte a, byte b, byte add) {
    long_byte intermediate = a * b + add;
    return {static_cast<byte>(intermediate), static_cast<byte>(intermediate >> sizeof(byte) * 8)};
}
