#include "Class.h"
#include <iostream>

int main() {
    Matrix<int> m;
    std::cin>>m;
    m.Transpose();
    std::cout<<m;
    return 0;
}