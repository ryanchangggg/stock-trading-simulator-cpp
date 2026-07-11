#ifndef STOCK_TRADING_ORDER_REPOSITORY_H
#define STOCK_TRADING_ORDER_REPOSITORY_H

#include "model/order/Order.h"
#include <memory>
#include <vector>

namespace stock_trading {

class OrderRepository {
public:
    virtual ~OrderRepository() = default;

    virtual void save(std::unique_ptr<Order> order) = 0;
    virtual Order* findById(long id) = 0;
    virtual std::vector<Order*> findByUserId(long userId) = 0;
    virtual std::vector<Order*> findByStatus(OrderStatus status) = 0;
    virtual void updateStatus(long orderId, OrderStatus newStatus) = 0;
};

}  // namespace stock_trading

#endif
