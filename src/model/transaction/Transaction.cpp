#include "model/transaction/Transaction.h"
#include <stdexcept>

namespace stock_trading {

Transaction::Transaction(long id, long userId, std::string accountId,
                         TransactionType type, double amount,
                         double balanceAfter, std::string description)
    : id_(id)
    , userId_(userId)
    , accountId_(std::move(accountId))
    , type_(type)
    , amount_(amount)
    , balanceAfter_(balanceAfter)
    , description_(std::move(description)) {
    if (amount <= 0) throw std::invalid_argument("Transaction amount must be positive");
}

}  // namespace stock_trading
