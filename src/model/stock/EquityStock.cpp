#include "model/stock/EquityStock.h"
#include <stdexcept>

namespace stock_trading {

EquityStock::EquityStock(std::string symbol, std::string companyName,
                         std::string sector, double currentPrice,
                         long marketCap, long sharesOutstanding)
    : Stock(std::move(symbol), std::move(companyName),
            std::move(sector), currentPrice)
    , marketCap_(marketCap)
    , sharesOutstanding_(sharesOutstanding) {
    if (marketCap < 0) throw std::invalid_argument("Market cap cannot be negative");
    if (sharesOutstanding < 0) throw std::invalid_argument("Shares outstanding cannot be negative");
}

void EquityStock::setMarketCap(long cap) {
    if (cap < 0) throw std::invalid_argument("Market cap cannot be negative");
    marketCap_ = cap;
}

void EquityStock::setSharesOutstanding(long shares) {
    if (shares < 0) throw std::invalid_argument("Shares outstanding cannot be negative");
    sharesOutstanding_ = shares;
}

}  // namespace stock_trading
