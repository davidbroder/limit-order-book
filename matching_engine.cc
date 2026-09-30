#include "matching_engine.hh"
#include <algorithm>
using namespace std;

namespace lob {

vector<Trade> MatchingEngine::match_buy_order(Order& buy_order) {
    vector<Trade> trades;

    while (!buy_order.is_filled()) {
        const Order* best_ask = order_book_.get_best_ask();
        
        if (best_ask == nullptr) {
            break; 
        }

        if (buy_order.get_type() == OrderType::LIMIT && best_ask->get_price() > buy_order.get_price()) {
            break;
        }

        Quantity fill_qty = min(buy_order.get_quantity(), best_ask->get_quantity());
        
        Price fill_price = best_ask->get_price();

        trades.push_back({
            buy_order.get_id(),
            best_ask->get_id(),
            fill_price,
            fill_qty
        });

        // Actualizamos las cantidades
        buy_order.fill(fill_qty);
        
        // Actualizamos el libro
        if (best_ask->get_quantity() == fill_qty) {
            order_book_.remove_best_ask();
        } else {
            order_book_.reduce_best_ask(fill_qty);
        }
    }

    return trades;
}

vector<Trade> MatchingEngine::match_sell_order(Order& sell_order) {
    vector<Trade> trades;

    while (!sell_order.is_filled()) {
        const Order* best_bid = order_book_.get_best_bid();
        
        if (best_bid == nullptr) {
            break; 
        }

        if (sell_order.get_type() == OrderType::LIMIT && best_bid->get_price() < sell_order.get_price()) {
            break;
        }

        Quantity fill_qty = min(sell_order.get_quantity(), best_bid->get_quantity());
        Price fill_price = best_bid->get_price();

        trades.push_back({
            best_bid->get_id(),
            sell_order.get_id(),
            fill_price,
            fill_qty
        });

        sell_order.fill(fill_qty);
        
        if (best_bid->get_quantity() == fill_qty) {
            order_book_.remove_best_bid();
        } 
        else {
            order_book_.reduce_best_bid(fill_qty);
        }
    }

    return trades;
}

vector<Trade> MatchingEngine::process_order(Order order) {
    vector<Trade> trades;

    if (order.get_type() == OrderType::MARKET) {
        if (order.get_side() == Side::BID) {
            trades = match_buy_order(order);
        } 
        else {
            trades = match_sell_order(order);
        }
    } 
    else if (order.get_type() == OrderType::LIMIT) {
        if (order.get_side() == Side::BID) {
            trades = match_buy_order(order);
        } 
        else {
            trades = match_sell_order(order);
        }
        
        if (!order.is_filled()) {
            order_book_.add_order(order);
        }
    }

    return trades;
}

bool MatchingEngine::cancel_order(OrderId id) {
    return order_book_.cancel_order(id);
}

}