#ifndef STOCK_TRADING_ORDER_H
#define STOCK_TRADING_ORDER_H

#include "model/order/OrderType.h"
#include "model/order/OrderStatus.h"
#include "model/trade/TradeType.h"
#include <string>
#include <memory>
#include <chrono>

namespace stock_trading {

class Order {
public:
    Order(long id, long userId, std::string symbol, int quantity,
          double price, OrderType orderType, TradeType tradeType);
    virtual ~Order() = default;

    long getId() const { return id_; }
    long getUserId() const { return userId_; }
    const std::string& getSymbol() const { return symbol_; }
    int getQuantity() const { return quantity_; }
    double getPrice() const { return price_; }
    double getTotalValue() const { return price_ * quantity_; }
   OrderType getOrderType() const { return orderType_; }
   TradeType getTradeType() const { return tradeType_; }
   OrderStatus getStatus() const { return status_; }
    const std::string& getCreatedAt() const { return createdAt_; }
    const std::string& getFilledAt() const { return filledAt_; }

   void fill();
   void cancel();

   virtual void validate() = 0;
   virtual std::string getDescription() const = 0;

    void restoreStatus(OrderStatus status, const std::string& filledAt = "");

   bool operator==(const Order& other) const { return id_ == other.id_; }

protected:
    long id_;
    long userId_;
    std::string symbol_;
    int quantity_ = 0;
    double price_ = 0.0;
    OrderType orderType_;
    TradeType tradeType_;
   OrderStatus status_ = OrderStatus::PENDING;
    std::string createdAt_;
    std::string filledAt_;
};

}  // namespace stock_trading

#endif
