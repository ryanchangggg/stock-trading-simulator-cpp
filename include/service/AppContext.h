#ifndef STOCK_TRADING_APP_CONTEXT_H
#define STOCK_TRADING_APP_CONTEXT_H
#include "model/market/Market.h"
#include "service/AuthenticationService.h"
#include "service/TradingService.h"
#include "service/PortfolioService.h"
#include "service/MarketDataService.h"
#include "service/TradingEngine.h"
#include "repository/UserRepository.h"
#include "repository/OrderRepository.h"
#include "repository/PortfolioRepository.h"
#include "repository/TradeRepository.h"
#include <memory>
namespace stock_trading {
class AppContext {
public:
    AppContext(std::unique_ptr<UserRepository> userRepo,
               std::unique_ptr<OrderRepository> orderRepo,
               std::unique_ptr<PortfolioRepository> portfolioRepo,
               std::unique_ptr<TradeRepository> tradeRepo);
    void loadMarketData(const std::string& dataDir);
    AuthenticationService& getAuthService() { return *authService_; }
    TradingService& getTradingService() { return *tradingService_; }
    PortfolioService& getPortfolioService() { return *portfolioService_; }
    MarketDataService& getMarketDataService() { return *marketDataService_; }
    Market& getMarket() { return market_; }
    TradingEngine& getEngine() { return *tradingEngine_; }
private:
    Market market_;
    std::unique_ptr<UserRepository> userRepository_;
    std::unique_ptr<OrderRepository> orderRepository_;
    std::unique_ptr<PortfolioRepository> portfolioRepository_;
    std::unique_ptr<TradeRepository> tradeRepository_;
    std::unique_ptr<TradingEngine> tradingEngine_;
    std::unique_ptr<AuthenticationService> authService_;
    std::unique_ptr<TradingService> tradingService_;
    std::unique_ptr<PortfolioService> portfolioService_;
    std::unique_ptr<MarketDataService> marketDataService_;
};
}  // namespace stock_trading
#endif
