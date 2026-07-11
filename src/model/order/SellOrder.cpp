#include "model/order/SellOrder.h"
#include "model/exception/InvalidOrderException.h"

namespace stock_trading {

SellOrder::SellOrder(long id, long userId, std::string symbol,
                     int quantity, double price, OrderType orderType)
    : Order(id, userId, std::move(symbol), quantity, price, orderType, TradeType::SELL) {}

void SellOrder::validate() {
    if (getOrderType() == OrderType::LIMIT && getPrice() <= 0) {
        throw InvalidOrderException("Limit sell order must have a positive limit price");
    }
}

std::string SellOrder::getDescription() const {
    return (getOrderType() == OrderType::MARKET ? "Market" : "Limit") + std::string(" Sell");
}

}  // namespace stock_trading
