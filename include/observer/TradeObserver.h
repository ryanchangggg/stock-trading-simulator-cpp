#ifndef STOCK_TRADING_TRADE_OBSERVER_H
#define STOCK_TRADING_TRADE_OBSERVER_H

#include "model/order/Order.h"
#include "model/trade/Trade.h"

namespace stock_trading {

class TradeObserver {
public:
    virtual ~TradeObserver() = default;
    virtual void onTrade(const Trade& trade) {}
    virtual void onOrderStatusChange(const Order& order) {}
};

}  // namespace stock_trading

#endif
