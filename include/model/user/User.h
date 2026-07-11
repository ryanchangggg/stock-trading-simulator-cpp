#ifndef STOCK_TRADING_USER_H
#define STOCK_TRADING_USER_H

#include "model/account/CashAccount.h"
#include "model/portfolio/Portfolio.h"
#include <memory>
#include <string>

namespace stock_trading {

class User {
public:
    User(long id, std::string username, std::string passwordHash);
    User(long id, std::string username, std::string passwordHash, double initialBalance);

    long getId() const { return id_; }
    const std::string& getUsername() const { return username_; }
    const std::string& getPasswordHash() const { return passwordHash_; }
    void setPasswordHash(const std::string& hash) { passwordHash_ = hash; }

    Account& getAccount() { return *account_; }
    const Account& getAccount() const { return *account_; }
    Portfolio& getPortfolio() { return *portfolio_; }
    const Portfolio& getPortfolio() const { return *portfolio_; }

    double getCashBalance() const { return account_->getBalance(); }
    void deposit(double amount) { account_->deposit(amount); }
    void withdraw(double amount) { account_->withdraw(amount); }

    bool canAfford(double amount) const;
    bool holdsAtLeast(const std::string& symbol, int quantity) const;

    bool operator==(const User& other) const { return id_ == other.id_; }

private:
    long id_;
    std::string username_;
    std::string passwordHash_;
    std::unique_ptr<Account> account_;
    std::unique_ptr<Portfolio> portfolio_;
};

}  // namespace stock_trading

#endif
