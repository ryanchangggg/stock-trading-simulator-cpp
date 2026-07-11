#ifndef STOCK_TRADING_SESSION_H
#define STOCK_TRADING_SESSION_H

#include "model/user/User.h"

namespace stock_trading {

class Session {
public:
    explicit Session(User& user)
        : user_(&user) {}

    User& getUser() { return *user_; }
    const User& getUser() const { return *user_; }
    long getUserId() const { return user_->getId(); }
    const std::string& getUsername() const { return user_->getUsername(); }

private:
    User* user_;
};

}  // namespace stock_trading

#endif
