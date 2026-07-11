#include "model/order/Order.h"
#include <stdexcept>
#include <ctime>

namespace stock_trading {

static std::string currentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

Order::Order(long id, long userId, std::string symbol, int quantity,
             double price, OrderType orderType, TradeType tradeType)
    : id_(id)
    , userId_(userId)
    , symbol_(std::move(symbol))
    , quantity_(quantity)
    , price_(price)
    , orderType_(orderType)
    , tradeType_(tradeType) {
    if (quantity <= 0) throw std::invalid_argument("Order quantity must be positive");
    if (price < 0) throw std::invalid_argument("Order price cannot be negative");
    if (symbol_.empty()) throw std::invalid_argument("Symbol must not be empty");
    createdAt_ = currentTimestamp();
}

void Order::fill() {
    if (status_ != OrderStatus::PENDING) {
        throw std::logic_error("Cannot fill an order with status: " + orderStatusToString(status_));
    }
    status_ = OrderStatus::FILLED;
    filledAt_ = currentTimestamp();
}

void Order::cancel() {
    if (status_ != OrderStatus::PENDING) {
        throw std::logic_error("Cannot cancel an order with status: " + orderStatusToString(status_));
    }
    status_ = OrderStatus::CANCELLED;
}

void Order::restoreStatus(OrderStatus status, const std::string& filledAt) {
    status_ = status;
    if (!filledAt.empty()) {
        filledAt_ = filledAt;
    }
}

}  // namespace stock_trading
