// main.cpp
#include "LoadBalancer.h"
#include <iostream>
#include <limits>

static int readInt(const char* prompt, int min_val) {
    int x;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x && x >= min_val) return x;
        std::cout << "Please enter an integer >= " << min_val << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    int numServers = readInt("Enter initial number of servers: ", 1);
    int duration   = readInt("Enter duration (ticks): ", 1);
    int cooldownN  = readInt("Enter cooldown ticks between scaling actions: ", 0);

    // Generator: you can tune arrival_percent to simulate heavier traffic.
    RequestGenerator gen(/*seed=*/12345u, /*arrival_percent=*/35);

    LoadBalancer lb(gen, static_cast<std::size_t>(numServers), cooldownN);

    // Prefill queue = servers * 20
    lb.prefill(static_cast<std::size_t>(numServers) * 20);

    lb.run(duration);
    return 0;
}
