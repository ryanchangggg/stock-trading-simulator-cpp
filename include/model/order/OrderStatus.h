#ifndef STOCK_TRADING_ORDER_STATUS_H
#define STOCK_TRADING_ORDER_STATUS_H

#include <string>

namespace stock_trading {

enum class OrderStatus {
    PENDING,
    FILLED,
    CANCELLED
};

inline std::string orderStatusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::FILLED: return "FILLED";
        case OrderStatus::CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

}  // namespace stock_trading

#endif
