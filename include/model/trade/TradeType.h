#ifndef STOCK_TRADING_TRADE_TYPE_H
#define STOCK_TRADING_TRADE_TYPE_H

#include <string>

namespace stock_trading {

enum class TradeType {
    BUY,
    SELL
};

inline std::string tradeTypeToString(TradeType t) {
    switch (t) {
        case TradeType::BUY: return "BUY";
        case TradeType::SELL: return "SELL";
    }
    return "UNKNOWN";
}

}  // namespace stock_trading

#endif
