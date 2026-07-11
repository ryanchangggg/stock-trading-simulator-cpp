#include "strategy/LimitOrderExecutor.h"
#include "service/ExecutionResult.h"
#include "model/trade/Trade.h"
#include "model/exception/InsufficientFundsException.h"
#include "model/exception/InvalidOrderException.h"
namespace stock_trading {
ExecutionResult LimitOrderExecutor::execute(Order& order, User& user, Market& market, long nextTradeId) {
    std::string symbol = order.getSymbol();
    double currentPrice = market.getCurrentPrice(symbol);
    double limitPrice = order.getPrice();
    bool canFill;
    if (order.getTradeType() == TradeType::BUY) {
        canFill = currentPrice <= limitPrice;
    } else {
        canFill = currentPrice >= limitPrice;
    }
    if (!canFill) return ExecutionResult::pending(order);
    int quantity = order.getQuantity();
    double fillPrice = currentPrice;
    try {
        if (order.getTradeType() == TradeType::BUY) {
            user.withdraw(fillPrice * quantity);
            user.getPortfolio().addShares(symbol, quantity, fillPrice);
        } else {
            user.getPortfolio().removeShares(symbol, quantity);
            user.deposit(fillPrice * quantity);
        }
    } catch (const InsufficientFundsException& e) {
        order.cancel();
        return ExecutionResult::rejected(order, "Limit order cancelled: " + std::string(e.what()));
    } catch (const InvalidOrderException& e) {
        order.cancel();
        return ExecutionResult::rejected(order, "Limit order cancelled: " + std::string(e.what()));
    }
    order.fill();
    auto trade = std::make_unique<Trade>(nextTradeId, user.getId(), symbol, order.getTradeType(), quantity, fillPrice);
    return ExecutionResult::filled(order, std::move(trade));
}
}  // namespace stock_trading
