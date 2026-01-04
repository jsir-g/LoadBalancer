// LoadBalancer.h
#pragma once
#include "RequestQueue.h"
#include "RequestGenerator.h"
#include "WebServer.h"
#include <vector>
#include <cstddef>

/**
 * @file LoadBalancer.h
 * @brief Manages a queue of Requests and a pool of WebServers with scaling policy.
 */
class LoadBalancer {
public:
    /**
     * @param generator RequestGenerator used to create new traffic.
     * @param initial_servers Starting number of web servers.
     * @param cooldown_ticks Minimum ticks between scaling actions.
     */
    LoadBalancer(RequestGenerator generator,
                 std::size_t initial_servers,
                 int cooldown_ticks);

    /**
     * @brief Prefill the queue with count initial requests at time 0.
     */
    void prefill(std::size_t count);

    /**
     * @brief Run simulation for duration ticks.
     */
    void run(int duration);

private:
    RequestGenerator gen_;
    RequestQueue queue_;
    std::vector<WebServer> servers_;

    int clock_;
    int cooldown_ticks_;
    int cooldown_remaining_;

    // stats (optional but useful)
    unsigned long total_generated_;
    unsigned long total_completed_;
    unsigned long total_wait_time_;
    std::size_t max_queue_size_;
    unsigned long scale_up_events_;
    unsigned long scale_down_events_;

    void tickOnce();
    void generateArrivals();
    void serviceServers();
    void dispatchRequests();
    void scaleIfNeeded();

    std::size_t idleServerCount() const;
    bool removeOneIdleServer(); // scale-down safely
};
