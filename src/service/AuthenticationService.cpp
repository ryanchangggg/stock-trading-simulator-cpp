#include "service/AuthenticationService.h"
#include <iomanip>
#include <sstream>
#include <functional>
namespace stock_trading {
AuthenticationService::AuthenticationService(UserRepository& userRepo) : userRepository_(userRepo) {}
std::string AuthenticationService::hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    auto hash = hasher(password);
    std::ostringstream oss;
    oss << std::hex << hash;
    return oss.str();
}
Result<std::shared_ptr<Session>> AuthenticationService::registerUser(const std::string& username, const std::string& password) {
    if (username.empty()) return Result<std::shared_ptr<Session>>::failure("Username is required");
    if (password.length() < 4) return Result<std::shared_ptr<Session>>::failure("Password must be at least 4 characters");
    if (userRepository_.findByUsernamePtr(username) != nullptr)
        return Result<std::shared_ptr<Session>>::failure("Username already exists");
    auto user = std::make_unique<User>(nextUserId_++, username, hashPassword(password));
    User& saved = userRepository_.save(std::move(user));
    auto session = std::make_shared<Session>(saved);
    return Result<std::shared_ptr<Session>>::success(session);
}
Result<std::shared_ptr<Session>> AuthenticationService::login(const std::string& username, const std::string& password) {
    if (username.empty()) return Result<std::shared_ptr<Session>>::failure("Username is required");
    if (password.empty()) return Result<std::shared_ptr<Session>>::failure("Password is required");
    User* user = userRepository_.findByUsernamePtr(username);
    if (!user) return Result<std::shared_ptr<Session>>::failure("Invalid username or password");
    if (user->getPasswordHash() != hashPassword(password))
        return Result<std::shared_ptr<Session>>::failure("Invalid username or password");
    auto session = std::make_shared<Session>(*user);
    return Result<std::shared_ptr<Session>>::success(session);
}
}  // namespace stock_trading
