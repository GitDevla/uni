#include <iostream>

using namespace std;
int main() {
    int mb = 1024 * 1024;
    void *p = malloc(mb);
    while (true) {
        p = malloc(mb);
        if (p == nullptr)
            break;
    }
    while (true) {
    }

    return 0;
}