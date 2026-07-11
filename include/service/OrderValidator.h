#ifndef STOCK_TRADING_ORDER_VALIDATOR_H
#define STOCK_TRADING_ORDER_VALIDATOR_H
#include "model/order/Order.h"
#include "model/user/User.h"
#include "model/market/Market.h"
#include "model/exception/InvalidOrderException.h"
namespace stock_trading {
class OrderValidator {
public:
    void validate(Order& order, User& user, Market& market);
};
}  // namespace stock_trading
#endif
