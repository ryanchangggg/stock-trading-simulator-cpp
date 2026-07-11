#ifndef STOCK_TRADING_STOP_LOSS_EXECUTOR_H
#define STOCK_TRADING_STOP_LOSS_EXECUTOR_H

#include "OrderExecutionStrategy.h"

namespace stock_trading {

class StopLossExecutor : public OrderExecutionStrategy {
public:
    ExecutionResult execute(Order& order, User& user, Market& market, long nextTradeId) override;
};

}  // namespace stock_trading

#endif
