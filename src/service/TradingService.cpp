#include "service/TradingService.h"
#include "model/order/BuyOrder.h"
#include "model/order/SellOrder.h"
#include "model/order/StopLossOrder.h"
#include <memory>
namespace stock_trading {
TradingService::TradingService(TradingEngine& engine, OrderRepository& orderRepo,
                               TradeRepository& tradeRepo, UserRepository& userRepo,
                               Market& market)
    : engine_(engine), orderRepository_(orderRepo),
      tradeRepository_(tradeRepo), userRepository_(userRepo), market_(market) {}
Result<std::shared_ptr<Trade>> TradingService::buyMarket(Session& session, const std::string& symbol, int qty) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<std::shared_ptr<Trade>>::failure("User not found");
    auto order = std::make_unique<BuyOrder>(engine_.nextOrderId(), user->getId(), symbol, qty,
                                            market_.getCurrentPrice(symbol), OrderType::MARKET);
    ExecutionResult result = engine_.placeOrder(std::move(order), *user, market_);
    if (!result.success) return Result<std::shared_ptr<Trade>>::failure(result.message);
    if (result.trade) {
        return Result<std::shared_ptr<Trade>>::success(std::make_shared<Trade>(*result.trade));
    }
    return Result<std::shared_ptr<Trade>>::failure("Order was not executed");
}
Result<std::shared_ptr<Trade>> TradingService::sellMarket(Session& session, const std::string& symbol, int qty) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<std::shared_ptr<Trade>>::failure("User not found");
    auto order = std::make_unique<SellOrder>(engine_.nextOrderId(), user->getId(), symbol, qty,
                                             market_.getCurrentPrice(symbol), OrderType::MARKET);
    ExecutionResult result = engine_.placeOrder(std::move(order), *user, market_);
    if (!result.success) return Result<std::shared_ptr<Trade>>::failure(result.message);
    if (result.trade) {
        return Result<std::shared_ptr<Trade>>::success(std::make_shared<Trade>(*result.trade));
    }
    return Result<std::shared_ptr<Trade>>::failure("Order was not executed");
}
Result<Order*> TradingService::buyLimit(Session& session, const std::string& symbol, int qty, double limit) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<Order*>::failure("User not found");
    auto order = std::make_unique<BuyOrder>(engine_.nextOrderId(), user->getId(), symbol, qty, limit, OrderType::LIMIT);
    auto* raw = order.get();
    ExecutionResult result = engine_.placeOrder(std::move(order), *user, market_);
    if (!result.success) return Result<Order*>::failure(result.message);
    return Result<Order*>::success(raw);
}
Result<Order*> TradingService::sellLimit(Session& session, const std::string& symbol, int qty, double limit) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<Order*>::failure("User not found");
    auto order = std::make_unique<SellOrder>(engine_.nextOrderId(), user->getId(), symbol, qty, limit, OrderType::LIMIT);
    auto* raw = order.get();
    ExecutionResult result = engine_.placeOrder(std::move(order), *user, market_);
    if (!result.success) return Result<Order*>::failure(result.message);
    return Result<Order*>::success(raw);
}
Result<Order*> TradingService::buyStopLoss(Session& session, const std::string& symbol, int qty, double stop) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<Order*>::failure("User not found");
    auto order = std::make_unique<StopLossOrder>(engine_.nextOrderId(), user->getId(), symbol, qty, stop, TradeType::BUY);
    auto* raw = order.get();
    ExecutionResult result = engine_.placeOrder(std::move(order), *user, market_);
    if (!result.success) return Result<Order*>::failure(result.message);
    return Result<Order*>::success(raw);
}
Result<Order*> TradingService::sellStopLoss(Session& session, const std::string& symbol, int qty, double stop) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<Order*>::failure("User not found");
    auto order = std::make_unique<StopLossOrder>(engine_.nextOrderId(), user->getId(), symbol, qty, stop, TradeType::SELL);
    auto* raw = order.get();
    ExecutionResult result = engine_.placeOrder(std::move(order), *user, market_);
    if (!result.success) return Result<Order*>::failure(result.message);
    return Result<Order*>::success(raw);
}
Result<void> TradingService::cancelOrder(Session& session, long orderId) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<void>::failure("User not found");
    bool ok = engine_.cancelOrder(orderId, *user);
    return ok ? Result<void>::success() : Result<void>::failure("Order not found or cannot be cancelled");
}
Result<std::vector<Order*>> TradingService::getOrders(Session& session) {
    return Result<std::vector<Order*>>::success(orderRepository_.findByUserId(session.getUserId()));
}
Result<std::vector<Order*>> TradingService::getPendingOrders(Session& session) {
    return Result<std::vector<Order*>>::success(orderRepository_.findByStatus(OrderStatus::PENDING));
}
Result<std::vector<Trade*>> TradingService::getTradeHistory(Session& session) {
    return Result<std::vector<Trade*>>::success(tradeRepository_.findByUserId(session.getUserId()));
}
}  // namespace stock_trading
