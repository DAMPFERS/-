#include <iostream>
#include "tbb/tbb.h"

int main() {
    tbb::parallel_for(0, 100, [](int i) {
        std::cout << "Index: " << i << std::endl;
    });

    return 0;
}
