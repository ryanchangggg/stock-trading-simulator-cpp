#ifndef STOCK_TRADING_LIMIT_ORDER_EXECUTOR_H
#define STOCK_TRADING_LIMIT_ORDER_EXECUTOR_H

#include "OrderExecutionStrategy.h"

namespace stock_trading {

class LimitOrderExecutor : public OrderExecutionStrategy {
public:
    ExecutionResult execute(Order& order, User& user, Market& market, long nextTradeId) override;
};

}  // namespace stock_trading

#endif
