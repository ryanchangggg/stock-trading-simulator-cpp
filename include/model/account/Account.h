#ifndef STOCK_TRADING_ACCOUNT_H
#define STOCK_TRADING_ACCOUNT_H

#include <string>
#include <chrono>

namespace stock_trading {

class Account {
public:
    Account(std::string currency, double initialBalance);
    virtual ~Account() = default;

   const std::string& getAccountId() const { return accountId_; }
   double getBalance() const { return balance_; }
   const std::string& getCurrency() const { return currency_; }
    const std::string& getCreatedAt() const { return createdAt_; }
    const std::string& getLastUpdatedAt() const { return lastUpdatedAt_; }

    void deposit(double amount);
    void withdraw(double amount);

    virtual std::string getAccountType() const = 0;

protected:
   std::string accountId_;
   std::string currency_;
   double balance_ = 0.0;
    std::string createdAt_;
    std::string lastUpdatedAt_;

private:
    static std::string generateId();
};

}  // namespace stock_trading

#endif
