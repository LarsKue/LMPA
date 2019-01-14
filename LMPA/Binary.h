//
// Created by Lars on 13/01/2019.
//

#ifndef LMPA_LIBRARY_BINARY_H
#define LMPA_LIBRARY_BINARY_H

#include <vector> // container, size_t
#include <iostream> // operator<< stream overload, size_t

class Binary {
public:
    typedef unsigned char _type;
    typedef std::size_t size_type;
    // has to be a dynamic container
    typedef std::vector<_type> container_type;

    /// Constructors ///
    Binary() noexcept;
    explicit Binary(std::size_t precision) noexcept;
    explicit Binary(const container_type& d) noexcept;
    explicit Binary(const container_type& d, int sgn) noexcept(false);
    explicit Binary(const container_type& d, _type sgn) noexcept;

    Binary(const Binary& b) = default;
    Binary(Binary&& b) noexcept = default;

    ~Binary() = default;


    /// Utility ///
    inline _type sign() const { return *std::begin(digits); }
    inline size_type precision() const { return _precision; }
    void set_precision(size_type prec);
    void shrink_to_fit();
    void reserve(size_type n);
    void flip();
    void clear(); // keeps precision intact while setting value to 0

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
    Binary& operator/=(const Binary& b);
    Binary& operator%=(const Binary& b);
    // shift assignment
    Binary& operator<<=(const size_type n);
    Binary& operator>>=(const size_type n);

    /// Increment, Decrement ///
    // prefix
    Binary operator++();
    Binary operator--();
    // postfix
    const Binary operator++(int);
    const Binary operator--(int);

    /// Arithmetic ///
    Binary operator+() const;
    Binary operator-() const;

    Binary operator+(const Binary& b) const;
    Binary operator-(const Binary& b) const;
    Binary operator*(const Binary& b) const;
    Binary operator/(const Binary& b) const;
    Binary operator%(const Binary& b) const;
    // shifting
    Binary operator<<(const std::size_t n) const;
    Binary operator>>(const std::size_t n) const;

    /// Logical ///
    bool operator!() const;
    bool operator&&(const Binary& b) const;
    bool operator||(const Binary& b) const;
    // bitwise logical -- Consider returning Binaries here instead / on top of bool
    bool operator&(const Binary& b) const;
    bool operator|(const Binary& b) const;

    /// Comparison ///
    bool operator==(const Binary& b) const;
    bool operator!=(const Binary& b) const;
    bool operator<(const Binary& b) const;
    bool operator>(const Binary& b) const;
    bool operator<=(const Binary& b) const;
    bool operator>=(const Binary& b) const;


    friend std::ostream& operator<< (std::ostream& stream, const Binary& b);

    // for debug purposes
    void print() {
        for (const _type b : digits) {
            std::cout << static_cast<bool>(b) << ", ";
        }
        std::cout << std::endl;
    }

private:
    size_type _precision = 32; // bits, including the sign
    container_type digits;

};


#endif //LMPA_LIBRARY_BINARY_H
