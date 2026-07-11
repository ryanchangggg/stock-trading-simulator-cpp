#include "model/market/Market.h"
#include <algorithm>
#include <stdexcept>

namespace stock_trading {

void Market::addStock(std::shared_ptr<Stock> stock) {
    if (!stock) throw std::invalid_argument("Stock must not be null");
    const auto& symbol = stock->getSymbol();
    if (stocks_.find(symbol) == stocks_.end()) {
        symbols_.push_back(symbol);
    }
    stocks_[symbol] = std::move(stock);
}

void Market::removeStock(const std::string& symbol) {
    stocks_.erase(symbol);
    auto it = std::find(symbols_.begin(), symbols_.end(), symbol);
    if (it != symbols_.end()) {
        symbols_.erase(it);
    }
}

std::shared_ptr<Stock> Market::getStock(const std::string& symbol) const {
    auto it = stocks_.find(symbol);
    if (it != stocks_.end()) {
        return it->second;
    }
    return nullptr;
}

double Market::getCurrentPrice(const std::string& symbol) const {
    auto stock = getStock(symbol);
    if (!stock) {
        throw std::invalid_argument("Stock not found: " + symbol);
    }
    return stock->getCurrentPrice();
}

void Market::updatePrice(const std::string& symbol, double price) {
    if (price < 0) throw std::invalid_argument("Price cannot be negative");
    auto stock = getStock(symbol);
    if (!stock) {
        throw std::invalid_argument("Stock not found: " + symbol);
    }
    stock->setCurrentPrice(price);
}

std::vector<std::string> Market::getAllSymbols() const {
    return symbols_;
}

std::vector<std::shared_ptr<Stock>> Market::getAllStocks() const {
    std::vector<std::shared_ptr<Stock>> result;
    result.reserve(stocks_.size());
    for (const auto& kv : stocks_) {
        result.push_back(kv.second);
    }
    return result;
}

void Market::clear() {
    stocks_.clear();
    symbols_.clear();
}

}  // namespace stock_trading
