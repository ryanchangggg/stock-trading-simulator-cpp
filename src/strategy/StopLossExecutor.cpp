#include "strategy/StopLossExecutor.h"
#include "service/ExecutionResult.h"
#include "model/order/StopLossOrder.h"
#include "model/trade/Trade.h"
#include "model/exception/InsufficientFundsException.h"
#include "model/exception/InvalidOrderException.h"
namespace stock_trading {
ExecutionResult StopLossExecutor::execute(Order& order, User& user, Market& market, long nextTradeId) {
    auto& stopOrder = dynamic_cast<StopLossOrder&>(order);
    std::string symbol = stopOrder.getSymbol();
    double currentPrice = market.getCurrentPrice(symbol);
    if (!stopOrder.isTriggered()) {
        if (!stopOrder.shouldTrigger(currentPrice)) return ExecutionResult::pending(stopOrder);
        stopOrder.trigger();
    }
    int quantity = stopOrder.getQuantity();
    double fillPrice = currentPrice;
    try {
        if (stopOrder.getTradeType() == TradeType::SELL) {
            user.getPortfolio().removeShares(symbol, quantity);
            user.deposit(fillPrice * quantity);
        } else {
            user.withdraw(fillPrice * quantity);
            user.getPortfolio().addShares(symbol, quantity, fillPrice);
        }
    } catch (const InsufficientFundsException& e) {
        stopOrder.cancel();
        return ExecutionResult::rejected(stopOrder, "Stop-loss cancelled: " + std::string(e.what()));
    } catch (const InvalidOrderException& e) {
        stopOrder.cancel();
        return ExecutionResult::rejected(stopOrder, "Stop-loss cancelled: " + std::string(e.what()));
    }
    stopOrder.fill();
    auto trade = std::make_unique<Trade>(nextTradeId, user.getId(), symbol, stopOrder.getTradeType(), quantity, fillPrice);
    return ExecutionResult::filled(stopOrder, std::move(trade));
}
}  // namespace stock_trading
