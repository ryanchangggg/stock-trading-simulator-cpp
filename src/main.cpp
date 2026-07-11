#include "service/AppContext.h"
#include "repository/InMemoryRepositories.h"
#include "model/order/BuyOrder.h"
#include "model/order/SellOrder.h"
#include "model/order/StopLossOrder.h"
#include "model/trade/Trade.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <thread>
#include <chrono>
#include <atomic>
#include <ctime>
using namespace stock_trading;
static void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}
static void printHeader(const std::string& title) {
    std::cout << "\n============================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "============================================\n";
}
static void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
static std::string formatPrice(double p) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "$" << p;
    return oss.str();
}
static void showMarket(Market& market) {
    printHeader("Market Overview");
    auto stocks = market.getAllStocks();
    if (stocks.empty()) { std::cout << "  No stocks loaded.\n"; return; }
    std::cout << "  " << std::left << std::setw(8) << "Symbol"
              << std::setw(30) << "Company"
              << std::setw(12) << "Price" << "\n";
    std::cout << "  " << std::string(50, '-') << "\n";
    for (const auto& s : stocks) {
        std::cout << "  " << std::left << std::setw(8) << s->getSymbol()
                  << std::setw(30) << s->getCompanyName()
                  << std::right << std::setw(10) << formatPrice(s->getCurrentPrice()) << "\n";
    }
}
static void showPortfolio(PortfolioService& ps, Session& session, Market& market) {
    printHeader("Your Portfolio");
    auto balResult = ps.getCashBalance(session);
    if (balResult.isSuccess())
        std::cout << "  Cash Balance: " << formatPrice(balResult.getValue()) << "\n\n";
    auto summaryResult = ps.getSummary(session);
    if (summaryResult.isSuccess()) {
        auto& s = summaryResult.getValue();
        std::cout << "  Holdings Value: " << formatPrice(s.totalValue) << "\n";
        std::cout << "  Total Cost: " << formatPrice(s.totalCost) << "\n";
        std::cout << "  Unrealised P&L: " << formatPrice(s.unrealisedPnl);
        if (s.unrealisedPnl >= 0) std::cout << " (+" << s.unrealisedPnlPercent << "%)";
        else std::cout << " (" << s.unrealisedPnlPercent << "%)";
        std::cout << "\n  Positions: " << s.positionCount
                  << " | Total Shares: " << s.totalShares << "\n\n";
    }
    auto portResult = ps.getPortfolio(session);
    if (portResult.isSuccess()) {
        auto* portfolio = portResult.getValue();
        if (portfolio->getPositions().empty()) {
            std::cout << "  No holdings yet.\n";
        } else {
            std::cout << "  " << std::left << std::setw(8) << "Symbol"
                      << std::setw(10) << "Qty"
                      << std::setw(12) << "Avg Cost"
                      << std::setw(14) << "Mkt Value"
                      << std::setw(12) << "P&L" << "\n";
            std::cout << "  " << std::string(56, '-') << "\n";
            for (const auto& pos : portfolio->getPositions()) {
                double currentPrice = market.getCurrentPrice(pos.getSymbol());
                std::cout << "  " << std::left << std::setw(8) << pos.getSymbol()
                          << std::right << std::setw(8) << pos.getQuantity()
                          << std::setw(12) << formatPrice(pos.getAverageCost())
                          << std::setw(14) << formatPrice(pos.getMarketValue(currentPrice))
                          << std::setw(12) << formatPrice(pos.getUnrealisedPnl(currentPrice)) << "\n";
            }
        }
    }
}
static void showOrders(TradingService& ts, Session& session) {
    printHeader("Your Orders");
    auto pendingResult = ts.getPendingOrders(session);
    if (pendingResult.isSuccess()) {
        auto orders = pendingResult.getValue();
        if (orders.empty()) { std::cout << "  No pending orders.\n"; }
        else {
            std::cout << "  Pending Orders:\n";
            for (auto* o : orders) {
                std::cout << "  [" << o->getId() << "] " << o->getDescription()
                          << " " << o->getSymbol() << " x" << o->getQuantity()
                          << " @ " << formatPrice(o->getPrice())
                          << " (" << orderStatusToString(o->getStatus()) << ")\n";
            }
        }
    }
    auto histResult = ts.getTradeHistory(session);
    if (histResult.isSuccess()) {
        auto trades = histResult.getValue();
        if (!trades.empty()) {
            std::cout << "\n  Trade History:\n";
            std::cout << "  " << std::left << std::setw(6) << "ID"
                      << std::setw(8) << "Type"
                      << std::setw(8) << "Symbol"
                      << std::setw(8) << "Qty"
                      << std::setw(12) << "Price"
                      << std::setw(14) << "Total" << "\n";
            std::cout << "  " << std::string(56, '-') << "\n";
            for (auto* t : trades) {
                std::cout << "  " << std::left << std::setw(6) << t->getId()
                          << std::setw(8) << t->getTradeTypeName()
                          << std::setw(8) << t->getSymbol()
                          << std::right << std::setw(6) << t->getQuantity()
                          << std::setw(12) << formatPrice(t->getPrice())
                          << std::setw(14) << formatPrice(t->getTotalValue()) << "\n";
            }
        }
    }
}
static void placeOrder(TradingService& ts, Session& session, Market& market,
                        const std::string& mode, const std::string& side) {
    std::string symbol;
    int qty;
    double price = 0.0;
    std::cout << "  Enter symbol: ";
    std::cin >> symbol;
    std::cout << "  Enter quantity: ";
    std::cin >> qty;
    if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); std::cout << "  Invalid input.\n"; return; }
    if (mode == "LIMIT") {
        std::cout << "  Enter limit price: $";
        std::cin >> price;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); std::cout << "  Invalid input.\n"; return; }
    } else if (mode == "STOP") {
        std::cout << "  Enter stop price: $";
        std::cin >> price;
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); std::cout << "  Invalid input.\n"; return; }
    }
    std::cin.ignore();
    if (mode == "MARKET" && side == "BUY") {
        auto r = ts.buyMarket(session, symbol, qty);
        if (r.isSuccess()) std::cout << "  Market buy order placed successfully!\n";
        else std::cout << "  Error: " << r.getErrorMessage() << "\n";
    } else if (mode == "MARKET" && side == "SELL") {
        auto r = ts.sellMarket(session, symbol, qty);
        if (r.isSuccess()) std::cout << "  Market sell order placed successfully!\n";
        else std::cout << "  Error: " << r.getErrorMessage() << "\n";
    } else if (mode == "LIMIT" && side == "BUY") {
        auto r = ts.buyLimit(session, symbol, qty, price);
        if (r.isSuccess()) std::cout << "  Limit buy placed (pending at " << formatPrice(price) << ")\n";
        else std::cout << "  Error: " << r.getErrorMessage() << "\n";
    } else if (mode == "LIMIT" && side == "SELL") {
        auto r = ts.sellLimit(session, symbol, qty, price);
        if (r.isSuccess()) std::cout << "  Limit sell placed (pending at " << formatPrice(price) << ")\n";
        else std::cout << "  Error: " << r.getErrorMessage() << "\n";
    } else if (mode == "STOP" && side == "BUY") {
        auto r = ts.buyStopLoss(session, symbol, qty, price);
        if (r.isSuccess()) std::cout << "  Buy stop-loss placed (triggers at " << formatPrice(price) << ")\n";
        else std::cout << "  Error: " << r.getErrorMessage() << "\n";
    } else if (mode == "STOP" && side == "SELL") {
        auto r = ts.sellStopLoss(session, symbol, qty, price);
        if (r.isSuccess()) std::cout << "  Sell stop-loss placed (triggers at " << formatPrice(price) << ")\n";
        else std::cout << "  Error: " << r.getErrorMessage() << "\n";
    }
}
static void cancelOrder(TradingService& ts, Session& session) {
    std::cout << "  Enter order ID to cancel: ";
    long id;
    std::cin >> id;
    std::cin.ignore();
    if (std::cin.fail()) { std::cin.clear(); std::cout << "  Invalid input.\n"; return; }
    auto result = ts.cancelOrder(session, id);
    if (result.isSuccess()) std::cout << "  Order cancelled.\n";
    else std::cout << "  Error: " << result.getErrorMessage() << "\n";
}
static void advanceSimulation(Market& market, TradingEngine& engine) {
    printHeader("Advancing Simulation");
    int count = 0;
    auto stocks = market.getAllStocks();
    for (auto& s : stocks) {
        double curr = s->getCurrentPrice();
        double change = (std::rand() % 1001 - 500) / 10000.0;
        double newPrice = curr * (1.0 + change);
        if (newPrice < 0.01) newPrice = 0.01;
        market.updatePrice(s->getSymbol(), newPrice);
        ++count;
    }
    auto results = engine.processPendingOrders(market);
    std::cout << "  Advanced " << count << " stock prices.\n";
    if (!results.empty()) {
        for (auto& r : results) {
            if (r.trade) {
                std::cout << "  Order filled: " << r.trade->getSymbol()
                          << " x" << r.trade->getQuantity()
                          << " @ " << formatPrice(r.trade->getPrice()) << "\n";
            }
        }
    } else {
        std::cout << "  No pending orders were filled.\n";
    }
}
static void tradingMenu(TradingService& ts, PortfolioService& ps,
                         Market& market, Session& session, TradingEngine& engine) {
    int choice = -1;
    do {
        clearScreen();
        printHeader("Stock Trading Simulator - Dashboard");
        auto cash = ps.getCashBalance(session);
        if (cash.isSuccess())
            std::cout << "  User: " << session.getUsername()
                      << " | Cash: " << formatPrice(cash.getValue()) << "\n";
        auto summary = ps.getSummary(session);
        if (summary.isSuccess()) {
            auto& s = summary.getValue();
            std::cout << "  Portfolio Value: " << formatPrice(s.totalValue + cash.getValue())
                      << " | P&L: " << formatPrice(s.unrealisedPnl);
            if (s.unrealisedPnl >= 0) std::cout << " (+" << s.unrealisedPnlPercent << "%)";
            else std::cout << " (" << s.unrealisedPnlPercent << "%)";
            std::cout << "\n";
        }
        std::cout << "\n  [1] View Market\n";
        std::cout << "  [2] Buy (Market)\n";
        std::cout << "  [3] Buy (Limit)\n";
        std::cout << "  [4] Buy (Stop-Loss)\n";
        std::cout << "  [5] Sell (Market)\n";
        std::cout << "  [6] Sell (Limit)\n";
        std::cout << "  [7] Sell (Stop-Loss)\n";
        std::cout << "  [8] View Portfolio\n";
        std::cout << "  [9] View Orders & History\n";
        std::cout << "  [10] Cancel Order\n";
        std::cout << "  [11] Advance Simulation Tick\n";
        std::cout << "  [0] Logout\n";
        std::cout << "\n  Choice: ";
        std::cin >> choice;
        std::cin.ignore();
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); choice = -1; }
        clearScreen();
        switch (choice) {
            case 1: showMarket(market); waitForEnter(); break;
            case 2: placeOrder(ts, session, market, "MARKET", "BUY"); waitForEnter(); break;
            case 3: placeOrder(ts, session, market, "LIMIT", "BUY"); waitForEnter(); break;
            case 4: placeOrder(ts, session, market, "STOP", "BUY"); waitForEnter(); break;
            case 5: placeOrder(ts, session, market, "MARKET", "SELL"); waitForEnter(); break;
            case 6: placeOrder(ts, session, market, "LIMIT", "SELL"); waitForEnter(); break;
            case 7: placeOrder(ts, session, market, "STOP", "SELL"); waitForEnter(); break;
            case 8: showPortfolio(ps, session, market); waitForEnter(); break;
            case 9: showOrders(ts, session); waitForEnter(); break;
            case 10: cancelOrder(ts, session); waitForEnter(); break;
            case 11: advanceSimulation(market, engine); waitForEnter(); break;
            case 0: std::cout << "  Logging out...\n"; return;
            default: std::cout << "  Invalid choice.\n"; waitForEnter();
        }
    } while (choice != 0);
}
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    auto userRepo = std::make_unique<InMemoryUserRepository>();
    auto orderRepo = std::make_unique<InMemoryOrderRepository>();
    auto portfolioRepo = std::make_unique<InMemoryPortfolioRepository>();
    auto tradeRepo = std::make_unique<InMemoryTradeRepository>();
    AppContext context(std::move(userRepo), std::move(orderRepo),
                       std::move(portfolioRepo), std::move(tradeRepo));
    context.loadMarketData("data/stocks");
    std::cout << "Loaded " << context.getMarket().getStockCount()
              << " stocks from data/stocks\n";
    int mainChoice = -1;
    std::shared_ptr<Session> currentSession;
    do {
        clearScreen();
        printHeader("Stock Trading Simulator");
        std::cout << "  [1] Register\n";
        std::cout << "  [2] Login\n";
        std::cout << "  [0] Exit\n";
        std::cout << "\n  Choice: ";
        std::cin >> mainChoice;
        std::cin.ignore();
        if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); mainChoice = -1; }
        if (mainChoice == 1 || mainChoice == 2) {
            std::string username, password;
            std::cout << "  Username: ";
            std::getline(std::cin, username);
            std::cout << "  Password: ";
            std::getline(std::cin, password);
            if (mainChoice == 1) {
                auto result = context.getAuthService().registerUser(username, password);
                if (result.isSuccess()) {
                    currentSession = result.getValue();
                    std::cout << "  Registration successful! Welcome, " << username << ".\n";
                } else {
                    std::cout << "  Error: " << result.getErrorMessage() << "\n";
                }
            } else {
                auto result = context.getAuthService().login(username, password);
                if (result.isSuccess()) {
                    currentSession = result.getValue();
                    std::cout << "  Login successful! Welcome back, " << username << ".\n";
                } else {
                    std::cout << "  Error: " << result.getErrorMessage() << "\n";
                }
            }
            if (currentSession) {
                waitForEnter();
                tradingMenu(context.getTradingService(), context.getPortfolioService(),
                            context.getMarket(), *currentSession, context.getEngine());
                currentSession.reset();
            } else {
                waitForEnter();
            }
        }
    } while (mainChoice != 0);
    printHeader("Goodbye!");
    return 0;
}
