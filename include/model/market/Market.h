#ifndef STOCK_TRADING_MARKET_H
#define STOCK_TRADING_MARKET_H

#include "model/stock/Stock.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace stock_trading {

class Market {
public:
    Market() = default;

    void addStock(std::shared_ptr<Stock> stock);
    void removeStock(const std::string& symbol);

    std::shared_ptr<Stock> getStock(const std::string& symbol) const;
    double getCurrentPrice(const std::string& symbol) const;
    void updatePrice(const std::string& symbol, double price);

    std::vector<std::string> getAllSymbols() const;
    std::vector<std::shared_ptr<Stock>> getAllStocks() const;
    int getStockCount() const { return static_cast<int>(stocks_.size()); }
    bool isEmpty() const { return stocks_.empty(); }
    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Stock>> stocks_;
    std::vector<std::string> symbols_;
};

}  // namespace stock_trading

#endif
