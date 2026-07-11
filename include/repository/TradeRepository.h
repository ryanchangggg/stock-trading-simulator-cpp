#ifndef STOCK_TRADING_TRADE_REPOSITORY_H
#define STOCK_TRADING_TRADE_REPOSITORY_H

#include "model/trade/Trade.h"
#include <memory>
#include <vector>

namespace stock_trading {

class TradeRepository {
public:
    virtual ~TradeRepository() = default;

    virtual void save(std::unique_ptr<Trade> trade) = 0;
    virtual std::vector<Trade*> findByUserId(long userId) = 0;
    virtual std::vector<Trade*> findBySymbol(const std::string& symbol) = 0;
};

}  // namespace stock_trading

#endif
