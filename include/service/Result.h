#ifndef STOCK_TRADING_RESULT_H
#define STOCK_TRADING_RESULT_H
#include <string>
#include <optional>
namespace stock_trading {
template<typename T>
class Result {
public:
    static Result success(T value) {
        return Result(true, std::move(value), "");
    }
    static Result failure(std::string error) {
        return Result(false, T{}, std::move(error));
    }
    bool isSuccess() const { return success_; }
    bool isFailure() const { return !success_; }
    const T& getValue() const { return value_; }
    const std::string& getErrorMessage() const { return error_; }
private:
    Result(bool success, T value, std::string error)
        : success_(success), value_(std::move(value)), error_(std::move(error)) {}
    bool success_;
    T value_;
    std::string error_;
};
template<>
class Result<void> {
public:
    static Result success() { return Result(true, ""); }
    static Result failure(std::string error) { return Result(false, std::move(error)); }
    bool isSuccess() const { return success_; }
    bool isFailure() const { return !success_; }
    const std::string& getErrorMessage() const { return error_; }
private:
    Result(bool success, std::string error) : success_(success), error_(std::move(error)) {}
    bool success_;
    std::string error_;
};
}  // namespace stock_trading
#endif
