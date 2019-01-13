//
// Created by Lars on 13/01/2019.
//

#ifndef LMPA_LIBRARY_UNITTESTS_H
#define LMPA_LIBRARY_UNITTESTS_H

class UnitTests {
private:

    /// Arithmetic ///
    static bool Invert();
    static bool Plus();
    static bool PlusEquals();
    static bool Minus();
    static bool MinusEquals();
    static bool Times();
    static bool TimesEquals();
    static bool Divide();
    static bool DivideEquals();

    static bool PreIncrement();
    static bool PostIncrement();
    static bool PreDecrement();
    static bool PostDecrement();

    static bool SmallerThan();

    static bool Other();


public:

    static void run();

};


#endif //LMPA_LIBRARY_UNITTESTS_H
