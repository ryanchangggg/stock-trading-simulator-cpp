#ifndef STOCK_TRADING_TRADING_ENGINE_H
#define STOCK_TRADING_TRADING_ENGINE_H
#include "service/ExecutionResult.h"
#include "service/OrderBook.h"
#include "service/OrderValidator.h"
#include "strategy/OrderExecutionStrategy.h"
#include "strategy/MarketOrderExecutor.h"
#include "strategy/LimitOrderExecutor.h"
#include "strategy/StopLossExecutor.h"
#include "repository/OrderRepository.h"
#include "repository/TradeRepository.h"
#include "repository/UserRepository.h"
#include <memory>
#include <unordered_map>
#include <vector>
namespace stock_trading {
class TradingEngine {
public:
    TradingEngine(OrderRepository& orderRepo,
                  TradeRepository& tradeRepo,
                  UserRepository& userRepo);
    long nextOrderId() { return nextOrderId_++; }
    long nextTradeId() { return nextTradeId_++; }
    ExecutionResult placeOrder(std::unique_ptr<Order> order, User& user, Market& market);
    std::vector<ExecutionResult> processPendingOrders(Market& market);
    bool cancelOrder(long orderId, User& user);
    std::vector<Order*> getPendingOrders();
    int getPendingOrderCount();
    bool hasPendingOrders();
private:
    static constexpr long INITIAL_ORDER_ID = 1;
    static constexpr long INITIAL_TRADE_ID = 1;
    OrderValidator validator_;
    OrderBook orderBook_;
    std::unordered_map<OrderType, std::unique_ptr<OrderExecutionStrategy>> strategies_;
    OrderRepository& orderRepository_;
    TradeRepository& tradeRepository_;
    UserRepository& userRepository_;
    long nextOrderId_ = INITIAL_ORDER_ID;
    long nextTradeId_ = INITIAL_TRADE_ID;
};
}  // namespace stock_trading
#endif
