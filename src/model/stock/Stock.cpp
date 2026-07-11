#include "model/stock/Stock.h"
#include <stdexcept>

namespace stock_trading {

Stock::Stock(std::string symbol, std::string companyName,
             std::string sector, double currentPrice)
    : symbol_(std::move(symbol))
    , companyName_(std::move(companyName))
    , sector_(std::move(sector))
    , currentPrice_(currentPrice) {
    if (symbol_.empty()) throw std::invalid_argument("Symbol must not be empty");
    if (currentPrice_ < 0) throw std::invalid_argument("Price cannot be negative");
}

void Stock::setCurrentPrice(double price) {
    if (price < 0) throw std::invalid_argument("Price cannot be negative");
    currentPrice_ = price;
}

std::ostream& operator<<(std::ostream& os, const Stock& s) {
    os << s.getAssetType() << "{symbol='" << s.symbol_
       << "', company='" << s.companyName_
       << "', price=" << s.currentPrice_ << "}";
    return os;
}

}  // namespace stock_trading
