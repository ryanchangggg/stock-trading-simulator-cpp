#ifndef STOCK_TRADING_BUY_ORDER_H
#define STOCK_TRADING_BUY_ORDER_H

#include "Order.h"

namespace stock_trading {

class BuyOrder : public Order {
public:
    BuyOrder(long id, long userId, std::string symbol,
             int quantity, double price, OrderType orderType);

    void validate() override;
    std::string getDescription() const override;
};

}  // namespace stock_trading

#endif
