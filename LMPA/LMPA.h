//
// Created by Lars on 14/12/2018.
//

#ifndef MULTIPLEPRECISION_H
#define MULTIPLEPRECISION_H

#include <string>
#include <cstddef>
#include <iostream>

class LMPA {
private:

    std::string pre_digits;
    std::string post_digits;
    std::size_t pre_length = 1;
    // 0 for integer-only
    std::size_t precision = 0;

    int sign = 1;

    int toDigit(char a);
    char toChar(int a);

    int insertDigit(std::string& result, int digit);

    static bool checkString(const std::string& s);

public:

    LMPA() noexcept;
    explicit LMPA(std::string n, std::size_t prec) noexcept(false);
    LMPA(const LMPA& m);

    /* TODO:
     *  Floating Point Support
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
