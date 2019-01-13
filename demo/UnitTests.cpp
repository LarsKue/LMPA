//
// Created by Lars on 13/01/2019.
//

#include "UnitTests.h"
#include "../LMPA/Binary.h"

#include <cassert>

void UnitTests::run() {
    assert(Invert());
    std::cout << "Successfully Passed Test Invert" << std::endl;
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
    assert(PreIncrement());
    std::cout << "Successfully Passed Test PreIncrement" << std::endl;
    assert(PostIncrement());
    std::cout << "Successfully Passed Test PostIncrement" << std::endl;
    assert(PreDecrement());
    std::cout << "Successfully Passed Test PreDecrement" << std::endl;
    assert(PostDecrement());
    std::cout << "Successfully Passed Test PostDecrement" << std::endl;
    assert(SmallerThan());
    std::cout << "Successfully Passed Test SmallerThan" << std::endl;


    assert(Other());
    std::cout << "Successfully Passed Test Other" << std::endl;


    std::cout << "----------------------------------------------------------------" << std::endl;
    std::cout << "All Tests Passed Successfully." << std::endl;
}

bool UnitTests::Invert() {
    Binary a({0, 1, 1, 0, 1, 0, 0, 1});
    Binary result({1, 0, 0, 1, 0, 1, 1, 1});

    return (-a) == result;
}

bool UnitTests::Plus() {
    Binary a({0, 0, 0, 1, 1, 0, 1, 1}); // 27
    Binary b({0, 1, 0, 0, 1, 1, 1, 0}); // 78

    Binary result({0, 1, 1, 0, 1, 0, 0, 1}); // 27 + 78 = 105

    return a + b == result;
}

bool UnitTests::PlusEquals() {
    Binary a({0, 0, 0, 1, 1, 0, 1, 1}); // 27
    Binary b({0, 1, 0, 0, 1, 1, 1, 0}); // 78

    Binary result({0, 1, 1, 0, 1, 0, 0, 1}); // 27 + 78 = 105

    a += b;
    return a == result;
}

bool UnitTests::Minus() {
    Binary a({0, 0, 0, 1, 1, 0, 1, 1}); // 27
    Binary b({0, 1, 0, 0, 1, 1, 1, 0}); // 78

    Binary result({1, 1, 0, 0, 1, 1, 0, 1}); // 27 - 78 = -51

    return a - b == result;
}

bool UnitTests::MinusEquals() {
    Binary a({0, 0, 0, 1, 1, 0, 1, 1}); // 27
    Binary b({0, 1, 0, 0, 1, 1, 1, 0}); // 78

    Binary result({1, 1, 0, 0, 1, 1, 0, 1}); // 27 - 78 = -51

    a -= b;
    return a == result;
}

bool UnitTests::Times() {
    Binary a({0, 0, 0, 0, 1, 1, 0, 1, 1, 1}); // 55
    Binary b({0, 0, 0, 0, 0, 0, 0, 1, 1, 0}); // 6

    Binary result({0, 1, 0, 1, 0, 0, 1, 0, 1, 0}); // 55 * 6 = 330
    return a * b == result;
}

bool UnitTests::TimesEquals() {
    Binary a({0, 0, 0, 0, 1, 1, 0, 1, 1, 1}); // 55
    Binary b({0, 0, 0, 0, 0, 0, 0, 1, 1, 0}); // 6

    Binary result({0, 1, 0, 1, 0, 0, 1, 0, 1, 0}); // 55 * 6 = 330

    a *= b;
    return a == result;
}

bool UnitTests::Divide() {
    // TODO: Implement
    return true;
}

bool UnitTests::DivideEquals() {
    // TODO: Implement
    return true;
}

bool UnitTests::PreIncrement() {
    Binary a({0, 0, 0, 1, 0, 1, 1, 1}); // 23
    Binary b({0, 0, 0, 1, 1, 0, 0, 0}); // 24

    return ++a == b;
}

bool UnitTests::PostIncrement() {
    Binary a({0, 0, 0, 1, 0, 1, 1, 1}); // 23
    Binary b({0, 0, 0, 1, 0, 1, 1, 1}); // still 23
    Binary c({0, 0, 0, 1, 1, 0, 0, 0}); // 24

    return a++ == b && a == c;
}

bool UnitTests::PreDecrement() {
    Binary a({0, 0, 0, 1, 0, 1, 1, 1}); // 23
    Binary b({0, 0, 0, 1, 0, 1, 1, 0}); // 22

    return --a == b;
}

bool UnitTests::PostDecrement() {
    Binary a({0, 0, 0, 1, 0, 1, 1, 1}); // 23
    Binary b({0, 0, 0, 1, 0, 1, 1, 1}); // still 23
    Binary c({0, 0, 0, 1, 0, 1, 1, 0}); // 22

    return a-- == b && a == c;
}

bool UnitTests::SmallerThan() {
    Binary a({0, 0, 0, 1, 0, 1, 1, 1}); // 23
    Binary b({0, 0, 0, 1, 1, 0, 0, 0}); // 24

    return a < b;
}

bool UnitTests::Other() {
    // dynamically test += vs * etc.
    Binary a({1, 0}); // 2
    Binary b({0, 1}); // 1
    Binary result({1, 0}); // 2
    a.reserve(256);
    b.reserve(256);
    result.reserve(256);

    for (std::size_t i = 0; i < 1000; ++i) {
        if (a * b != result) { return false; }
        ++b;
        result += a;
    }

    return true;
}
