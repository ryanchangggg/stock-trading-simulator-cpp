#include "service/PortfolioService.h"
namespace stock_trading {
PortfolioService::PortfolioService(PortfolioRepository& portfolioRepo,
                                   UserRepository& userRepo, Market& market)
    : portfolioRepository_(portfolioRepo), userRepository_(userRepo), market_(market) {}
Result<Portfolio*> PortfolioService::getPortfolio(Session& session) {
    return Result<Portfolio*>::success(&portfolioRepository_.findByUserId(session.getUserId()));
}
Result<Portfolio::Summary> PortfolioService::getSummary(Session& session) {
    Portfolio& portfolio = portfolioRepository_.findByUserId(session.getUserId());
    Portfolio::Summary summary = portfolio.getSummary(
        [this](const std::string& sym) { return market_.getCurrentPrice(sym); });
    return Result<Portfolio::Summary>::success(summary);
}
Result<double> PortfolioService::getCashBalance(Session& session) {
    User* user = userRepository_.findByIdPtr(session.getUserId());
    if (!user) return Result<double>::failure("User not found");
    return Result<double>::success(user->getCashBalance());
}
Result<std::vector<std::reference_wrapper<const Position>>> PortfolioService::getPositions(Session& session) {
    Portfolio& portfolio = portfolioRepository_.findByUserId(session.getUserId());
    const auto& positions = portfolio.getPositions();
    std::vector<std::reference_wrapper<const Position>> result;
    result.reserve(positions.size());
    for (const auto& pos : positions) {
        result.push_back(std::cref(pos));
    }
    return Result<std::vector<std::reference_wrapper<const Position>>>::success(std::move(result));
}
}  // namespace stock_trading
