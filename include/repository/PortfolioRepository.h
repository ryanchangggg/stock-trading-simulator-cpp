#ifndef STOCK_TRADING_PORTFOLIO_REPOSITORY_H
#define STOCK_TRADING_PORTFOLIO_REPOSITORY_H

#include "model/portfolio/Portfolio.h"
#include <memory>

namespace stock_trading {

class PortfolioRepository {
public:
    virtual ~PortfolioRepository() = default;

    virtual Portfolio& findByUserId(long userId) = 0;
    virtual void save(Portfolio& portfolio) = 0;
    virtual void deleteByUserId(long userId) = 0;
};

}  // namespace stock_trading

#endif
