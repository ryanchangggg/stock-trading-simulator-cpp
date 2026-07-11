#include "service/TradingEngine.h"
#include "model/exception/InvalidOrderException.h"
#include "model/trade/Trade.h"
#include "model/trade/TradeType.h"
#include <stdexcept>
namespace stock_trading {
TradingEngine::TradingEngine(OrderRepository& orderRepo, TradeRepository& tradeRepo, UserRepository& userRepo)
    : orderRepository_(orderRepo), tradeRepository_(tradeRepo), userRepository_(userRepo) {
    strategies_[OrderType::MARKET] = std::make_unique<MarketOrderExecutor>();
    strategies_[OrderType::LIMIT] = std::make_unique<LimitOrderExecutor>();
    strategies_[OrderType::STOP_LOSS] = std::make_unique<StopLossExecutor>();
}
ExecutionResult TradingEngine::placeOrder(std::unique_ptr<Order> order, User& user, Market& market) {
    Order* rawPtr = order.get();
    try {
        validator_.validate(*rawPtr, user, market);
    } catch (const InvalidOrderException& e) {
        rawPtr->cancel();
        orderRepository_.save(std::move(order));
        return ExecutionResult::rejected(*rawPtr, e.what());
    }
    auto it = strategies_.find(rawPtr->getOrderType());
    if (it == strategies_.end()) {
        throw std::logic_error("No strategy for order type");
    }
    ExecutionResult result;
    try {
        result = it->second->execute(*rawPtr, user, market, nextTradeId_++);
    } catch (const std::exception& e) {
        rawPtr->cancel();
        orderRepository_.save(std::move(order));
        return ExecutionResult::rejected(*rawPtr, std::string("Execution failed: ") + e.what());
    }
    if (result.finalStatus == OrderStatus::PENDING) {
        orderBook_.add(std::move(order));
    } else {
        orderRepository_.save(std::move(order));
    }
    if (result.trade) {
        tradeRepository_.save(std::make_unique<Trade>(*result.trade));
    }
    return result;
}
std::vector<ExecutionResult> TradingEngine::processPendingOrders(Market& market) {
    std::vector<ExecutionResult> results;
    auto pending = orderBook_.getAllPending();
    for (auto* order : pending) {
        User* user = userRepository_.findByIdPtr(order->getUserId());
        if (!user) { orderBook_.remove(order->getId()); continue; }
        auto it = strategies_.find(order->getOrderType());
        if (it == strategies_.end()) continue;
        ExecutionResult result;
        try {
            result = it->second->execute(*order, *user, market, nextTradeId_++);
        } catch (...) {
            orderBook_.remove(order->getId());
            continue;
        }
    if (result.finalStatus == OrderStatus::FILLED) {
        orderBook_.remove(order->getId());
        if (result.trade) tradeRepository_.save(std::make_unique<Trade>(*result.trade));
        results.push_back(std::move(result));
    }
    }
    return results;
}
bool TradingEngine::cancelOrder(long orderId, User& user) {
    Order* order = orderBook_.findById(orderId);
    if (!order || order->getUserId() != user.getId()) return false;
    if (order->getStatus() != OrderStatus::PENDING) return false;
    order->cancel();
    orderBook_.remove(orderId);
    orderRepository_.updateStatus(orderId, OrderStatus::CANCELLED);
    return true;
}
std::vector<Order*> TradingEngine::getPendingOrders() {
    return orderBook_.getAllPending();
}
int TradingEngine::getPendingOrderCount() {
    return orderBook_.size();
}
bool TradingEngine::hasPendingOrders() {
    return !orderBook_.isEmpty();
}
}  // namespace stock_trading
