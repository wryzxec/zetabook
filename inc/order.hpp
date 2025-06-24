#pragma once

#include "message.hpp"

#include <cstdint>
#include <cstddef>
#include <stdexcept>

class Order {
public:
    explicit Order(const Message& message) :
        timestamp_(message.timestamp),
        order_id_(message.order_id),
        size_(message.size),
        raw_price_(message.raw_price),
        side_(message.side),
        entity_id_(message.entity_id) {}

    double timestamp() const { return timestamp_; }
    uint64_t id() const { return order_id_; }
    size_t size() const { return size_; }
    uint64_t raw_price() const { return raw_price_; } 
    int side() const { return side_; }
    uint64_t entity_id() const { return entity_id_; }
    
    void reduce_size(size_t amount) {
        if(amount > size_) {
            throw std::invalid_argument("reduce amount > size.");
        }
        size_ -= amount;
    }

private:
    double timestamp_;
    uint64_t order_id_;
    size_t size_;
    uint64_t raw_price_;
    int side_;
    uint64_t entity_id_;
};
