// WebServer.h
#pragma once
#include "Request.h"
#include <optional>

/**
 * @file WebServer.h
 * @brief Represents one web server that processes one request at a time.
 */
class WebServer {
public:
    WebServer();

    /**
     * @brief Returns true if server is not currently processing a request.
     */
    bool isIdle() const;

    /**
     * @brief Assign a request to this server (server must be idle).
     */
    void assign(const Request& r);

    /**
     * @brief Advance one tick of processing.
     * @return true if a request finished during this tick.
     */
    bool tick();

    /**
     * @brief If a request finished, get it and clear internal state.
     * @return finished request (if available), otherwise std::nullopt.
     */
    std::optional<Request> takeFinished();

    /**
     * @brief Get remaining time (0 if idle).
     */
    int remainingTime() const;

private:
    bool busy_;
    Request current_;
    bool has_finished_;
};
