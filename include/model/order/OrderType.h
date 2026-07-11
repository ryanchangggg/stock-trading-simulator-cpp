#ifndef STOCK_TRADING_ORDER_TYPE_H
#define STOCK_TRADING_ORDER_TYPE_H

#include <string>

namespace stock_trading {

enum class OrderType {
    MARKET,
    LIMIT,
    STOP_LOSS
};

inline std::string orderTypeToString(OrderType t) {
    switch (t) {
        case OrderType::MARKET: return "MARKET";
        case OrderType::LIMIT: return "LIMIT";
        case OrderType::STOP_LOSS: return "STOP_LOSS";
    }
    return "UNKNOWN";
}

}  // namespace stock_trading

#endif
