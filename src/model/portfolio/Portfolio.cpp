#include "model/portfolio/Portfolio.h"
#include "model/exception/InvalidOrderException.h"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <ctime>

namespace stock_trading {

static std::string currentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

Portfolio::Portfolio(long userId) : userId_(userId) {
    auto ts = currentTimestamp();
    createdAt_ = ts;
    lastUpdatedAt_ = ts;
}

std::optional<std::reference_wrapper<const Position>> Portfolio::getPosition(const std::string& symbol) const {
    for (const auto& p : positions_) {
        if (p.getSymbol() == symbol) {
            return std::cref(p);
        }
    }
    return std::nullopt;
}

bool Portfolio::holdsSymbol(const std::string& symbol) const {
    auto pos = getPosition(symbol);
    return pos.has_value() && pos->get().getQuantity() > 0;
}

void Portfolio::addShares(const std::string& symbol, int quantity, double purchasePrice) {
    if (quantity <= 0) throw std::invalid_argument("Added quantity must be positive");
    if (purchasePrice < 0) throw std::invalid_argument("Purchase price cannot be negative");

   for (auto& p : positions_) {
       if (p.getSymbol() == symbol) {
           p.addShares(quantity, purchasePrice);
            lastUpdatedAt_ = currentTimestamp();
           return;
       }
   }
   positions_.emplace_back(symbol, quantity, purchasePrice);
    lastUpdatedAt_ = currentTimestamp();
}

void Portfolio::removeShares(const std::string& symbol, int quantity) {
    if (quantity <= 0) throw std::invalid_argument("Removed quantity must be positive");

    for (auto it = positions_.begin(); it != positions_.end(); ++it) {
        if (it->getSymbol() == symbol) {
            if (it->getQuantity() < quantity) {
                throw InvalidOrderException("Insufficient shares: held="
                    + std::to_string(it->getQuantity()) + ", requested=" + std::to_string(quantity));
            }
            it->removeShares(quantity);
           if (it->isEmpty()) {
               positions_.erase(it);
           }
            lastUpdatedAt_ = currentTimestamp();
           return;
        }
    }
    throw InvalidOrderException("No position found for symbol: " + symbol);
}

int Portfolio::getTotalShares() const {
    int total = 0;
    for (const auto& p : positions_) {
        total += p.getQuantity();
    }
    return total;
}

Portfolio::Summary Portfolio::getSummary(
    const std::function<double(const std::string&)>& priceProvider) const {
    Summary summary;
    for (const auto& pos : positions_) {
        double currentPrice = priceProvider(pos.getSymbol());
        summary.totalValue += pos.getMarketValue(currentPrice);
        summary.totalCost += pos.getTotalCost();
    }
    summary.unrealisedPnl = summary.totalValue - summary.totalCost;
    summary.positionCount = static_cast<int>(positions_.size());
    summary.totalShares = getTotalShares();

    if (summary.totalCost > 0) {
        summary.unrealisedPnlPercent =
            std::round((summary.unrealisedPnl / summary.totalCost) * 10000.0) / 100.0;
    }

    return summary;
}

}  // namespace stock_trading
