#include "model/trade/Trade.h"
#include <stdexcept>

namespace stock_trading {

Trade::Trade(long id, long userId, std::string symbol,
             TradeType tradeType, int quantity, double price)
    : id_(id)
    , userId_(userId)
    , symbol_(std::move(symbol))
    , tradeType_(tradeType)
    , quantity_(quantity)
    , price_(price)
    , totalValue_(price * quantity) {
    if (quantity <= 0) throw std::invalid_argument("Trade quantity must be positive");
    if (price < 0) throw std::invalid_argument("Price cannot be negative");
    if (symbol_.empty()) throw std::invalid_argument("Symbol must not be empty");
}

}  // namespace stock_trading
