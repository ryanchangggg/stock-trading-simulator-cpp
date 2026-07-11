#ifndef STOCK_TRADING_CASH_ACCOUNT_H
#define STOCK_TRADING_CASH_ACCOUNT_H

#include "Account.h"

namespace stock_trading {

class CashAccount : public Account {
public:
    static constexpr double DEFAULT_STARTING_BALANCE = 100000.00;

    CashAccount();
    explicit CashAccount(double initialBalance);
    CashAccount(std::string currency, double initialBalance);

    std::string getAccountType() const override { return "CashAccount"; }
};

}  // namespace stock_trading

#endif
