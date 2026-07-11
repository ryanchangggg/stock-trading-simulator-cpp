#ifndef STOCK_TRADING_AUTH_SERVICE_H
#define STOCK_TRADING_AUTH_SERVICE_H
#include "service/Result.h"
#include "service/Session.h"
#include "repository/UserRepository.h"
#include <memory>
namespace stock_trading {
class AuthenticationService {
public:
    explicit AuthenticationService(UserRepository& userRepo);
    Result<std::shared_ptr<Session>> registerUser(const std::string& username, const std::string& password);
    Result<std::shared_ptr<Session>> login(const std::string& username, const std::string& password);
private:
    UserRepository& userRepository_;
    long nextUserId_ = 1;
    static std::string hashPassword(const std::string& password);
};
}  // namespace stock_trading
#endif
