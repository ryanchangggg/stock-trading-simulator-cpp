#ifndef STOCK_TRADING_ORDER_EXECUTION_STRATEGY_H
#define STOCK_TRADING_ORDER_EXECUTION_STRATEGY_H

#include "model/order/Order.h"
#include "model/user/User.h"
#include "model/market/Market.h"

namespace stock_trading {

struct ExecutionResult;

class OrderExecutionStrategy {
public:
    virtual ~OrderExecutionStrategy() = default;
    virtual ExecutionResult execute(Order& order, User& user, Market& market, long nextTradeId) = 0;
};

}  // namespace stock_trading

#endif
