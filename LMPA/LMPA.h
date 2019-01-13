//
// Created by Lars on 14/12/2018.
//

#ifndef MULTIPLEPRECISION_H
#define MULTIPLEPRECISION_H

#include <vector>
#include <cstddef> // size_t
#include <iostream> // operator<< stream overload



struct Binary {
private:
    std::size_t _precision = 32; // bits, including the sign
    std::vector<bool> digits;

public:

    /// Constructors ///
    Binary() noexcept;
    explicit Binary(std::size_t precision) noexcept;
    explicit Binary(const std::vector<bool>& d) noexcept;
    explicit Binary(const std::vector<bool>& d, int sgn) noexcept(false);
    explicit Binary(const std::vector<bool>& d, bool sgn) noexcept;

    Binary(const Binary& b) = default;
    Binary(Binary&& b) noexcept = default;

    ~Binary() = default;


    /// Utility ///
    inline bool sign() const { return digits[0]; }
    inline std::size_t precision() const { return _precision; }
    void set_precision(std::size_t prec);
    void shrink_to_fit();
    void reserve(std::size_t n);


    /// Assignment ///
    Binary& operator=(const Binary& b);
    Binary& operator=(Binary&& b) noexcept;
    Binary& operator+=(const Binary& b);
    Binary& operator-=(const Binary& b);
    Binary& operator*=(const Binary& b);
    Binary& operator/=(const Binary& b);
    Binary& operator%=(const Binary& b);
    // shift assignment
    Binary& operator<<=(const std::size_t n);
    Binary& operator>>=(const std::size_t n);

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
    // bitwise logical -- Consider returning Binaries here instead of bool
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
        for (const bool b : digits) {
            std::cout << b << ", ";
        }
        std::cout << std::endl;
    }

};

class LMPA {
private:

    std::vector<bool> pre_digits;
    std::vector<bool> post_digits;
    // 0 for integer-only
    std::size_t precision = 0;

    int sign = 1;

    int toDigit(char a);
    char toChar(int a);
    std::vector<bool> toBinary(const std::string& s);
    std::string toString(const std::vector<bool>& v);

    int insertDigit(std::string& result, int digit);

    static bool checkString(const std::string& s);

public:

    LMPA() noexcept;
    explicit LMPA(std::string n, std::size_t prec) noexcept(false);
    explicit LMPA(std::vector<bool> n) noexcept;
    explicit LMPA(std::vector<bool> pre, std::vector<bool> post, std::size_t prec) noexcept;
    LMPA(const LMPA& m);

    /* TODO:
     *  Multiplication
     *  Division
     *
     */

    /// Arithmetic ///
    LMPA operator+() const;
    LMPA operator-() const;

    LMPA operator+(const LMPA& m);
    LMPA operator-(const LMPA& m);
    LMPA operator*(const LMPA& m);
    LMPA operator/(const LMPA& m);
    LMPA operator%(const LMPA& m);


    /// Assignment ///
    LMPA& operator=(const LMPA& m);
    LMPA& operator+=(const LMPA& m);
    LMPA& operator-=(const LMPA& m);
    LMPA& operator*=(const LMPA& m);
    LMPA& operator/=(const LMPA& m);
    LMPA& operator%=(const LMPA& m);

    /// Logical ///
    bool operator==(const LMPA& m);
    bool operator!=(const LMPA& m);
    bool operator<(const LMPA& m);
    bool operator>(const LMPA& m);
    bool operator<=(const LMPA& m);
    bool operator>=(const LMPA& m);


    LMPA absVal();

    std::size_t getPrecision() { return precision; }

    void setPrecision(std::size_t prec);

    std::string getString() const;

    int getSign() { return sign; }

    friend std::ostream& operator<< (std::ostream& stream, const LMPA& m);

};


#endif //MULTIPLEPRECISION_H
