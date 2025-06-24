#pragma once

#include <cstdint>
#include <cstddef>

struct Message {
    double timestamp;
    int message_type;
    uint64_t order_id;
    size_t size;
    uint64_t raw_price;
    int side;
    uint64_t entity_id;
};
