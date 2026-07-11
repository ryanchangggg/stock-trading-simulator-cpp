#ifndef STOCK_TRADING_USER_REPOSITORY_H
#define STOCK_TRADING_USER_REPOSITORY_H

#include "model/user/User.h"
#include <memory>
#include <optional>

namespace stock_trading {

class UserRepository {
public:
    virtual ~UserRepository() = default;

    virtual std::optional<std::reference_wrapper<User>> findById(long id) = 0;
    virtual std::optional<std::reference_wrapper<User>> findByUsername(const std::string& username) = 0;
    virtual User& save(std::unique_ptr<User> user) = 0;
    virtual User* findByIdPtr(long id) = 0;
    virtual User* findByUsernamePtr(const std::string& username) = 0;
};

}  // namespace stock_trading

#endif
