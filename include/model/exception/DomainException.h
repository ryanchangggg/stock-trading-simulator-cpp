#ifndef STOCK_TRADING_EXCEPTION_H
#define STOCK_TRADING_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace stock_trading {

class DomainException : public std::runtime_error {
public:
    explicit DomainException(const std::string& message)
        : std::runtime_error(message) {}
    explicit DomainException(const char* message)
        : std::runtime_error(message) {}
};

}  // namespace stock_trading

#endif
