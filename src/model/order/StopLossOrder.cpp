#include "model/order/StopLossOrder.h"
#include "model/exception/InvalidOrderException.h"
#include <stdexcept>

namespace stock_trading {

StopLossOrder::StopLossOrder(long id, long userId, std::string symbol,
                             int quantity, double stopPrice, TradeType tradeType)
    : Order(id, userId, std::move(symbol), quantity, 0.0, OrderType::STOP_LOSS, tradeType)
    , stopPrice_(stopPrice) {
    if (stopPrice <= 0) throw std::invalid_argument("Stop price must be positive");
}

void StopLossOrder::trigger() {
    if (getStatus() != OrderStatus::PENDING) {
        throw std::logic_error("Cannot trigger an order with status: " + orderStatusToString(getStatus()));
    }
    triggered_ = true;
}

bool StopLossOrder::shouldTrigger(double marketPrice) const {
    if (getTradeType() == TradeType::SELL) {
        return marketPrice <= stopPrice_;
    } else {
        return marketPrice >= stopPrice_;
    }
}

void StopLossOrder::validate() {
    if (stopPrice_ <= 0) {
        throw InvalidOrderException("Stop-loss order must have a positive stop price");
    }
}

std::string StopLossOrder::getDescription() const {
    return "Stop-Loss " + tradeTypeToString(getTradeType());
}

}  // namespace stock_trading
