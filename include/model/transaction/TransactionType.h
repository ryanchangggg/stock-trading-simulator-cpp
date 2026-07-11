#ifndef STOCK_TRADING_TRANSACTION_TYPE_H
#define STOCK_TRADING_TRANSACTION_TYPE_H

#include <string>

namespace stock_trading {

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRADE_SETTLEMENT,
    FEE
};

inline std::string transactionTypeToString(TransactionType t) {
    switch (t) {
        case TransactionType::DEPOSIT: return "DEPOSIT";
        case TransactionType::WITHDRAWAL: return "WITHDRAWAL";
        case TransactionType::TRADE_SETTLEMENT: return "TRADE_SETTLEMENT";
        case TransactionType::FEE: return "FEE";
    }
    return "UNKNOWN";
}

}  // namespace stock_trading

#endif
