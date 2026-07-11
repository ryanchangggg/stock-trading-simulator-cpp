#include "util/CsvDataLoader.h"
#include "model/stock/EquityStock.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
namespace fs = std::filesystem;
namespace stock_trading {
CsvDataLoader::CsvDataLoader(Market& market) : market_(market) {}
void CsvDataLoader::loadAll(const std::string& dataDir) {
    fs::path dir(dataDir);
    if (!fs::is_directory(dir)) return;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() == ".csv") {
            loadFile(entry.path().string());
        }
    }
}
void CsvDataLoader::loadFile(const std::string& path) {
    try {
        fs::path p(path);
        std::string symbol = p.stem().string();
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper);
        std::ifstream file(path);
        if (!file.is_open()) return;
        std::string line;
        std::string lastLine;
        bool header = true;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (header) { header = false; continue; }
            lastLine = line;
        }
        if (lastLine.empty()) return;
        std::istringstream ss(lastLine);
        std::string token;
        int col = 0;
        double close = 0.0;
        while (std::getline(ss, token, ',')) {
            if (col == 4) { close = std::stod(token); break; }
            ++col;
        }
        auto stock = std::make_shared<EquityStock>(symbol, symbol, "N/A", close, 0, 0);
        market_.addStock(std::move(stock));
    } catch (const std::exception& e) {
        std::cerr << "Skipping " << path << ": " << e.what() << std::endl;
    }
}
}  // namespace stock_trading
