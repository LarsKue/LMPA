//
// Created by Lars on 13/01/2019.
//

#ifndef LMPA_LIBRARY_BINARY_H
#define LMPA_LIBRARY_BINARY_H

#include <vector> // container, size_t
#include <iostream> // operator<< stream overload, size_t
#include <deque>

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
    typedef bool                            value_type;
    typedef std::size_t                     size_type;
    // has to be a dynamic container
    typedef std::vector<value_type>         container_type;

    /// Constructors ///
    Binary() noexcept;
    explicit Binary(std::size_t precision) noexcept;
    explicit Binary(const container_type& d) noexcept;
    explicit Binary(const container_type& d, int sgn) noexcept(false);
    explicit Binary(const container_type& d, value_type sgn) noexcept;

    Binary(const Binary& b) = default;
    Binary(Binary&& b) = default;

    ~Binary() = default;


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
    Binary operator/(const Binary& b) const noexcept(false);
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
    void print() const {
        for (const value_type b : digits) {
            std::cout << static_cast<bool>(b) << ", ";
        }
        std::cout << std::endl;
    }

private:
    size_type _precision = 32; // bits, including the sign
    container_type digits;

};


#endif //LMPA_LIBRARY_BINARY_H
