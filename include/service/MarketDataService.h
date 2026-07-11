#ifndef STOCK_TRADING_MARKET_DATA_SERVICE_H
#define STOCK_TRADING_MARKET_DATA_SERVICE_H
#include "model/market/Market.h"
#include "service/Result.h"
namespace stock_trading {
class MarketDataService {
public:
    explicit MarketDataService(Market& market);
    Result<std::vector<std::shared_ptr<Stock>>> getAvailableStocks();
    Result<std::vector<std::string>> getSymbols();
    Result<double> getPrice(const std::string& symbol);
    Result<std::shared_ptr<Stock>> getStock(const std::string& symbol);
private:
    Market& market_;
};
}  // namespace stock_trading
#endif
