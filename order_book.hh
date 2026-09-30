#pragma once
#include "order.hh"
#include <map>
#include <unordered_map>
#include <list>
using namespace std;
namespace lob {

class OrderBook {
private:
    using OrderList = list<Order>;
    using OrderIterator = OrderList::iterator;

    map<Price, OrderList, greater<Price>> bids_;

    map<Price, OrderList, less<Price>> asks_;

    unordered_map<OrderId, OrderIterator> order_map_;

    void clean_up_level(Side side, Price price);

public:
    void add_order(const Order& order);

    bool cancel_order(OrderId id);

    const Order* get_best_bid() const;
    const Order* get_best_ask() const;

    void remove_best_bid();
    void remove_best_ask();
    void reduce_best_bid(Quantity qty);
    void reduce_best_ask(Quantity qty);


};

} 