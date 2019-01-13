//
// Created by Lars on 14/12/2018.
//

#ifndef MULTIPLEPRECISION_H
#define MULTIPLEPRECISION_H

#include <vector> // container
#include <cstddef> // size_type
#include <iostream> // operator<< stream overload

class Binary;

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
