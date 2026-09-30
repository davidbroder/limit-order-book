#ifndef LOB_MATCHING_ENGINE_HH
#define LOB_MATCHING_ENGINE_HH

#include "order_book.hh"
#include "types.hh"
#include <vector>
using namespace std;

namespace lob {

class MatchingEngine {
private:
    OrderBook order_book_;

    vector<Trade> match_buy_order(Order& order);
    vector<Trade> match_sell_order(Order& order);
    
public:
    vector<Trade> process_order(Order order);

    bool cancel_order(OrderId id);


};

}

#endif