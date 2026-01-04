// RequestGenerator.h
#pragma once
#include "Request.h"
#include <random>
#include <string>

/**
 * @file RequestGenerator.h
 * @brief Encapsulates all randomness for arrivals and request fields.
 */
class RequestGenerator {
public:
    /**
     * @brief Construct generator.
     * @param seed RNG seed (use fixed seed for reproducible tests).
     * @param arrival_percent Probability [0..100] of a request arriving per tick.
     */
    explicit RequestGenerator(unsigned int seed = std::random_device{}(),
                              int arrival_percent = 35);

    /**
     * @brief Decide whether one request arrives this tick.
     */
    bool hasArrival();

    /**
     * @brief Generate a new Request for the given simulation tick.
     */
    Request generateRequest(int now_tick);

private:
    std::mt19937 rng_;

    std::uniform_int_distribution<int> percent_;     // 0..99
    int arrival_percent_;                             // 0..100

    std::uniform_int_distribution<int> octet_;       // 0..255
    std::bernoulli_distribution job_coin_;           // choose S vs P

    // Service time distributions (tune as desired)
    std::uniform_int_distribution<int> s_time_;      // streaming: short
    std::uniform_int_distribution<int> p_time_;      // processing: longer

    unsigned long next_id_;

    std::string randomIP();
};
