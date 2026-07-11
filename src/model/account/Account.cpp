#include "model/account/Account.h"
#include "model/exception/InsufficientFundsException.h"
#include <stdexcept>
#include <random>
#include <sstream>
#include <ctime>

namespace stock_trading {

static std::string currentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

std::string Account::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static const char* hex = "0123456789abcdef";
    std::ostringstream oss;
    for (int i = 0; i < 16; ++i) {
        oss << hex[dis(gen)];
    }
    return oss.str();
}

Account::Account(std::string currency, double initialBalance)
    : accountId_(generateId())
    , currency_(std::move(currency))
    , balance_(initialBalance) {
    auto ts = currentTimestamp();
    createdAt_ = ts;
    lastUpdatedAt_ = ts;
    if (balance_ < 0) throw std::invalid_argument("Initial balance cannot be negative");
}

void Account::deposit(double amount) {
    if (amount <= 0) throw std::invalid_argument("Deposit amount must be positive");
    balance_ += amount;
    lastUpdatedAt_ = currentTimestamp();
}

void Account::withdraw(double amount) {
    if (amount <= 0) throw std::invalid_argument("Withdrawal amount must be positive");
    if (balance_ < amount) {
        throw InsufficientFundsException(
            "Insufficient funds: balance=" + std::to_string(balance_)
            + ", required=" + std::to_string(amount));
    }
    balance_ -= amount;
    lastUpdatedAt_ = currentTimestamp();
}

}  // namespace stock_trading
