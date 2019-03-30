
#ifndef MULTIPRECISON_BINARY_H
#define MULTIPRECISON_BINARY_H

#if __cplusplus <= 199711L
#error This library requires at least C++11.
#endif // C++11 check


#include <vector>
#include <iostream>
#include <algorithm>

#ifdef LMPA_DEBUG
#include <bitset>
#endif

#include <bitset>



class LMPA;


namespace Binary_Types {
    // large_segment must be of a precision higher than segment.
    typedef             unsigned char                   segment;
    typedef             unsigned int                    large_segment;
    typedef typename    std::vector<segment>            base_type;
    typedef typename    base_type::size_type            size_type;
}


class Binary : protected Binary_Types::base_type {
    
    friend class LMPA;
    
public:
    typedef             Binary_Types::segment           segment;
    typedef             Binary_Types::large_segment     large_segment;
    typedef typename    Binary_Types::base_type         base_type;
    typedef typename    Binary_Types::size_type         size_type;
    
    // Zero-Initialization Constructor, token bool for signature distinction
    explicit Binary(size_type prec, bool) noexcept;

    // conversion constructor
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true>
    Binary(T initializer) noexcept : base_type(sizeof(initializer), 0) {
        // TODO: Make this work for value types other than unsigned char
        auto riter = rbegin();
        for (size_type i = 0; i < sizeof(initializer); ++i) {
            *riter = static_cast<segment>(initializer >> i * sizeof(segment) * 8);
            ++riter;
        }
    }

    // conversion operators
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true>
    explicit operator T() const {
        T result = 0;
        for (const segment seg : *this) {
            // TODO: Make this work for value types other than unsigned char
            result <<= sizeof(segment) * 8;
            result += seg;
        }
        return result;
    }

    /// Operators ///

    // assignment
    Binary& operator+=(const Binary& other);
    Binary& operator-=(const Binary& other);
    Binary& operator*=(const Binary& other);
    Binary& operator/=(const Binary& other);

    // shift assignment
    Binary& operator<<=(size_type n);
    Binary& operator>>=(size_type n);

    // shift
    Binary operator<<(size_type n) const;
    Binary operator>>(size_type n) const;

    // arithmetic
    Binary operator+() const;
    Binary operator-() const;

    // prefix increment
    Binary operator++();
    Binary operator--();

    // postfix increment
    const Binary operator++(int);
    const Binary operator--(int);

    size_type precision() const noexcept { return size() * 8; }
    bool get_bit(size_type index) const noexcept(false);
    void set_bit(size_type index, bool val) noexcept(false);
    bool toggle_bit(size_type index) noexcept(false);
    void flip();
    Binary flipped() const;

#ifdef LMPA_DEBUG
    friend std::ostream& operator<<(std::ostream& stream, const Binary& b) {
        stream << "0b";
        for (const segment seg : b) {
            stream << std::bitset<sizeof(segment) * 8>(seg);
        }
        return stream;
    }
#endif

private:
    void indexCheck(size_type index) const noexcept(false);
    bool get_value_bit(size_type index, segment value) const;
    
};

#endif // MULTIPRECISON_BINARY_H