#ifndef STOCK_TRADING_STOP_LOSS_ORDER_H
#define STOCK_TRADING_STOP_LOSS_ORDER_H

#include "Order.h"

namespace stock_trading {

class StopLossOrder : public Order {
public:
    StopLossOrder(long id, long userId, std::string symbol,
                  int quantity, double stopPrice, TradeType tradeType);

    double getStopPrice() const { return stopPrice_; }
    bool isTriggered() const { return triggered_; }
    void trigger();
    bool shouldTrigger(double marketPrice) const;

    void validate() override;
    std::string getDescription() const override;

private:
    double stopPrice_ = 0.0;
    bool triggered_ = false;
};

}  // namespace stock_trading

#endif
