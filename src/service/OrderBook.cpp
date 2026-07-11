#include "service/OrderBook.h"
#include <stdexcept>
#include <algorithm>
namespace stock_trading {
void OrderBook::add(std::unique_ptr<Order> order) {
    if (order->getStatus() != OrderStatus::PENDING) {
        throw std::invalid_argument("Only PENDING orders can be added to the order book");
    }
    long id = order->getId();
    std::string symbol = order->getSymbol();
    ordersBySymbol_[symbol].push_back(order.get());
    ordersById_[id] = std::move(order);
}
Order* OrderBook::findById(long orderId) {
    auto it = ordersById_.find(orderId);
    if (it != ordersById_.end()) return it->second.get();
    return nullptr;
}
std::vector<Order*> OrderBook::getAllPending() {
    std::vector<Order*> result;
    for (auto& kv : ordersById_) {
        result.push_back(kv.second.get());
    }
    return result;
}
std::vector<Order*> OrderBook::getPendingBySymbol(const std::string& symbol) {
    auto it = ordersBySymbol_.find(symbol);
    if (it != ordersBySymbol_.end()) return it->second;
    return {};
}
Order* OrderBook::remove(long orderId) {
    auto it = ordersById_.find(orderId);
    if (it == ordersById_.end()) return nullptr;
    auto extracted = std::move(it->second);
    Order* ptr = extracted.get();
    auto& sym = ptr->getSymbol();
    auto symIt = ordersBySymbol_.find(sym);
    if (symIt != ordersBySymbol_.end()) {
        auto& vec = symIt->second;
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
        if (vec.empty()) ordersBySymbol_.erase(symIt);
    }
    ordersById_.erase(it);
    return ptr;
}
void OrderBook::clear() {
    ordersById_.clear();
    ordersBySymbol_.clear();
}
}  // namespace stock_trading
