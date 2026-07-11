#ifndef STOCK_TRADING_TRADING_SERVICE_H
#define STOCK_TRADING_TRADING_SERVICE_H
#include "service/Result.h"
#include "service/Session.h"
#include "service/TradingEngine.h"
#include "repository/OrderRepository.h"
#include "repository/TradeRepository.h"
#include "repository/UserRepository.h"
#include "model/market/Market.h"
namespace stock_trading {
class TradingService {
public:
    TradingService(TradingEngine& engine, OrderRepository& orderRepo,
                   TradeRepository& tradeRepo, UserRepository& userRepo,
                   Market& market);
    Result<std::shared_ptr<Trade>> buyMarket(Session& session, const std::string& symbol, int qty);
    Result<std::shared_ptr<Trade>> sellMarket(Session& session, const std::string& symbol, int qty);
    Result<Order*> buyLimit(Session& session, const std::string& symbol, int qty, double limit);
    Result<Order*> sellLimit(Session& session, const std::string& symbol, int qty, double limit);
    Result<Order*> buyStopLoss(Session& session, const std::string& symbol, int qty, double stop);
    Result<Order*> sellStopLoss(Session& session, const std::string& symbol, int qty, double stop);
    Result<void> cancelOrder(Session& session, long orderId);
    Result<std::vector<Order*>> getOrders(Session& session);
    Result<std::vector<Order*>> getPendingOrders(Session& session);
    Result<std::vector<Trade*>> getTradeHistory(Session& session);
private:
    TradingEngine& engine_;
    OrderRepository& orderRepository_;
    TradeRepository& tradeRepository_;
    UserRepository& userRepository_;
    Market& market_;
};
}  // namespace stock_trading
#endif
