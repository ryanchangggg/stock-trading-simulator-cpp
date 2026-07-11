#include "model/stock/Stock.h"
#include "model/stock/EquityStock.h"
#include "model/account/Account.h"
#include "model/account/CashAccount.h"
#include "model/order/Order.h"
#include "model/order/BuyOrder.h"
#include "model/order/SellOrder.h"
#include "model/order/StopLossOrder.h"
#include "model/portfolio/Position.h"
#include "model/portfolio/Portfolio.h"
#include "model/trade/Trade.h"
#include "model/trade/TradeType.h"
#include "model/market/Market.h"
#include "model/user/User.h"
#include "model/exception/InsufficientFundsException.h"
#include "model/exception/InvalidOrderException.h"
#include "service/OrderValidator.h"
#include "service/OrderBook.h"
#include "service/TradingEngine.h"
#include "service/ExecutionResult.h"
#include "repository/InMemoryRepositories.h"
#include "strategy/MarketOrderExecutor.h"
#include "strategy/LimitOrderExecutor.h"
#include "strategy/StopLossExecutor.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <memory>
using namespace stock_trading;
static int passed = 0, failed = 0;
static void check(bool cond, const std::string& name) {
    if (cond) { ++passed; std::cout << "  PASS: " << name << "\n"; }
    else { ++failed; std::cout << "  FAIL: " << name << "\n"; }
}
static void approxEqual(double a, double b, const std::string& name) {
    check(std::abs(a - b) < 0.01, name);
}
int main() {
    std::cout << "=== Stock Trading Simulator - C++ Tests ===\n\n";
    // Stock Tests
    {
        EquityStock aapl("AAPL", "Apple Inc.", "Technology", 185.40, 2000000000, 0);
        check(aapl.getSymbol() == "AAPL", "Stock: symbol");
        check(aapl.getCompanyName() == "Apple Inc.", "Stock: company name");
        check(aapl.getAssetType() == "Equity", "Stock: asset type");
        approxEqual(aapl.getCurrentPrice(), 185.40, "Stock: initial price");
        aapl.setCurrentPrice(190.00);
        approxEqual(aapl.getCurrentPrice(), 190.00, "Stock: price update");
    }
    // Account Tests
    {
        CashAccount acc(50000.0);
        approxEqual(acc.getBalance(), 50000.0, "Account: initial balance");
        acc.deposit(1000.0);
        approxEqual(acc.getBalance(), 51000.0, "Account: deposit");
        acc.withdraw(10000.0);
        approxEqual(acc.getBalance(), 41000.0, "Account: withdraw");
        bool threw = false;
        try { acc.withdraw(999999.0); } catch (const InsufficientFundsException&) { threw = true; }
        check(threw, "Account: insufficient funds throws");
    }
    {
        CashAccount defaultAcc;
        approxEqual(defaultAcc.getBalance(), 100000.0, "Account: default starting balance");
    }
    // Order Tests
    {
        auto order = std::make_unique<BuyOrder>(1, 1, "AAPL", 100, 185.40, OrderType::MARKET);
        check(order->getId() == 1, "Order: id");
        check(order->getDescription() == "Market Buy", "Order: market buy description");
        approxEqual(order->getTotalValue(), 18540.0, "Order: total value");
        order->fill();
        check(order->getStatus() == OrderStatus::FILLED, "Order: fill status");
        bool threw = false;
        try { order->cancel(); } catch (const std::logic_error&) { threw = true; }
        check(threw, "Order: cannot cancel filled order");
    }
    {
        auto order = std::make_unique<BuyOrder>(2, 1, "AAPL", 10, 180.0, OrderType::LIMIT);
        check(order->getDescription() == "Limit Buy", "Order: limit buy description");
    }
    {
        auto order = std::make_unique<StopLossOrder>(3, 1, "AAPL", 20, 170.0, TradeType::SELL);
        check(order->getDescription() == "Stop-Loss SELL", "Order: stop-loss description");
        check(order->getStopPrice() == 170.0, "Order: stop price");
        check(!order->shouldTrigger(180.0), "Order: stop not triggered above price");
        check(order->shouldTrigger(170.0), "Order: stop triggered at price");
        check(order->shouldTrigger(169.0), "Order: stop triggered below price");
        order->trigger();
        check(order->isTriggered(), "Order: triggered flag set");
    }
    // Position Tests
    {
        Position pos("AAPL", 100, 150.0);
        approxEqual(pos.getTotalCost(), 15000.0, "Position: total cost");
        approxEqual(pos.getMarketValue(200.0), 20000.0, "Position: market value");
        approxEqual(pos.getUnrealisedPnl(200.0), 5000.0, "Position: unrealised profit");
        approxEqual(pos.getUnrealisedPnl(100.0), -5000.0, "Position: unrealised loss");
        pos.addShares(100, 200.0);
        approxEqual(pos.getQuantity(), 200, "Position: add shares quantity");
        approxEqual(pos.getAverageCost(), 175.0, "Position: weighted avg cost");
        pos.removeShares(50);
        approxEqual(pos.getQuantity(), 150, "Position: remove shares quantity");
        check(!pos.isEmpty(), "Position: not empty after partial remove");
        pos.removeShares(150);
        check(pos.isEmpty(), "Position: empty after full remove");
    }
    // Portfolio Tests
    {
        Portfolio port(1);
        check(port.getPositionCount() == 0, "Portfolio: initial empty");
        port.addShares("AAPL", 100, 150.0);
        check(port.holdsSymbol("AAPL"), "Portfolio: holds symbol after add");
        port.addShares("AAPL", 100, 200.0);
        check(port.getPosition("AAPL")->get().getQuantity() == 200, "Portfolio: combined quantity");
        port.addShares("GOOG", 50, 250.0);
        check(port.getPositionCount() == 2, "Portfolio: two positions");
        port.removeShares("AAPL", 200);
        check(!port.holdsSymbol("AAPL"), "Portfolio: removed symbol");
        bool threw = false;
        try { port.removeShares("AAPL", 1); } catch (const InvalidOrderException&) { threw = true; }
        check(threw, "Portfolio: remove from empty throws");
    }
    // Trade Tests
    {
        Trade trade(1, 1, "AAPL", TradeType::BUY, 100, 150.0);
        check(trade.getTradeTypeName() == "BUY", "Trade: type name");
        approxEqual(trade.getTotalValue(), 15000.0, "Trade: total value");
    }
    // User Tests
    {
        User user(1, "trader1", "hash123", 50000.0);
        approxEqual(user.getCashBalance(), 50000.0, "User: initial balance");
        user.deposit(1000.0);
        approxEqual(user.getCashBalance(), 51000.0, "User: deposit");
        user.withdraw(10000.0);
        approxEqual(user.getCashBalance(), 41000.0, "User: withdraw");
        check(user.canAfford(30000.0), "User: can afford");
        check(!user.canAfford(99999.0), "User: cannot afford");
        user.getPortfolio().addShares("AAPL", 50, 150.0);
        check(user.holdsAtLeast("AAPL", 50), "User: holds exact quantity");
        check(user.holdsAtLeast("AAPL", 30), "User: holds more than quantity");
        check(!user.holdsAtLeast("AAPL", 51), "User: holds less than quantity");
        check(!user.holdsAtLeast("GOOG", 1), "User: does not hold symbol");
    }
    // Market Tests
    {
        Market market;
        check(market.isEmpty(), "Market: initially empty");
        auto s1 = std::make_shared<EquityStock>("AAPL", "Apple", "Tech", 150.0, 0, 0);
        auto s2 = std::make_shared<EquityStock>("GOOG", "Google", "Tech", 200.0, 0, 0);
        market.addStock(s1);
        market.addStock(s2);
        check(market.getStockCount() == 2, "Market: added stocks");
        approxEqual(market.getCurrentPrice("AAPL"), 150.0, "Market: get price");
        market.updatePrice("AAPL", 160.0);
        approxEqual(market.getCurrentPrice("AAPL"), 160.0, "Market: update price");
        auto symbols = market.getAllSymbols();
        check(symbols.size() == 2, "Market: symbol list");
    }
    // Trading Engine Tests (with in-memory repos)
    {
        InMemoryUserRepository userRepo;
        InMemoryOrderRepository orderRepo;
        InMemoryTradeRepository tradeRepo;
        auto userPtr = std::make_unique<User>(1, "testuser", "hash", 100000.0);
        userPtr->getPortfolio().addShares("AAPL", 50, 180.0);
        auto* user = &userRepo.save(std::move(userPtr));
        Market market;
        market.addStock(std::make_shared<EquityStock>("AAPL", "Apple", "Tech", 185.40, 0, 0));
        TradingEngine engine(orderRepo, tradeRepo, userRepo);
        // Market buy
        {
            auto order = std::make_unique<BuyOrder>(1, 1, "AAPL", 10, 185.40, OrderType::MARKET);
            auto result = engine.placeOrder(std::move(order), *user, market);
            check(result.success, "Engine: market buy success");
            approxEqual(user->getCashBalance(), 98146.0, "Engine: cash reduced after buy");
            check(user->getPortfolio().holdsSymbol("AAPL"), "Engine: holds AAPL after buy");
            check(user->getPortfolio().getPosition("AAPL")->get().getQuantity() == 60, "Engine: position increased");
        }
        // Market sell
        {
            auto order = std::make_unique<SellOrder>(2, 1, "AAPL", 20, 185.40, OrderType::MARKET);
            auto result = engine.placeOrder(std::move(order), *user, market);
            check(result.success, "Engine: market sell success");
            approxEqual(user->getCashBalance(), 101854.0, "Engine: cash increased after sell");
            check(user->getPortfolio().getPosition("AAPL")->get().getQuantity() == 40, "Engine: position decreased");
        }
        // Insufficient shares
        {
            auto order = std::make_unique<SellOrder>(3, 1, "AAPL", 999, 185.40, OrderType::MARKET);
            auto result = engine.placeOrder(std::move(order), *user, market);
            check(!result.success, "Engine: sell too many rejected");
        }
        // Limit order pending
        {
            auto order = std::make_unique<BuyOrder>(4, 1, "AAPL", 10, 180.0, OrderType::LIMIT);
            auto result = engine.placeOrder(std::move(order), *user, market);
            check(result.success, "Engine: limit buy accepted");
            check(result.finalStatus == OrderStatus::PENDING, "Engine: limit buy pending");
            check(engine.getPendingOrderCount() == 1, "Engine: one pending order");
        }
        // Limit order fills on price change
        {
            market.updatePrice("AAPL", 179.0);
            auto results = engine.processPendingOrders(market);
            check(!results.empty(), "Engine: limit fills on tick");
            if (!results.empty()) {
                check(results[0].finalStatus == OrderStatus::FILLED, "Engine: limit filled status");
            }
        }
        // Stop-loss
        {
            auto order = std::make_unique<StopLossOrder>(5, 1, "AAPL", 10, 170.0, TradeType::SELL);
            auto result = engine.placeOrder(std::move(order), *user, market);
            check(result.success, "Engine: stop-loss accepted");
            check(engine.getPendingOrderCount() == 1, "Engine: stop-loss pending");
            market.updatePrice("AAPL", 170.0);
            auto results = engine.processPendingOrders(market);
            check(!results.empty(), "Engine: stop-loss triggers at price");
        }
        // Cancel order
        {
            auto order = std::make_unique<BuyOrder>(6, 1, "AAPL", 10, 150.0, OrderType::LIMIT);
            engine.placeOrder(std::move(order), *user, market);
            check(engine.getPendingOrderCount() == 1, "Engine: new pending order");
            check(engine.cancelOrder(6, *user), "Engine: cancel success");
            check(engine.getPendingOrderCount() == 0, "Engine: no pending after cancel");
        }
    }
    // Order book tests
    {
        OrderBook book;
        book.add(std::make_unique<BuyOrder>(1, 1, "AAPL", 10, 180.0, OrderType::LIMIT));
        book.add(std::make_unique<SellOrder>(2, 1, "AAPL", 5, 190.0, OrderType::LIMIT));
        check(book.size() == 2, "OrderBook: size after adds");
        auto* found = book.remove(1);
        check(found != nullptr, "OrderBook: remove returns non-null");
        check(book.size() == 1, "OrderBook: size after remove");
        book.clear();
        check(book.isEmpty(), "OrderBook: empty after clear");
    }
    // Execution result tests
    {
        auto order = std::make_unique<BuyOrder>(1, 1, "AAPL", 10, 185.0, OrderType::MARKET);
        auto trade = std::make_unique<Trade>(1, 1, "AAPL", TradeType::BUY, 10, 185.0);
        auto result = ExecutionResult::filled(*order, std::move(trade));
        check(result.success, "ExecutionResult: filled success");
        check(result.finalStatus == OrderStatus::FILLED, "ExecutionResult: filled status");
        auto pending = ExecutionResult::pending(*order);
        check(pending.success, "ExecutionResult: pending success");
        check(pending.trade == nullptr, "ExecutionResult: no trade for pending");
        auto rejected = ExecutionResult::rejected(*order, "No cash");
        check(!rejected.success, "ExecutionResult: rejected");
    }
    std::cout << "\n=== Results: " << passed << " passed, " << failed << " failed ===\n";
    return failed > 0 ? 1 : 0;
}
