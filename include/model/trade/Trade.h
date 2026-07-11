#ifndef STOCK_TRADING_TRADE_H
#define STOCK_TRADING_TRADE_H

#include "model/trade/TradeType.h"
#include <string>
#include <chrono>

namespace stock_trading {

class Trade {
public:
    Trade(long id, long userId, std::string symbol,
          TradeType tradeType, int quantity, double price);

    long getId() const { return id_; }
    long getUserId() const { return userId_; }
    const std::string& getSymbol() const { return symbol_; }
    TradeType getTradeType() const { return tradeType_; }
    std::string getTradeTypeName() const { return tradeTypeToString(tradeType_); }
    int getQuantity() const { return quantity_; }
    double getPrice() const { return price_; }
    double getTotalValue() const { return totalValue_; }

    bool operator==(const Trade& other) const { return id_ == other.id_; }

private:
    long id_;
    long userId_;
    std::string symbol_;
    TradeType tradeType_;
    int quantity_ = 0;
    double price_ = 0.0;
    double totalValue_ = 0.0;
};

}  // namespace stock_trading

#endif
