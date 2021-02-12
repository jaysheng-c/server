#include "../jaysheng/thread.h"

#include <iostream>

void print() {
    std::cout << "Hello my thread test, print by std::cout.\n";
}

int main(int argc, char const *argv[]) {

    std::cout << "Start cell thread.\n";

    jaysheng::Thread thread(print, "print_test");
    thread.join();

    std::cout << "End cell thread.\n";

    return 0;
}
