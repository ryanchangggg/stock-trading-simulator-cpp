#ifndef STOCK_TRADING_POSITION_H
#define STOCK_TRADING_POSITION_H

#include <string>

namespace stock_trading {

class Position {
public:
    Position(std::string symbol, int quantity, double averageCost);

    const std::string& getSymbol() const { return symbol_; }
    int getQuantity() const { return quantity_; }
    double getAverageCost() const { return averageCost_; }
    double getTotalCost() const;
    double getMarketValue(double currentPrice) const;
    double getUnrealisedPnl(double currentPrice) const;

    void addShares(int addedQuantity, double purchasePrice);
    void removeShares(int removedQuantity);
    bool isEmpty() const { return quantity_ == 0; }

    bool operator==(const Position& other) const { return symbol_ == other.symbol_; }

private:
    std::string symbol_;
    int quantity_ = 0;
    double averageCost_ = 0.0;
};

}  // namespace stock_trading

#endif
