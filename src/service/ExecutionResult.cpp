#include "service/ExecutionResult.h"
namespace stock_trading {
ExecutionResult ExecutionResult::filled(Order& order, std::unique_ptr<Trade> trade) {
    return {true, "Order filled: " + order.getDescription(),
            std::move(trade), &order, OrderStatus::FILLED};
}
ExecutionResult ExecutionResult::pending(Order& order) {
    return {true, "Order placed and pending: " + order.getDescription(),
            nullptr, &order, OrderStatus::PENDING};
}
ExecutionResult ExecutionResult::rejected(Order& order, const std::string& reason) {
    return {false, "Order rejected: " + reason,
            nullptr, &order, OrderStatus::CANCELLED};
}
ExecutionResult ExecutionResult::cancelled(Order& order) {
    return {true, "Order cancelled: " + order.getDescription(),
            nullptr, &order, OrderStatus::CANCELLED};
}
}  // namespace stock_trading
