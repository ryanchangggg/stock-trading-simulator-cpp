#include "service/AppContext.h"
#include "util/CsvDataLoader.h"
#include <iostream>
namespace stock_trading {
AppContext::AppContext(std::unique_ptr<UserRepository> userRepo,
                       std::unique_ptr<OrderRepository> orderRepo,
                       std::unique_ptr<PortfolioRepository> portfolioRepo,
                       std::unique_ptr<TradeRepository> tradeRepo)
    : userRepository_(std::move(userRepo))
    , orderRepository_(std::move(orderRepo))
    , portfolioRepository_(std::move(portfolioRepo))
    , tradeRepository_(std::move(tradeRepo)) {
    tradingEngine_ = std::make_unique<TradingEngine>(
        *orderRepository_, *tradeRepository_, *userRepository_);
    authService_ = std::make_unique<AuthenticationService>(*userRepository_);
    tradingService_ = std::make_unique<TradingService>(
        *tradingEngine_, *orderRepository_, *tradeRepository_, *userRepository_, market_);
    portfolioService_ = std::make_unique<PortfolioService>(
        *portfolioRepository_, *userRepository_, market_);
    marketDataService_ = std::make_unique<MarketDataService>(market_);
}
void AppContext::loadMarketData(const std::string& dataDir) {
    try {
        CsvDataLoader loader(market_);
        loader.loadAll(dataDir);
    } catch (const std::exception& e) {
        std::cerr << "Warning: could not load market data from " << dataDir
                  << ": " << e.what() << std::endl;
    }
}
}  // namespace stock_trading
