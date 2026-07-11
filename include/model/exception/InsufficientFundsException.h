#ifndef STOCK_TRADING_INSUFFICIENT_FUNDS_H
#define STOCK_TRADING_INSUFFICIENT_FUNDS_H

#include "DomainException.h"

namespace stock_trading {

class InsufficientFundsException : public DomainException {
public:
    explicit InsufficientFundsException(const std::string& message)
        : DomainException(message) {}
};

}  // namespace stock_trading

#endif
