#include "service/MarketDataService.h"
namespace stock_trading {
MarketDataService::MarketDataService(Market& market) : market_(market) {}
Result<std::vector<std::shared_ptr<Stock>>> MarketDataService::getAvailableStocks() {
    return Result<std::vector<std::shared_ptr<Stock>>>::success(market_.getAllStocks());
}
Result<std::vector<std::string>> MarketDataService::getSymbols() {
    return Result<std::vector<std::string>>::success(market_.getAllSymbols());
}
Result<double> MarketDataService::getPrice(const std::string& symbol) {
    try { return Result<double>::success(market_.getCurrentPrice(symbol)); }
    catch (const std::exception& e) { return Result<double>::failure("Stock not found: " + symbol); }
}
Result<std::shared_ptr<Stock>> MarketDataService::getStock(const std::string& symbol) {
    auto stock = market_.getStock(symbol);
    if (stock) return Result<std::shared_ptr<Stock>>::success(stock);
    return Result<std::shared_ptr<Stock>>::failure("Stock not found: " + symbol);
}
}  // namespace stock_trading
