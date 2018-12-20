//
// Created by Lars on 16/12/2018.
//
#include "../LMPA/LMPA.h"

#include <ctime>

int main() {

    std::vector<bool> avec;
    std::vector<bool> bvec;
    std::vector<bool> cvec;
    std::vector<bool> dvec;

    avec = {0, 1, 0, 1, 0};
    bvec = {1, 1, 0, 0, 1};

    cvec = {1, 0, 0, 0, 0};
    dvec = {0, 1, 0, 0, 1};

    Binary a(avec);
    Binary b(bvec);

    Binary c(cvec);
    Binary d(dvec);

    std::cout << a + b << "\t= 0b100011" << std::endl;
    std::cout << a - b << "\t= -0b01111" << std::endl;
    std::cout << std::endl;
    std::cout << c + d << "\t= 0b11001" <<std::endl;
    std::cout << c - d << "\t= 0b00111" << std::endl;


//    srand(time(0));

//    for (int i = 0; i < 100; ++i) {
//        avec.clear();
//        bvec.clear();
//        for (int j = 0; j < 5; ++j) {
//            avec.push_back(rand() % 2);
//            bvec.push_back(rand() % 2);
//        }
//        Binary a(avec);
//        Binary b(bvec);
//
//        std::cout << a << std::endl;
//        std::cout << b << std::endl;
//        std::cout << "a + b: " << a + b << std::endl;
//        std::cout << "a - b: " << a - b << std::endl;
//
//        std::cout << "--------------------------------------------------" << std::endl;
//    }

    return 0;
}
