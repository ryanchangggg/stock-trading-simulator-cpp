#ifndef STOCK_TRADING_STOCK_H
#define STOCK_TRADING_STOCK_H

#include <string>
#include <ostream>

namespace stock_trading {

class Stock {
public:
    Stock(std::string symbol, std::string companyName,
          std::string sector, double currentPrice);
    virtual ~Stock() = default;

    const std::string& getSymbol() const { return symbol_; }
    const std::string& getCompanyName() const { return companyName_; }
    const std::string& getSector() const { return sector_; }
    double getCurrentPrice() const { return currentPrice_; }
    void setCurrentPrice(double price);

    virtual std::string getAssetType() const = 0;

    bool operator==(const Stock& other) const { return symbol_ == other.symbol_; }

    friend std::ostream& operator<<(std::ostream& os, const Stock& s);

protected:
    std::string symbol_;
    std::string companyName_;
    std::string sector_;
    double currentPrice_ = 0.0;
};

}  // namespace stock_trading

#endif
