//
// Created by Lars on 13/01/2019.
//

#include "UnitTests.h"
#include "../LMPA/Binary.h"

#include <cassert>

void UnitTests::run() {
    assert(SmallerThan());
    std::cout << "Successfully Passed Test SmallerThan" << std::endl;
    assert(Invert());
    std::cout << "Successfully Passed Test Invert" << std::endl;
    assert(AbsVal());
    std::cout << "Successfully Passed Test AbsVal" << std::endl;
    assert(Plus());
    std::cout << "Successfully Passed Test Plus" << std::endl;
    assert(PlusEquals());
    std::cout << "Successfully Passed Test PlusEquals" << std::endl;
    assert(Minus());
    std::cout << "Successfully Passed Test Minus" << std::endl;
    assert(MinusEquals());
    std::cout << "Successfully Passed Test MinusEquals" << std::endl;
    assert(Times());
    std::cout << "Successfully Passed Test Times" << std::endl;
    assert(TimesEquals());
    std::cout << "Successfully Passed Test TimesEquals" << std::endl;
    assert(Divide());
    std::cout << "Successfully Passed Test Divide" << std::endl;
    assert(DivideEquals());
    std::cout << "Successfully Passed Test DivideEquals" << std::endl;
    assert(Modulo());
    std::cout << "Successfully Passed Test Modulo" << std::endl;
    assert(ModuloEquals());
    std::cout << "Successfully Passed Test ModuloEquals" << std::endl;
    assert(PreIncrement());
    std::cout << "Successfully Passed Test PreIncrement" << std::endl;
    assert(PostIncrement());
    std::cout << "Successfully Passed Test PostIncrement" << std::endl;
    assert(PreDecrement());
    std::cout << "Successfully Passed Test PreDecrement" << std::endl;
    assert(PostDecrement());
    std::cout << "Successfully Passed Test PostDecrement" << std::endl;


    assert(Other());
    std::cout << "Successfully Passed Test Other" << std::endl;


    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "All Tests Passed Successfully." << std::endl;
}

bool UnitTests::Invert() {
    Binary a(43, true);
    Binary result(-43, true);

    return (-a) == result;
}

bool UnitTests::AbsVal() {
    Binary a(-43, true);
    Binary result(43, true);

    return (a.absVal()) == result;
}

bool UnitTests::Plus() {
    Binary a(27, true);
    Binary b(78, true);

    Binary result1(27 + 78, true);

    Binary c(-78, true);

    Binary result2(27 - 78, true);

    Binary d(-27, true);

    Binary result3(-27 + 78, true);

    Binary result4(-27 - 78, true);

    return (a + b) == result1 && (a + c) == result2
            && (d + b) == result3 && (d + c == result4);
}

bool UnitTests::PlusEquals() {
    Binary a(47, true);
    Binary b(65, true);

    Binary c(-47, true);
    Binary d(-65, true);

    Binary a2(47, true);


    Binary result1(47 + 65, true);
    Binary result2(-47 + 65, true);
    Binary result3(47 - 65, true);

    a += b;
    c += b;
    a2 += d;
    return a == result1 && c == result2 && a2 == result3;
}

bool UnitTests::Minus() {
    Binary a(78, true);
    Binary b(27, true);
    Binary c(-78, true);
    Binary d(-27, true);

    Binary result1(78 - 27, true);
    Binary result2(27 - 78, true);
    Binary result3(-27 - 78, true);
    Binary result4(-27 + 78, true);

    return (a - b) == result1 && (b - a) == result2 && (d - a) == result3 && (d - c) == result4;
}

bool UnitTests::MinusEquals() {
    Binary a(27, true);
    Binary b(78, true);

    Binary result(27 - 78, true);

    a -= b;
    return a == result;
}

bool UnitTests::Times() {
    Binary a(55, true);
    Binary b(6, true);

    Binary result(55 * 6, true);
    return a * b == result;
}

bool UnitTests::TimesEquals() {
    Binary a(55, true);
    Binary b(6, true);

    Binary result(55 * 6, true);

    a *= b;
    return a == result;
}

bool UnitTests::Divide() {
    // TODO: Expand

    Binary a(-55, true);
    Binary b(5, true);

    Binary result(-11, true);

    return a / b == result;
}

bool UnitTests::DivideEquals() {
    // TODO: Expand
    Binary a(55, true);
    Binary b(5, true);

    Binary result(11, true);
    a /= b;
    return a == result;
}

bool UnitTests::Modulo() {
    Binary a(56, true);
    Binary b(10, true);
    Binary result1(6, true);
    Binary result2(-6, true);
    // 56 % 10 = 6
    // -56 % 10 = -6
    // 56 % -10 = 6
    // -56 % -10 = -6

    if (!(a % b == result1)) { return false; }
    if (!((-a) % b == result2)) { ((-a) % b).print(); return false; }
    if (!(a % (-b) == result1)) { (a % (-b)).print(); return false; }
    if (!((-a) % (-b) == result2)) { ((-a) % (-b)).print(); return false; }
    return true;
}


bool UnitTests::ModuloEquals() {
    Binary a(57, true);
    Binary b(10, true);
    Binary a2(57, true);
    Binary b2(-10, true);
    Binary a3(-57, true);
    Binary a4(-57, true);
    Binary result1(7, true);
    Binary result2(-7, true);

    a %= b;
    a2 %= b2;
    a3 %= b;
    a4 %= b2;
    if (a != result1) { a.print(); return false; }
    if (a2 != result1) { a2.print(); return false; }
    if (a3 != result2) { a3.print(); return false; }
    if (a4 != result2) { a4.print(); return false; }

    return true;
}

bool UnitTests::PreIncrement() {
    Binary a(23, true);
    Binary b(23 + 1, true);

    return ++a == b;
}

bool UnitTests::PostIncrement() {
    Binary a(23, true);
    Binary b(23, true);
    Binary c(23 + 1, true);

    return a++ == b && a == c;
}

bool UnitTests::PreDecrement() {
    Binary a(23, true);
    Binary b(23 - 1, true);

    return --a == b;
}

bool UnitTests::PostDecrement() {
    Binary a(23, true);
    Binary b(23, true);
    Binary c(23 - 1, true);

    return a-- == b && a == c;
}

bool UnitTests::SmallerThan() {
    Binary a(23, true);
    Binary b(23 + 1, true);

    return a < b && !(b < a);
}

bool UnitTests::Other() {
    // dynamically test += vs * etc.
//    Binary a({0, 1, 0}); // 2
//    Binary b({0, 0, 1}); // 1
//    Binary result({0, 1, 0}); // 2
//    a.reserve(256);
//    b.reserve(256);
//    result.reserve(256);
//
//    for (std::size_t i = 0; i < 1000; ++i) {
//        if (a * b != result) { return false; }
//        ++b;
//        result += a;
//    }


    // dynamically test / vs *
    Binary a(2, true);
    Binary b(3, true);

    constexpr Binary::size_type prec = 512;
    a.reserve(prec);
    b.reserve(prec);

    // at least 2 extra bits necessary to hold the result from the multiplication of a and b
    for (std::size_t i = 0; i < prec - 3; ++i) {
        if (((a * b) / b) != a) {
            a.print();
            b.print();
            std::cout << "Test Failed at: i = " << i << std::endl;
            return false;
        }

        a *= Binary({0, 1, 0}); // 2
    }
    return true;
}
