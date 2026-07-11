#include "model/user/User.h"
#include <stdexcept>

namespace stock_trading {

User::User(long id, std::string username, std::string passwordHash)
    : id_(id)
    , username_(std::move(username))
    , passwordHash_(std::move(passwordHash))
    , account_(std::make_unique<CashAccount>())
    , portfolio_(std::make_unique<Portfolio>(id_)) {
    if (username_.empty()) throw std::invalid_argument("Username must not be empty");
}

User::User(long id, std::string username, std::string passwordHash, double initialBalance)
    : id_(id)
    , username_(std::move(username))
    , passwordHash_(std::move(passwordHash))
    , account_(std::make_unique<CashAccount>(initialBalance))
    , portfolio_(std::make_unique<Portfolio>(id_)) {
    if (username_.empty()) throw std::invalid_argument("Username must not be empty");
}

bool User::canAfford(double amount) const {
    return account_->getBalance() >= amount;
}

bool User::holdsAtLeast(const std::string& symbol, int quantity) const {
    auto pos = portfolio_->getPosition(symbol);
    return pos.has_value() && pos->get().getQuantity() >= quantity;
}

}  // namespace stock_trading
