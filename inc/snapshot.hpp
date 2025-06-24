#pragma once

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
struct Snapshot {
    std::vector<std::pair<uint64_t, size_t>> raw_asks;
    std::vector<std::pair<uint64_t, size_t>> raw_bids;

    size_t precision = 4;

    std::string to_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision);

        oss << "Asks: ";
        for(int i = 0; i < raw_asks.size(); ++i) {
            double price = static_cast<double>(raw_asks[i].first) / 10000.0;
            oss << price << "x" << raw_asks[i].second;
            if(i + 1 < raw_asks.size()) {
                oss << ", ";
            }
        }

        oss << " | bids: ";
        for(int i = 0; i < raw_bids.size(); ++i) {
            double price = static_cast<double>(raw_bids[i].first) / 10000.0;
            oss << price << "x" << raw_bids[i].second;
            if(i + 1 < raw_bids.size()) {
                oss << ", ";
            }
        }

        return oss.str();
    }
};
