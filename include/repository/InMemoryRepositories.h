#ifndef STOCK_TRADING_INMEMORY_REPOS_H
#define STOCK_TRADING_INMEMORY_REPOS_H
#include "UserRepository.h"
#include "OrderRepository.h"
#include "PortfolioRepository.h"
#include "TradeRepository.h"
#include "model/portfolio/Portfolio.h"
#include <unordered_map>
#include <vector>
namespace stock_trading {
class InMemoryUserRepository : public UserRepository {
public:
    std::optional<std::reference_wrapper<User>> findById(long id) override {
        auto it = users_.find(id);
        if (it != users_.end()) return *it->second;
        return std::nullopt;
    }
    std::optional<std::reference_wrapper<User>> findByUsername(const std::string& username) override {
        for (auto& kv : users_) {
            if (kv.second->getUsername() == username) return *kv.second;
        }
        return std::nullopt;
    }
    User& save(std::unique_ptr<User> user) override {
        auto* ptr = user.get();
        users_[ptr->getId()] = std::move(user);
        return *ptr;
    }
    User* findByIdPtr(long id) override {
        auto it = users_.find(id);
        if (it != users_.end()) return it->second.get();
        return nullptr;
    }
    User* findByUsernamePtr(const std::string& username) override {
        for (auto& kv : users_) {
            if (kv.second->getUsername() == username) return kv.second.get();
        }
        return nullptr;
    }
private:
    std::unordered_map<long, std::unique_ptr<User>> users_;
};
class InMemoryOrderRepository : public OrderRepository {
public:
    void save(std::unique_ptr<Order> order) override {
        auto* ptr = order.get();
        orders_[ptr->getId()] = std::move(order);
    }
    Order* findById(long id) override {
        auto it = orders_.find(id);
        if (it != orders_.end()) return it->second.get();
        return nullptr;
    }
    std::vector<Order*> findByUserId(long userId) override {
        std::vector<Order*> result;
        for (auto& kv : orders_) {
            if (kv.second->getUserId() == userId) result.push_back(kv.second.get());
        }
        return result;
    }
    std::vector<Order*> findByStatus(OrderStatus status) override {
        std::vector<Order*> result;
        for (auto& kv : orders_) {
            if (kv.second->getStatus() == status) result.push_back(kv.second.get());
        }
        return result;
    }
    void updateStatus(long orderId, OrderStatus newStatus) override {
        auto it = orders_.find(orderId);
        if (it != orders_.end()) {
            it->second->restoreStatus(newStatus, "");
        }
    }
private:
    std::unordered_map<long, std::unique_ptr<Order>> orders_;
};
class InMemoryPortfolioRepository : public PortfolioRepository {
public:
    Portfolio& findByUserId(long userId) override {
        auto it = portfolios_.find(userId);
        if (it == portfolios_.end()) {
            portfolios_[userId] = std::make_unique<Portfolio>(userId);
        }
        return *portfolios_[userId];
    }
    void save(Portfolio& portfolio) override {
        auto* existing = portfolios_[portfolio.getUserId()].get();
        if (existing != &portfolio) {
            portfolios_[portfolio.getUserId()] = std::make_unique<Portfolio>(portfolio.getUserId());
            for (const auto& pos : portfolio.getPositions()) {
                portfolios_[portfolio.getUserId()]->addShares(
                    pos.getSymbol(), pos.getQuantity(), pos.getAverageCost());
            }
        }
    }
    void deleteByUserId(long userId) override {
        portfolios_.erase(userId);
    }
private:
    std::unordered_map<long, std::unique_ptr<Portfolio>> portfolios_;
};
class InMemoryTradeRepository : public TradeRepository {
public:
    void save(std::unique_ptr<Trade> trade) override {
        trades_.push_back(std::move(trade));
    }
    std::vector<Trade*> findByUserId(long userId) override {
        std::vector<Trade*> result;
        for (auto& t : trades_) {
            if (t->getUserId() == userId) result.push_back(t.get());
        }
        return result;
    }
    std::vector<Trade*> findBySymbol(const std::string& symbol) override {
        std::vector<Trade*> result;
        for (auto& t : trades_) {
            if (t->getSymbol() == symbol) result.push_back(t.get());
        }
        return result;
    }
private:
    std::vector<std::unique_ptr<Trade>> trades_;
};
}  // namespace stock_trading
#endif
