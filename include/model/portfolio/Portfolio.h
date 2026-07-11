#ifndef STOCK_TRADING_PORTFOLIO_H
#define STOCK_TRADING_PORTFOLIO_H

#include "Position.h"
#include <vector>
#include <memory>
#include <functional>
#include <optional>

namespace stock_trading {

class Portfolio {
public:
   explicit Portfolio(long userId);

   long getUserId() const { return userId_; }
   const std::vector<Position>& getPositions() const { return positions_; }
    const std::string& getCreatedAt() const { return createdAt_; }
    const std::string& getLastUpdatedAt() const { return lastUpdatedAt_; }

   std::optional<std::reference_wrapper<const Position>> getPosition(const std::string& symbol) const;
    bool holdsSymbol(const std::string& symbol) const;

    void addShares(const std::string& symbol, int quantity, double purchasePrice);
    void removeShares(const std::string& symbol, int quantity);

    int getPositionCount() const { return static_cast<int>(positions_.size()); }
    int getTotalShares() const;

    struct Summary {
        double totalValue = 0.0;
        double totalCost = 0.0;
        double unrealisedPnl = 0.0;
        double unrealisedPnlPercent = 0.0;
        int positionCount = 0;
        int totalShares = 0;
    };

    Summary getSummary(const std::function<double(const std::string&)>& priceProvider) const;

private:
   long userId_;
    std::string createdAt_;
    std::string lastUpdatedAt_;
   std::vector<Position> positions_;
};

}  // namespace stock_trading

#endif
