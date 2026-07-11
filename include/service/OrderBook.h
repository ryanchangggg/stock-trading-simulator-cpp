#ifndef STOCK_TRADING_ORDER_BOOK_H
#define STOCK_TRADING_ORDER_BOOK_H
#include "model/order/Order.h"
#include "model/order/OrderStatus.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
namespace stock_trading {
class OrderBook {
public:
    OrderBook() = default;
    void add(std::unique_ptr<Order> order);
    Order* findById(long orderId);
    std::vector<Order*> getAllPending();
    std::vector<Order*> getPendingBySymbol(const std::string& symbol);
    Order* remove(long orderId);
    int size() const { return static_cast<int>(ordersById_.size()); }
    bool isEmpty() const { return ordersById_.empty(); }
    void clear();
private:
    std::unordered_map<long, std::unique_ptr<Order>> ordersById_;
    std::unordered_map<std::string, std::vector<Order*>> ordersBySymbol_;
};
}  // namespace stock_trading
#endif
