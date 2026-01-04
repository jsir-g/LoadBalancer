// LoadBalancer.cpp
#include "LoadBalancer.h"
#include <iostream>
#include <algorithm>

LoadBalancer::LoadBalancer(RequestGenerator generator,
                           std::size_t initial_servers,
                           int cooldown_ticks)
    : gen_(std::move(generator)),
      queue_(),
      servers_(initial_servers),
      clock_(0),
      cooldown_ticks_(std::max(0, cooldown_ticks)),
      cooldown_remaining_(0),
      total_generated_(0),
      total_completed_(0),
      total_wait_time_(0),
      max_queue_size_(0),
      scale_up_events_(0),
      scale_down_events_(0) {}

void LoadBalancer::prefill(std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        Request r = gen_.generateRequest(0);
        queue_.enqueue(r);
        total_generated_++;
    }
    max_queue_size_ = std::max(max_queue_size_, queue_.size());
}

void LoadBalancer::run(int duration) {
    for (clock_ = 0; clock_ < duration; ++clock_) {
        tickOnce();
    }

    // Summary
    std::cout << "\n=== Simulation Summary ===\n";
    std::cout << "Ticks run: " << duration << "\n";
    std::cout << "Servers final: " << servers_.size() << "\n";
    std::cout << "Generated: " << total_generated_ << "\n";
    std::cout << "Completed: " << total_completed_ << "\n";
    std::cout << "Max queue size: " << max_queue_size_ << "\n";
    std::cout << "Scale-up events: " << scale_up_events_ << "\n";
    std::cout << "Scale-down events: " << scale_down_events_ << "\n";
    if (total_completed_ > 0) {
        double avg_wait = static_cast<double>(total_wait_time_) / static_cast<double>(total_completed_);
        std::cout << "Avg wait time (ticks): " << avg_wait << "\n";
    }
}

void LoadBalancer::tickOnce() {
    generateArrivals();
    serviceServers();
    dispatchRequests();
    scaleIfNeeded();

    max_queue_size_ = std::max<std::size_t>(max_queue_size_, queue_.size());
    if (cooldown_remaining_ > 0) cooldown_remaining_--;
}

void LoadBalancer::generateArrivals() {
    // One-arrival-per-tick model (simple). You can extend to multiple arrivals.
    if (gen_.hasArrival()) {
        Request r = gen_.generateRequest(clock_);
        queue_.enqueue(r);
        total_generated_++;
    }
}

void LoadBalancer::serviceServers() {
    for (auto& s : servers_) {
        s.tick();
        auto done = s.takeFinished();
        if (done.has_value()) {
            total_completed_++;
            // wait time = time started - arrival. We don't store start time, so approximate as:
            // time in system before completion minus service_time isn't tracked here.
            // Simple: count queue wait only when dispatched (below). We'll compute there instead.
            // For now, do nothing here.
        }
    }
}

void LoadBalancer::dispatchRequests() {
    for (auto& s : servers_) {
        if (queue_.isEmpty()) break;
        if (s.isIdle()) {
            Request r = queue_.dequeue();
            // queue wait time until dispatch
            if (clock_ >= r.arrival_time) {
                total_wait_time_ += static_cast<unsigned long>(clock_ - r.arrival_time);
            }
            s.assign(r);
        }
    }
}

void LoadBalancer::scaleIfNeeded() {
    const std::size_t S = servers_.size();
    const std::size_t Q = queue_.size();

    if (S == 0) {
        // safety: if someone started with 0 servers, add one.
        servers_.emplace_back();
        cooldown_remaining_ = cooldown_ticks_;
        scale_up_events_++;
        return;
    }

    if (cooldown_remaining_ > 0) return;

    // Scale up: Q > 25 * S
    if (Q > 25 * S) {
        servers_.emplace_back();
        cooldown_remaining_ = cooldown_ticks_;
        scale_up_events_++;
        return;
    }

    // Scale down: Q < 15 * S (remove one idle server if possible, keep at least 1 server)
    if (S > 1 && Q < 15 * S) {
        if (removeOneIdleServer()) {
            cooldown_remaining_ = cooldown_ticks_;
            scale_down_events_++;
        }
    }
}

std::size_t LoadBalancer::idleServerCount() const {
    std::size_t count = 0;
    for (const auto& s : servers_) {
        if (s.isIdle()) count++;
    }
    return count;
}

bool LoadBalancer::removeOneIdleServer() {
    // Remove from the end if possible, else scan.
    for (std::size_t i = servers_.size(); i-- > 0;) {
        if (servers_[i].isIdle()) {
            servers_.erase(servers_.begin() + static_cast<long>(i));
            return true;
        }
    }
    return false; // no idle server to remove safely
}
