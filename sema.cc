#include "sema.hpp"

void test() {
    ipcsema a;

    child c([&a]() {
        puts("Child");

        sleep(1);

        puts("Child exit");
    });

    puts("Parent");

    c.wait();

    puts("Parent: Child exits");
}

int main() {
    test();
    
    puts("After test()");
}
