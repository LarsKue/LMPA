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
        long_byte sum = *leftiter + *rightiter + add;
        *leftiter = static_cast<byte>(sum);
        add = sum > std::numeric_limits<byte>::max();
        std::cout << (*this) << std::endl;

        ++leftiter;
        ++rightiter;
    }

    while (leftiter != this->rend()) {
        long_byte sum = *leftiter + add;
        *leftiter = static_cast<byte>(sum);
        add = sum > std::numeric_limits<byte>::max();
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


    Binary result(size(), true);
    for (size_type i = 0; i < size(); ++i) {
        size_type idx = size() - i - 1;
        // no need to multiply if the current byte is zero
        if (this->at(idx) == 0) { continue; }
        byte add = 0;
        for (size_type j = 0; j < other.size(); ++j) {
            size_type jdx = other.size() - j - 1;
            long_byte intermediate = (this->at(idx)) * other.at(jdx) + add;
            add = static_cast<byte>(intermediate >> sizeof(byte) * 8);
            int64_t finalidx = size() - i - j - 1;
            if (finalidx >= 0) {
                long_byte intermediate2 = result.at(static_cast<size_type>(finalidx)) + static_cast<byte>(intermediate);
                add += static_cast<byte>(intermediate2 >> sizeof(byte) * 8);
                result.at(static_cast<size_type>(finalidx)) = static_cast<byte>(intermediate2);
            }
        }
        std::cout << result << std::endl;
        if (other.size() < size() && add != 0) {
            // in case the leftmost multiplication overflows
            size_type finalidx = size() - other.size();
            while (finalidx-- != 0) {
                long_byte sum = result.at(finalidx) + add;
                add = static_cast<byte>(sum >> sizeof(byte) * 8);
                result.at(finalidx) = static_cast<byte>(sum);
                if (add == 0) { break; }
            }
        }
        std::cout << result << std::endl;
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
