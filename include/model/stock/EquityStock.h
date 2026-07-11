#ifndef STOCK_TRADING_EQUITY_STOCK_H
#define STOCK_TRADING_EQUITY_STOCK_H

#include "Stock.h"

namespace stock_trading {

class EquityStock : public Stock {
public:
    EquityStock(std::string symbol, std::string companyName,
                std::string sector, double currentPrice,
                long marketCap = 0, long sharesOutstanding = 0);

    long getMarketCap() const { return marketCap_; }
    void setMarketCap(long cap);
    long getSharesOutstanding() const { return sharesOutstanding_; }
    void setSharesOutstanding(long shares);

    std::string getAssetType() const override { return "Equity"; }

private:
    long marketCap_ = 0;
    long sharesOutstanding_ = 0;
};

}  // namespace stock_trading

#endif
