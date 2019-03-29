
#ifndef MULTIPRECISON_BINARY_H
#define MULTIPRECISON_BINARY_H

#if __cplusplus <= 199711L
#error This library requires at least C++11.
#endif // C++11 check


#include <vector>
#include <iostream>
#include <algorithm>

#ifdef DEBUG
#include <bitset>
#endif

#include <bitset>



class LMPA;


namespace Binary_Types {
    typedef             unsigned char                   value_type;
    typedef typename    std::vector<value_type>         base_type;
    typedef typename    base_type::size_type            size_type;
}


class Binary : Binary_Types::base_type {
    
    friend class LMPA;
    
public:
    typedef             Binary_Types::value_type        value_type;
    typedef typename    Binary_Types::base_type         base_type;
    typedef typename    Binary_Types::size_type         size_type;
    
    // Zero-Initialization Constructor, token bool for signature distinction
    explicit Binary(size_type prec, bool) noexcept;

    // conversion constructor
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true>
    Binary(const T& initializer) noexcept : base_type(sizeof(initializer), 0) {
        // TODO: Make this work for value types other than unsigned char
        auto riter = rbegin();
        for (size_type i = 0; i < sizeof(initializer); ++i) {
            *riter = static_cast<value_type>(initializer >> i * sizeof(value_type) * 8);
            ++riter;
        }
    }

    // conversion operators
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true>
    explicit operator T() const {
        T result = 0;
        for (const value_type item : *this) {
            // TODO: Make this work for value types other than unsigned char
            result <<= sizeof(value_type) * 8;
            result += item;
        }
        return result;
    }


    // inherit base class constructors
    using base_type::base_type;

    /// Operators ///

    // shift assignment
    Binary& operator<<=(size_type n);
    Binary& operator>>=(size_type n);

    // shift
    Binary operator<<(size_type n) const;
    Binary operator>>(size_type n) const;

    // arithmetic
    Binary& operator+=(const Binary& other);
    Binary& operator-=(const Binary& other);
    Binary& operator*=(const Binary& other);
    Binary& operator/=(const Binary& other);

    bool get_bit(size_type index) const noexcept(false);
    void set_bit(size_type index, bool val) noexcept(false);
    bool toggle_bit(size_type index) noexcept(false);
    void flip();

#ifdef DEBUG
    friend std::ostream& operator<<(std::ostream& stream, const Binary& b) {
        stream << "0b";
        for (const value_type v : b) {
            stream << std::bitset<sizeof(value_type) * 8>(v);
        }
        return stream;
    }
#endif

private:
    void indexCheck(size_type index) const noexcept(false);
    bool get_value_bit(size_type index, value_type value) const;

    // precision in sizeof(value_type) (e.g. bytes for value_type == unsigned char)
    size_type precision = 0;
    
};

#endif // MULTIPRECISON_BINARY_H