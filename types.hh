#pragma once
#include <cstdint>

namespace lob {

using OrderId = uint64_t;
using Price = uint64_t;      
using Quantity = uint32_t;    


enum class Side : uint8_t {
    BID = 0, 
    ASK = 1   
};

enum class OrderType : uint8_t {
    MARKET = 0, 
    LIMIT = 1   
};

struct Trade {
    OrderId buyer_order_id;
    OrderId seller_order_id;
    Price match_price;
    Quantity match_quantity;
    
};

}