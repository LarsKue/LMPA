
#ifndef MULTIPRECISON_BINARY_H
#define MULTIPRECISON_BINARY_H

#if __cplusplus <= 199711L
#error This library requires at least C++11.
#endif // C++11 check

#if -1 != ~0
#warning "This library has been developed on a machine that uses Twos Complement. Yours uses Ones Complement or Sign Magnitude Representation. If you run into trouble, please contact the Developer."
#endif

#define LMPA_DEBUG


#include <vector> // base class
#include <iostream> // output
#include <algorithm> // iterators
#include <utility> // pair



#ifdef LMPA_DEBUG

#include <bitset>

#else

#define at(idx) operator[](idx)

#endif // LMPA_DEBUG



#ifndef LMPA_DISABLE_WARNINGS

#define LMPA_WARNING(condition, message) \
if (condition) { \
    std::cerr << message << std::endl; \
}

#else

#define LMPA_WARNING(condition, message)

#endif // LMPA_DISABLE_WARNINGS



class LMPA;


namespace Binary_Types {
    // long_byte must be of a precision higher than byte.
    // segments must be unsigned
    typedef             unsigned char                   byte;
    typedef             unsigned int                    long_byte;
    typedef typename    std::vector<byte>               base_type;
    typedef typename    base_type::size_type            size_type;
}


class Binary : protected Binary_Types::base_type {
    
    friend class LMPA;
    
public:
    typedef             Binary_Types::byte              byte;
    typedef             Binary_Types::long_byte         long_byte;
    typedef typename    Binary_Types::base_type         base_type;
    typedef typename    Binary_Types::size_type         size_type;

    template<typename T>
    using enable_if_arithmetic = typename std::enable_if<std::is_arithmetic<T>::value, bool>::type;
    
    // Zero-Initialization Constructor, token bool for signature distinction (is compiled away)
    explicit Binary(size_type prec, bool) noexcept;

    // conversion constructor
    template<typename T, enable_if_arithmetic<T> = true>
    Binary(T initializer) noexcept : base_type(sizeof(initializer), 0) {
        auto riter = rbegin();
        for (size_type i = 0; i < sizeof(initializer); ++i) {
            *riter = static_cast<byte>(initializer >> i * sizeof(byte) * 8);
            ++riter;
        }
    }

    // conversion operators, enable static casting to arithmetic types
    template<typename T, enable_if_arithmetic<T> = true>
    explicit operator T() const {
        T result = sign() ? ~0u : 0u;
        for (const byte seg : *this) {
            result <<= sizeof(byte) * 8;
            result += seg;
        }
        return result;
    }

    /// Operators ///

    // Integral Type Assignment
    template<typename T, enable_if_arithmetic<T> = true>
    Binary& operator+=(T other) {

        LMPA_WARNING(sizeof(other) > size(), "Warning: Truncation of Integral Type in Assignment to Binary with lower Precision.")

        auto leftiter = this->rbegin();
        size_type i = 0;
        byte add = 0;

        while (leftiter != this->rend() && (other >> i * 8) != 0) {
            auto temp = byte_addition(*leftiter, static_cast<byte>(other >> i * 8), add);
            *leftiter = temp.first;
            add = temp.second;

            ++leftiter;
            ++i;
        }

        while (add != 0 && leftiter != this->rend()) {
            // roll over the carried one
            auto temp = byte_addition(*leftiter, 0, add);
            *leftiter = temp.first;
            add = temp.second;

            ++leftiter;
        }

        return *this;
    }

    template<typename T, enable_if_arithmetic<T> = true>
    Binary& operator-=(T other) {
        // enforce twos complement negation
        return (*this) += -other;
    }

    template<typename T, enable_if_arithmetic<T> = true>
    Binary& operator*=(T other) {

        LMPA_WARNING(sizeof(other) > size(), "Warning: Truncation of Integral Type in Assignment to Binary with lower Precision.")

        Binary result(size(), true);

        // resolve individual byte multiplication
        for (auto leftiter = this->rbegin(); leftiter != this->rend(); ++leftiter) {
            for (size_type i = 0; i < sizeof(other); ++i) {
                // resolve addition of current byte
                byte otherbyte = static_cast<byte>(other >> i * 8);
                auto multiplicationresults = byte_multiplication(*leftiter, otherbyte, 0);

                auto resultiter = result.rbegin() + (leftiter - this->rbegin()) + i;
                byte add = multiplicationresults.second;
                if (resultiter < result.rend()) {
                    auto additionresults = byte_addition(*resultiter, multiplicationresults.first, 0);
                    // this addition may never overflow, as the maximum previous value for add is 0b011...11 or 2^(sizeof(byte) * 8 - 1) - 1
                    // and the maximum value for additionresults.second is 1.
                    *resultiter = additionresults.first;
                    add += additionresults.second;
                } else { continue; }

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


    bool sign() const;
    void set_precision(size_type prec);
    void promote(size_type prec);
    void shrink();
    size_type precision() const noexcept { return size() * 8; }
    bool get_bit(size_type index) const noexcept(false);
    void set_bit(size_type index, bool val) noexcept(false);
    bool toggle_bit(size_type index) noexcept(false);
    void flip();
    Binary flipped() const;

#ifdef LMPA_DEBUG
    friend std::ostream& operator<<(std::ostream& stream, const Binary& b) {
        stream << "0b";
        for (const byte seg : b) {
            stream << std::bitset<sizeof(byte) * 8>(seg);
        }
        return stream;
    }
#endif

private:
    void indexCheck(size_type index) const noexcept(false);
    bool get_value_bit(size_type index, byte value) const;

    // internal utility
    static std::pair<byte, byte> byte_addition(byte a, byte b, byte add);
    static std::pair<byte, byte> byte_multiplication(byte a, byte b, byte add);
};

#endif // MULTIPRECISON_BINARY_H