#ifndef STOCK_TRADING_EXECUTION_RESULT_H
#define STOCK_TRADING_EXECUTION_RESULT_H
#include "model/order/Order.h"
#include "model/order/OrderStatus.h"
#include "model/trade/Trade.h"
#include <memory>
#include <string>
#include <optional>
namespace stock_trading {
struct ExecutionResult {
    bool success;
    std::string message;
    std::unique_ptr<Trade> trade;
    Order* order;
    OrderStatus finalStatus;
    static ExecutionResult filled(Order& order, std::unique_ptr<Trade> trade);
    static ExecutionResult pending(Order& order);
    static ExecutionResult rejected(Order& order, const std::string& reason);
    static ExecutionResult cancelled(Order& order);
};
}  // namespace stock_trading
#endif
