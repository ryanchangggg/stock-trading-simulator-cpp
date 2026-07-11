#include "model/account/CashAccount.h"

namespace stock_trading {

CashAccount::CashAccount()
    : Account("USD", DEFAULT_STARTING_BALANCE) {}

CashAccount::CashAccount(double initialBalance)
    : Account("USD", initialBalance) {}

CashAccount::CashAccount(std::string currency, double initialBalance)
    : Account(std::move(currency), initialBalance) {}

}  // namespace stock_trading
