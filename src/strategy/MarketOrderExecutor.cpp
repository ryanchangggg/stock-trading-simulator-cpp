#include "strategy/MarketOrderExecutor.h"
#include "service/ExecutionResult.h"
#include "model/trade/Trade.h"
namespace stock_trading {
ExecutionResult MarketOrderExecutor::execute(Order& order, User& user, Market& market, long nextTradeId) {
    std::string symbol = order.getSymbol();
    int quantity = order.getQuantity();
    double currentPrice = market.getCurrentPrice(symbol);
    double totalValue = currentPrice * quantity;
    if (order.getTradeType() == TradeType::BUY) {
        user.withdraw(totalValue);
        user.getPortfolio().addShares(symbol, quantity, currentPrice);
        order.fill();
        auto trade = std::make_unique<Trade>(nextTradeId, user.getId(), symbol, TradeType::BUY, quantity, currentPrice);
        return ExecutionResult::filled(order, std::move(trade));
    } else {
        user.getPortfolio().removeShares(symbol, quantity);
        user.deposit(totalValue);
        order.fill();
        auto trade = std::make_unique<Trade>(nextTradeId, user.getId(), symbol, TradeType::SELL, quantity, currentPrice);
        return ExecutionResult::filled(order, std::move(trade));
    }
}
}  // namespace stock_trading
