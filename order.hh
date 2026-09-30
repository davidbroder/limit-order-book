#pragma once
#include "types.hh"
#include <chrono>
using namespace std;

namespace lob {

class Order {
private:
    static uint64_t generate_timestamp() {
        return chrono::duration_cast<chrono::nanoseconds>(
                   chrono::steady_clock::now().time_since_epoch())
            .count();
    }
    
    OrderId   id_;           // 8 bytes
    Price     price_;        // 8 bytes
    uint64_t  timestamp_;    // 8 bytes
    Quantity  quantity_;     // 4 bytes
    Side      side_;         // 1 byte 
    OrderType type_;         // 1 byte 

public:
    Order(OrderId id, Side side, OrderType type, Price price, Quantity quantity)
        : id_(id), 
          price_(price), 
          timestamp_(generate_timestamp()), 
          quantity_(quantity), 
          side_(side), 
          type_(type) {}

    OrderId get_id() const { return id_; }
    Price get_price() const { return price_; }
    uint64_t get_timestamp() const { return timestamp_; }
    Quantity get_quantity() const { return quantity_; }
    Side get_side() const { return side_; }
    OrderType get_type() const { return type_; }

    bool is_filled() const { return quantity_ == 0; }
    
    void fill(Quantity executed_quantity) {
        if (executed_quantity > quantity_) {
            quantity_ = 0; 
        } 
        else {
            quantity_ -= executed_quantity;
        }
    }


};

} 