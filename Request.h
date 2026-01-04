// Request.h
#pragma once
#include <string>

/**
 * @file Request.h
 * @brief Lightweight data container representing a web request.
 */
struct Request {
    std::string ip_in;
    std::string ip_out;
    int service_time = 0;     ///< Total required processing time (ticks)
    int remaining_time = 0;   ///< Remaining processing time (ticks)
    char job_type = 'P';      ///< 'S' (streaming) or 'P' (processing)
    int arrival_time = 0;     ///< Simulation tick when request arrived
    unsigned long id = 0;     ///< Unique request id (for debugging/stats)
};
