#include "model/order/BuyOrder.h"
#include "model/exception/InvalidOrderException.h"

namespace stock_trading {

BuyOrder::BuyOrder(long id, long userId, std::string symbol,
                   int quantity, double price, OrderType orderType)
    : Order(id, userId, std::move(symbol), quantity, price, orderType, TradeType::BUY) {}

void BuyOrder::validate() {
    if (getOrderType() == OrderType::LIMIT && getPrice() <= 0) {
        throw InvalidOrderException("Limit buy order must have a positive limit price");
    }
}

std::string BuyOrder::getDescription() const {
    return (getOrderType() == OrderType::MARKET ? "Market" : "Limit") + std::string(" Buy");
}

}  // namespace stock_trading
