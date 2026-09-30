#include "order_book.hh"

namespace lob {
    
void OrderBook::clean_up_level(Side side, Price price) {
    if (side == Side::BID) {
        if (bids_[price].empty()) {
            bids_.erase(price);
        }
    } else {
        if (asks_[price].empty()) {
            asks_.erase(price);
        }
    }
}

void OrderBook::add_order(const Order& order) {
    if (order.get_side() == Side::BID) {
        bids_[order.get_price()].push_back(order);
        
        auto it = bids_[order.get_price()].end();
        --it; 
        order_map_[order.get_id()] = it;
    } else {
        asks_[order.get_price()].push_back(order);
        
        auto it = asks_[order.get_price()].end();
        --it;
        order_map_[order.get_id()] = it;
    }
}

bool OrderBook::cancel_order(OrderId id) {
    auto map_it = order_map_.find(id);
    if (map_it == order_map_.end()) {
        return false; 
    }

    OrderIterator list_it = map_it->second;
    Side side = list_it->get_side();
    Price price = list_it->get_price();

    if (side == Side::BID) {
        bids_[price].erase(list_it);
    } 
    else {
        asks_[price].erase(list_it);
    }

    order_map_.erase(map_it);

    clean_up_level(side, price);

    return true;
}

const Order* OrderBook::get_best_bid() const {
    if (bids_.empty()) return nullptr;
    return &(bids_.begin()->second.front());
}

const Order* OrderBook::get_best_ask() const {
    if (asks_.empty()) return nullptr;
    return &(asks_.begin()->second.front());
}

void OrderBook::remove_best_bid() {
    if (bids_.empty()) return;
    auto& top_level = bids_.begin()->second;
    order_map_.erase(top_level.front().get_id());
    top_level.pop_front();
    if (top_level.empty()) bids_.erase(bids_.begin());
}

void OrderBook::remove_best_ask() {
    if (asks_.empty()) return;
    auto& top_level = asks_.begin()->second;
    order_map_.erase(top_level.front().get_id());
    top_level.pop_front();
    if (top_level.empty()) asks_.erase(asks_.begin());
}

void OrderBook::reduce_best_bid(Quantity qty) {
    if (!bids_.empty()) {
        bids_.begin()->second.front().fill(qty);
    }
}

void OrderBook::reduce_best_ask(Quantity qty) {
    if (!asks_.empty()) {
        asks_.begin()->second.front().fill(qty);
    }
}

}