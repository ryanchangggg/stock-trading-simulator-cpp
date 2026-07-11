#include "model/portfolio/Position.h"
#include <stdexcept>
#include <cmath>

namespace stock_trading {

Position::Position(std::string symbol, int quantity, double averageCost)
    : symbol_(std::move(symbol))
    , quantity_(quantity)
    , averageCost_(averageCost) {
    if (quantity < 0) throw std::invalid_argument("Position quantity cannot be negative");
    if (averageCost < 0) throw std::invalid_argument("Average cost cannot be negative");
    if (symbol_.empty()) throw std::invalid_argument("Symbol must not be empty");
}

double Position::getTotalCost() const {
    return averageCost_ * quantity_;
}

double Position::getMarketValue(double currentPrice) const {
    if (currentPrice < 0) throw std::invalid_argument("Current price cannot be negative");
    return currentPrice * quantity_;
}

double Position::getUnrealisedPnl(double currentPrice) const {
    if (currentPrice < 0) throw std::invalid_argument("Current price cannot be negative");
    return getMarketValue(currentPrice) - getTotalCost();
}

void Position::addShares(int addedQuantity, double purchasePrice) {
    if (addedQuantity <= 0) throw std::invalid_argument("Added quantity must be positive");
    if (purchasePrice < 0) throw std::invalid_argument("Purchase price cannot be negative");
    double totalCost = getTotalCost() + purchasePrice * addedQuantity;
    quantity_ += addedQuantity;
    // Round to 2 decimal places
    averageCost_ = std::round((totalCost / quantity_) * 100.0) / 100.0;
}

void Position::removeShares(int removedQuantity) {
    if (removedQuantity <= 0) throw std::invalid_argument("Removed quantity must be positive");
    if (removedQuantity > quantity_) {
        throw std::invalid_argument("Cannot remove " + std::to_string(removedQuantity)
            + " shares; only " + std::to_string(quantity_) + " held");
    }
    quantity_ -= removedQuantity;
}

}  // namespace stock_trading
