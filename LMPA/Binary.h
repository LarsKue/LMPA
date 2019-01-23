//
// Created by Lars on 13/01/2019.
//

#ifndef LMPA_LIBRARY_BINARY_H
#define LMPA_LIBRARY_BINARY_H

#if __cplusplus <= 199711L
#error This library requires at least C++11.
#endif // C++11 check

#include <vector> // container, size_t
#include <iostream> // operator<< stream overload, size_t
#include <algorithm> // reverse


#include <bitset> // debugging

class LMPA;

class div_by_zero_error : public std::runtime_error {
private:
    static constexpr const char* errmsg = "Division by Zero!";
public:
    div_by_zero_error() : std::runtime_error(errmsg) {}
    div_by_zero_error(const std::string& info) : std::runtime_error(errmsg) {
        std::cerr << "Error Information:\n" << info << std::endl;
    }
};

// TODO: Introduce static vs dynamic storage type (don't use different container_types)

class Binary {

    friend class LMPA;

public:
    typedef bool                                    value_type;
    typedef std::size_t                             size_type;
    // has to be a dynamic container
    typedef std::vector<value_type>                 container_type;


    /// Constructors ///
    Binary() noexcept;
    // The token boolean is necessary to differ the function signature from the Conversion Constructor.
    explicit Binary(size_type precision, bool) noexcept;
    explicit Binary(const container_type& d) noexcept;
    explicit Binary(const container_type& d, int sgn) noexcept(false);
    explicit Binary(const container_type& d, value_type sgn) noexcept;

    Binary(const Binary& b) = default;
    Binary(Binary&& b) = default;

    ~Binary() = default;

    /// Conversion Constructors ///
    // TODO: Consider this.

    /**
     * \brief Binary Conversion Constructor. Initializes a Binary from a primitive type's bit representation.
     * The precision will be that of the initializer.
     */
    template<typename T>
    Binary(const T& initializer) noexcept : digits() {
        static_assert(std::is_arithmetic<T>::value, "Binary conversion initialized with non-arithmetic or non-primitive data type!");
        for (size_type i = 0; i < sizeof(initializer) * 8; ++i) {
            digits.emplace_back(initializer & 1 << i);
        }
        _precision = sizeof(initializer) * 8;
        std::reverse(std::begin(digits), std::end(digits));
    }


    /// Conversion Operators ///
    explicit operator bool();

    /**
     * \brief Conversion Operator for Standard Arithmetic Types. Will copy bits one-by-one.
     */
    template<typename T>
    explicit operator T() {
        static_assert(std::is_arithmetic<T>::value, "Binary conversion operator called with non-arithmetic or non-primitive data type!");
        if (this->precision() == 0) {
            return 0;
        }
        T result = 0;
        auto iter = std::end(digits) - 1;
        for (size_type i = 0; i < precision(); ++i) {
            if (i > sizeof(T) * 8) { break; }
            if (*iter) {
                result |= 1 << i;
            }
            --iter;
        }
        return result;
    }


    /// Utility ///
    inline value_type sign() const { return *std::begin(digits); }
    inline size_type precision() const { return _precision; }
    void set_precision(size_type prec);
    void shrink_to_fit();
    void reserve(size_type n);
    void flip();
    void clear(); // keeps precision intact while setting value to 0
    Binary absVal() const;

    enum class PrintModes {
        Twos_Complement,
        Signed
    };

    PrintModes printmode = PrintModes::Twos_Complement;


    /// Assignment ///
    // all assignment operators may safely promote the assigned-to object's precision
    Binary& operator=(const Binary& b);
    Binary& operator=(Binary&& b) noexcept;
    Binary& operator+=(const Binary& b);
    Binary& operator-=(const Binary& b);
    Binary& operator*=(const Binary& b);
    Binary& operator/=(const Binary& b) noexcept(false);
    Binary& operator%=(const Binary& b);
    // shift assignment
    Binary& operator<<=(const size_type n);
    Binary& operator>>=(const size_type n);

    /// Increment, Decrement ///
    // prefix
    Binary& operator++();
    Binary& operator--();
    // postfix
    const Binary operator++(int);
    const Binary operator--(int);

    /// Arithmetic ///
    Binary operator+() const;
    Binary operator-() const;

    Binary operator+(const Binary& b) const;
    Binary operator-(const Binary& b) const;
    Binary operator*(const Binary& b) const;
    Binary operator/(const Binary& b) const noexcept(false);
    Binary operator%(const Binary& b) const;
    // shifting
    Binary operator<<(const std::size_t n) const;
    Binary operator>>(const std::size_t n) const;

    /// Logical ///
    // non-bitwise logical operators are replaced by boolean conversion operator
    Binary operator&(const Binary& b) const;
    Binary operator|(const Binary& b) const;

    /// Comparison ///
    bool operator==(const Binary& b) const;
    bool operator!=(const Binary& b) const;
    bool operator<(const Binary& b) const;
    bool operator>(const Binary& b) const;
    bool operator<=(const Binary& b) const;
    bool operator>=(const Binary& b) const;


    friend std::ostream& operator<< (std::ostream& stream, const Binary& b);

    // for debug purposes
    void print() const {
        for (const value_type b : digits) {
            std::cout << static_cast<bool>(b);
        }
        std::cout << std::endl; // flush is intentional
    }

private:
    size_type _precision = 32; // bits, including the sign
    container_type digits;

};


#endif //LMPA_LIBRARY_BINARY_H
