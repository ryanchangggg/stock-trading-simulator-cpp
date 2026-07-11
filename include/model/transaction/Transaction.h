#ifndef STOCK_TRADING_TRANSACTION_H
#define STOCK_TRADING_TRANSACTION_H

#include "TransactionType.h"
#include <string>

namespace stock_trading {

class Transaction {
public:
    Transaction(long id, long userId, std::string accountId,
                TransactionType type, double amount,
                double balanceAfter, std::string description);

    long getId() const { return id_; }
    long getUserId() const { return userId_; }
    const std::string& getAccountId() const { return accountId_; }
    TransactionType getType() const { return type_; }
    double getAmount() const { return amount_; }
    double getBalanceAfter() const { return balanceAfter_; }
    const std::string& getDescription() const { return description_; }

    bool operator==(const Transaction& other) const { return id_ == other.id_; }

private:
    long id_;
    long userId_;
    std::string accountId_;
    TransactionType type_;
    double amount_ = 0.0;
    double balanceAfter_ = 0.0;
    std::string description_;
};

}  // namespace stock_trading

#endif
