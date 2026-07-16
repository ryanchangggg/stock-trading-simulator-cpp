# Stock Trading Simulator (C++)

A C++17 port of the Java OOP Stock Trading Simulator project.
This terminal-based application lets users practice equity trading
with virtual currency using historical OHLCV stock data.

## Architecture

The project mirrors the original Java design, applying the same
object-oriented principles and design patterns:

### Design Patterns
- **Singleton** — implicit single Market instance per session
- **Factory Method** — InMemoryRepository instantiates concrete repositories
- **Strategy** — MarketOrderExecutor, LimitOrderExecutor, StopLossExecutor
- **Observer** — TradeObserver interface for event notifications
- **Repository** — In-memory persistence abstraction (UserRepository, OrderRepository, etc.)
- **MVC (Architectural)** — Model (domain + repos) -> Service (engine + services) -> CLI (views)

### Class Hierarchy
```
Stock (abstract)       -- base security
+-- EquityStock        -- common stock

Account (abstract)     -- financial account
+-- CashAccount        -- standard cash trading account

Order (abstract)       -- order base (MARKET, LIMIT, STOP_LOSS)
+-- BuyOrder           -- buy-specific validation
+-- SellOrder          -- sell-specific validation
+-- StopLossOrder      -- trigger-based execution

Portfolio              -- collection of Position objects
Position               -- single stock holding with avg cost
Trade                  -- immutable completed trade record
Transaction            -- immutable audit trail entry
User                   -- aggregate root (Account + Portfolio)
Market                 -- central stock registry
```

### Service Layer
- **TradingEngine** — core order orchestration with strategy dispatch
- **OrderValidator** — validates orders against business rules (sufficient cash/shares)
- **OrderBook** — in-memory pending order registry
- **AuthenticationService** — user registration and login
- **TradingService** — high-level trading operations
- **PortfolioService** — portfolio queries and summary calculation
- **MarketDataService** — market data queries
- **AppContext** — dependency injection container

## Build & Run

```bash
# Build (requires clang++ or g++ with C++17 support)
cd stock-trading-simulator-cpp
clang++ -std=c++17 -I include -o stock_trading_simulator \
  src/model/stock/Stock.cpp src/model/stock/EquityStock.cpp \
  src/model/account/Account.cpp src/model/account/CashAccount.cpp \
  src/model/order/Order.cpp src/model/order/BuyOrder.cpp \
  src/model/order/SellOrder.cpp src/model/order/StopLossOrder.cpp \
  src/model/portfolio/Position.cpp src/model/portfolio/Portfolio.cpp \
  src/model/trade/Trade.cpp src/model/transaction/Transaction.cpp \
  src/model/market/Market.cpp src/model/user/User.cpp \
  src/strategy/MarketOrderExecutor.cpp src/strategy/LimitOrderExecutor.cpp \
  src/strategy/StopLossExecutor.cpp src/service/ExecutionResult.cpp \
  src/service/OrderBook.cpp src/service/OrderValidator.cpp \
  src/service/MarketDataService.cpp src/service/AuthenticationService.cpp \
  src/service/TradingEngine.cpp src/service/TradingService.cpp \
  src/service/PortfolioService.cpp src/service/AppContext.cpp \
  src/util/CsvDataLoader.cpp src/main.cpp

# Run
./stock_trading_simulator
```

Or with CMake:
```bash
mkdir build && cd build
cmake ..
make
./stock_trading_simulator
```

## Tests

```bash
clang++ -std=c++17 -I include -o run_tests tests/run_tests.cpp \
  src/**/*.cpp src/**/**/*.cpp
./run_tests
```

## CLI Usage

1. **Register** a new account or **Login** with existing credentials
2. **Dashboard** shows cash balance, portfolio value, and P&L
3. **Buy/Sell** with market, limit, or stop-loss orders
4. **View Market** shows all available stocks with current prices
5. **Advance Simulation Tick** moves prices forward (random walk)
6. **Cancel Order** removes a pending limit/stop-loss order

## Data

The `data/stocks/` directory contains 507 CSV files with OHLCV data
for US equities (2013-2018 period). The first row of each file is loaded
on startup to set initial prices.

## Disclaimer

This repository is intended for educational and research purposes only.

The software, code, models, and any accompanying materials do not constitute investment, financial, legal, or tax advice.

All investment decisions are made solely at the user's own risk. The authors assume no responsibility or liability for any financial losses or damages arising from the use of this project.

Past performance, backtesting results, and simulated trading outcomes do not guarantee future performance.

This software is provided "AS IS", without warranty of any kind.

## License

MIT
