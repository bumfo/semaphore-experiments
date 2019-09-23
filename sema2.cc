#include "sema.hpp"

void test() {
    ipcsema a;

    child c([&a]() {
        puts("Child");
        
        while (1) {
            a.wait();

            puts("Child exec");
        }

        puts("Child exit");
    });

    puts("Parent");

    while (1) {
        sleep(1);
        puts("Parent exec");

        a.post();
    }

    c.wait();

    puts("Parent: Child exits");
}

int main() {
    test();
    
    puts("After test()");
}
