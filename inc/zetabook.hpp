#pragma once

#include "message.hpp"
#include "order.hpp"
#include "snapshot.hpp"

#include <cstdint>
#include <deque>
#include <memory>
#include <map>

class ZetaBook {

using OrderID = uint64_t;
using Price = uint64_t;

public:
    bool process_message(const Message& Message);
    
    bool insert_order(const Message& message);
    bool cancel_order(const Message& message);
    bool delete_order(const Message& message);
    bool execute_visible_order(const Message& message);
    
    std::deque<OrderID>& level_queue(int side, uint64_t raw_price);
    Snapshot snapshot_top_levels(size_t depth) const;
private:

    std::unordered_map<OrderID, std::unique_ptr<Order>> orders_;
    std::map<Price, std::deque<OrderID>> asks_;
    std::map<Price, std::deque<OrderID>, std::greater<>> bids_;
};
