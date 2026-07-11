#ifndef STOCK_TRADING_CSV_LOADER_H
#define STOCK_TRADING_CSV_LOADER_H
#include "model/market/Market.h"
#include <string>
namespace stock_trading {
class CsvDataLoader {
public:
    explicit CsvDataLoader(Market& market);
    void loadAll(const std::string& dataDir);
private:
    Market& market_;
    void loadFile(const std::string& path);
};
}  // namespace stock_trading
#endif
