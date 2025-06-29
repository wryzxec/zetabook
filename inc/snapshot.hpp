#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

struct Snapshot {
    std::vector<std::pair<uint64_t, size_t>> raw_asks;
    std::vector<std::pair<uint64_t, size_t>> raw_bids;

    // formatting constants
    static constexpr int precision = 3;
    static constexpr int col_spacing = 20;

    std::string to_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision);
        oss << std::left;

        assert(raw_asks.size() == raw_bids.size());
        size_t asks_size = raw_asks.size();
        
        oss << std::setw(col_spacing) << "Size";
        oss << std::setw(col_spacing) << "Price";
        oss << std::setw(col_spacing) << "Price";
        oss << std::setw(col_spacing) << "Size" << "\n";

        for(int i = 0; i < asks_size ; ++i) {
            oss << std::setw(col_spacing) << raw_asks[i].second;
            oss << std::setw(col_spacing) << "£" + std::to_string(raw_asks[i].first / 10000.0);
            oss << std::setw(col_spacing) << "£" + std::to_string(raw_bids[i].first / 10000.0);
            oss << std::setw(col_spacing) << raw_bids[i].second;
            oss << std::endl;
        }

        return oss.str();
    }
};
