#ifndef STOCK_TRADING_INVALID_ORDER_H
#define STOCK_TRADING_INVALID_ORDER_H

#include "DomainException.h"

namespace stock_trading {

class InvalidOrderException : public DomainException {
public:
    explicit InvalidOrderException(const std::string& message)
        : DomainException(message) {}
};

}  // namespace stock_trading

#endif
