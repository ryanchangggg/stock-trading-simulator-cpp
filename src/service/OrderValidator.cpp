#include "service/OrderValidator.h"
#include "model/trade/TradeType.h"
#include <cmath>
namespace stock_trading {
void OrderValidator::validate(Order& order, User& user, Market& market) {
    order.validate();
    auto stock = market.getStock(order.getSymbol());
    if (!stock) {
        throw InvalidOrderException("Stock not found in market: " + order.getSymbol());
    }
    if (order.getTradeType() == TradeType::BUY) {
        double currentPrice = market.getCurrentPrice(order.getSymbol());
        double totalCost = currentPrice * order.getQuantity();
        if (!user.canAfford(totalCost)) {
            throw InvalidOrderException("Insufficient funds: cash="
                + std::to_string(user.getCashBalance()) + ", required=" + std::to_string(totalCost));
        }
    } else {
        if (!user.holdsAtLeast(order.getSymbol(), order.getQuantity())) {
            throw InvalidOrderException("Insufficient shares: symbol="
                + order.getSymbol() + ", requested=" + std::to_string(order.getQuantity()));
        }
    }
}
}  // namespace stock_trading
