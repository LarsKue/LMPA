
#ifndef MULTIPRECISON_BINARY_H
#define MULTIPRECISON_BINARY_H

#if __cplusplus <= 199711L
#error This library requires at least C++11.
#endif // C++11 check

#if -1 != ~0
#warning "This library has been developed on a machine that uses Twos Complement. Yours uses Ones Complement. If you run into trouble, please contact the Developer."
#endif


#include <vector>
#include <iostream>
#include <algorithm>

#ifdef LMPA_DEBUG
#include <bitset>
#endif


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

    template<typename T>
    using enable_if_arithmetic = typename std::enable_if<std::is_arithmetic<T>::value, bool>::type;
    
    // Zero-Initialization Constructor, token bool for signature distinction
    explicit Binary(size_type prec, bool) noexcept;

    // conversion constructor
    template<typename T, enable_if_arithmetic<T> = true>
    Binary(T initializer) noexcept : base_type(sizeof(initializer), 0) {
        // TODO: Make this work for value types other than unsigned char
        auto riter = rbegin();
        for (size_type i = 0; i < sizeof(initializer); ++i) {
            *riter = static_cast<segment>(initializer >> i * sizeof(segment) * 8);
            ++riter;
        }
    }

    // conversion operators
    template<typename T, enable_if_arithmetic<T> = true>
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

    // Integral Type Assignment
    template<typename T, enable_if_arithmetic<T> = true>
    Binary& operator+=(T other) {
#ifndef LMPA_DISABLE_WARNINGS
        if (sizeof(other) > size()) {
            std::cerr << "Warning: Truncation of Integral Type in Assignment to Binary with lower Precision." << std::endl;
        }
#endif
        bool add = false;
        auto riter = rbegin();
        while (other > 0 && riter != rend()) {
            auto trunc = static_cast<segment>(other);
            large_segment sum = *riter + trunc + add;
            add = sum > std::numeric_limits<segment>::max();
            *riter = static_cast<segment>(sum);
            other >>= sizeof(segment) * 8;
            ++riter;
        }
        if (add && riter != rend()) {
            ++(*riter);
        }
        return *this;
    }

    template<typename T, enable_if_arithmetic<T> = true>
    Binary& operator-=(T other) {
        // enforce twos complement negation
        return (*this) += ++(~other);
    }

    template<typename T, enable_if_arithmetic<T> = true>
    Binary& operator*=(T other);

    template<typename T, enable_if_arithmetic<T> = true>
    Binary& operator/=(T other);

    // Binary Assignment
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
    Binary& operator++();
    Binary& operator--();

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