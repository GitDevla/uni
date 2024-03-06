#include <iostream>
#include <thread>

using namespace std;
void killCore() {
    while (true) {
    }
}

int main() {
    unsigned int num_of_cores = std::thread::hardware_concurrency();
    for (unsigned int i = 0; i < num_of_cores - 1; ++i) {
        thread t = thread(killCore);
        t.detach();
    }
    killCore();
}