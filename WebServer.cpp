// WebServer.cpp
#include "WebServer.h"
#include <stdexcept>

WebServer::WebServer() : busy_(false), current_{}, has_finished_(false) {}

bool WebServer::isIdle() const {
    return !busy_;
}

void WebServer::assign(const Request& r) {
    if (busy_) {
        throw std::runtime_error("WebServer::assign() called while busy");
    }
    current_ = r;
    busy_ = true;
    has_finished_ = false;
}

bool WebServer::tick() {
    if (!busy_) return false;

    if (current_.remaining_time > 0) {
        current_.remaining_time -= 1;
    }

    if (current_.remaining_time <= 0) {
        has_finished_ = true;
        // remain busy_ until takeFinished() is called (LB controls transitions)
        return true;
    }
    return false;
}

std::optional<Request> WebServer::takeFinished() {
    if (!busy_ || !has_finished_) return std::nullopt;

    Request done = current_;
    busy_ = false;
    has_finished_ = false;
    current_ = Request{};
    return done;
}

int WebServer::remainingTime() const {
    return busy_ ? current_.remaining_time : 0;
}
