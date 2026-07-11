#ifndef STOCK_TRADING_PORTFOLIO_SERVICE_H
#define STOCK_TRADING_PORTFOLIO_SERVICE_H
#include "service/Result.h"
#include "service/Session.h"
#include "model/portfolio/Portfolio.h"
#include "model/market/Market.h"
#include "repository/PortfolioRepository.h"
#include "repository/UserRepository.h"
namespace stock_trading {
class PortfolioService {
public:
    PortfolioService(PortfolioRepository& portfolioRepo,
                     UserRepository& userRepo, Market& market);
    Result<Portfolio*> getPortfolio(Session& session);
    Result<Portfolio::Summary> getSummary(Session& session);
    Result<double> getCashBalance(Session& session);
    Result<std::vector<std::reference_wrapper<const Position>>> getPositions(Session& session);
private:
    PortfolioRepository& portfolioRepository_;
    UserRepository& userRepository_;
    Market& market_;
};
}  // namespace stock_trading
#endif
