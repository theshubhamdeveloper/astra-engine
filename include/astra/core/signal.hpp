#pragma once

#include <functional>
#include <vector>

#include <astra/core/assert.hpp>

namespace astra::core {
    struct Connection {
        uint32_t id;
    };

    template<typename... Args>
    class SignalView {
    public:
        using Callback = std::function<void(Args...)>;

        Connection connect(Callback callback) {
            connections.emplace_back(callback);
            return Connection{static_cast<uint32_t>(connections.size() - 1)};
        }

        void disconnect(const Connection &connection) {
            ASSERT(connection.id < connections.size() - 1);
            connections.erase(connections.begin() + connection);
        }

        size_t size() {
            return connections.size();
        }
    protected:
        std::vector<Callback> connections;
    };

    template<typename... Args>
    class Signal : public SignalView<Args...> {
    public:
        void emit(Args... args) {
            for (const auto &connection: this->connections) {
                connection(args...);
            }
        }
    };
}
