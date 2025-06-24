#include "../inc/zetabook.hpp"
#include "../inc/message.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <deque>

bool ZetaBook::process_message(const Message& message) {
    int message_type = message.message_type;

    switch (message_type) {
        case 1:
            return insert_order(message);
        case 2:
            return cancel_order(message);
        case 3:
            return delete_order(message);
        case 4:
            return execute_visible_order(message);
        case 7:
            return true;
        default:
            return false;
    }
}

std::deque<uint64_t>& ZetaBook::level_queue(int side, uint64_t raw_price) {
  if(side == 1) {
    return bids_[raw_price];
  }
  return asks_[raw_price];
} 

Snapshot ZetaBook::snapshot_top_levels(size_t depth) const {
    Snapshot snapshot;
    
    size_t count = 0;

    for(auto it = asks_.begin(); it != asks_.end() && count < depth; ++it, ++count) {
        uint64_t price = it->first;
        auto& level_q = it->second;

        size_t total_size = 0;
        for(uint64_t order_id : level_q) {
            auto mapit = orders_.find(order_id);
            Order& order = *mapit->second;
            total_size += order.size();
        }

        snapshot.raw_asks.emplace_back(price, total_size);
    }

    for(; count < depth; ++count) {
        snapshot.raw_asks.emplace_back(0, 0);
    }
    
    count = 0;
    for(auto it = bids_.begin(); it != bids_.end() && count < depth; ++it, ++count) {
        uint64_t price = it->first;
        auto& level_q = it->second;

        size_t total_size = 0;
        for(uint64_t order_id : level_q) {
            auto mapit = orders_.find(order_id);
            Order& order = *mapit->second;
            total_size += order.size();
        }

        snapshot.raw_bids.emplace_back(price, total_size);
    }
    
    for(; count < depth; ++count) {
        snapshot.raw_bids.emplace_back(0, 0);
    }

    return snapshot;
}


bool ZetaBook::insert_order(const Message& message) {
    uint64_t order_id = message.order_id;
    auto order_ptr = std::make_unique<Order>(message);

    orders_.emplace(order_id, std::move(order_ptr));

    int side = message.side;
    uint64_t order_price = message.raw_price;

    if(side == 1) {
        bids_[order_price].push_back(order_id);
    } else {
        asks_[order_price].push_back(order_id);
    }
    return true;
}

bool ZetaBook::cancel_order(const Message& message) {
    uint64_t order_id = message.order_id;

    auto it = orders_.find(order_id);
    if(it == orders_.end()) {
        throw std::runtime_error("Order ID " + std::to_string(order_id) + " not present in existing orders");
    }

    Order& order = *it->second;

    order.reduce_size(message.size);

    if(order.size() == 0) {
        std::deque<uint64_t>& level_q = level_queue(order.side(), order.raw_price());
        auto qit = std::find(level_q.begin(), level_q.end(), order_id);

        if(qit == level_q.end()) {
            throw std::runtime_error("Order ID " + std::to_string(order_id) + " not present in price level queue");
        }
        
        level_q.erase(qit);
        orders_.erase(it);
    }
    
    return true;
}

bool ZetaBook::delete_order(const Message& message) {
    uint64_t order_id = message.order_id;

    auto it = orders_.find(order_id);
    if(it == orders_.end()) {
        throw std::runtime_error("Order ID " + std::to_string(order_id) + " not present in existing orders");
    }        

    Order& order = *it->second;

    auto& level_q = level_queue(order.size(), order.raw_price());
    auto qit = std::find(level_q.begin(), level_q.end(), order_id);

    if(qit == level_q.end()) {
        throw std::runtime_error("Order ID " + std::to_string(order_id) + " not present in price level queue");
    }

    level_q.erase(qit);
    orders_.erase(it);

    return true;
}

bool ZetaBook::execute_visible_order(const Message& message) {
    uint64_t order_id = message.order_id;

    auto it = orders_.find(order_id);
    if(it == orders_.end()) {
        throw std::runtime_error("Order ID " + std::to_string(order_id) + " not present in existing orders");
    }        

    Order& order = *it->second;

    order.reduce_size(message.size);
    
    if(order.size() == 0) {
        std::deque<uint64_t>& level_q = level_queue(order.side(), order.raw_price());
        auto qit = std::find(level_q.begin(), level_q.end(), order_id);

        if(qit == level_q.end()) {
            throw std::runtime_error("Order ID " + std::to_string(order_id) + " not present in price level queue");
        }
        
        level_q.erase(qit);
        orders_.erase(it);
    }
    return true;
}
