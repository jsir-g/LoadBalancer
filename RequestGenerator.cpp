// RequestGenerator.cpp
#include "RequestGenerator.h"

RequestGenerator::RequestGenerator(unsigned int seed, int arrival_percent)
    : rng_(seed),
      percent_(0, 99),
      arrival_percent_(arrival_percent),
      octet_(0, 255),
      job_coin_(0.50),        // 50/50 S vs P (change if desired)
      s_time_(1, 5),
      p_time_(3, 10),
      next_id_(1) {
    if (arrival_percent_ < 0) arrival_percent_ = 0;
    if (arrival_percent_ > 100) arrival_percent_ = 100;
}

bool RequestGenerator::hasArrival() {
    return percent_(rng_) < arrival_percent_;
}

Request RequestGenerator::generateRequest(int now_tick) {
    Request r;
    r.id = next_id_++;
    r.arrival_time = now_tick;

    r.ip_in = randomIP();
    r.ip_out = randomIP();

    bool is_streaming = job_coin_(rng_);
    r.job_type = is_streaming ? 'S' : 'P';

    r.service_time = is_streaming ? s_time_(rng_) : p_time_(rng_);
    r.remaining_time = r.service_time;

    return r;
}

std::string RequestGenerator::randomIP() {
    // Simple dotted-quad (doesn't try to exclude private ranges, etc.)
    return std::to_string(octet_(rng_)) + "." +
           std::to_string(octet_(rng_)) + "." +
           std::to_string(octet_(rng_)) + "." +
           std::to_string(octet_(rng_));
}
