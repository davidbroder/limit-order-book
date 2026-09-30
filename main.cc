#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "matching_engine.hh"
#include "order.hh"
#include "types.hh"
using namespace std;

// Helper function to parse a CSV file containing pre-generated orders
vector<lob::Order> load_orders_from_csv(const string& file_path) {
    vector<lob::Order> orders;
    ifstream file(file_path);
    
    if (!file.is_open()) {
        cerr << "[Error] Could not open benchmark file: " << file_path << "\n";
        return orders;
    }

    string line;
    lob::OrderId current_id = 1;
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty lines or comments

        stringstream ss(line);
        string type_str, side_str, price_str, qty_str;

        if (getline(ss, type_str, ',') &&
            getline(ss, side_str, ',') &&
            getline(ss, price_str, ',') &&
            getline(ss, qty_str, ',')) {

            lob::OrderType type = (type_str == "LIMIT") ? lob::OrderType::LIMIT : lob::OrderType::MARKET;
            lob::Side side = (side_str == "BID") ? lob::Side::BID : lob::Side::ASK;
            lob::Quantity quantity = static_cast<lob::Quantity>(stoul(qty_str));
            lob::Price price = 0;

            if (type == lob::OrderType::LIMIT) {
                double price_decimal = stod(price_str);
                price = static_cast<lob::Price>(price_decimal * 100.0);
            }

            orders.emplace_back(current_id++, side, type, price, quantity);
        }
    }

    file.close();
    return orders;
}

int main(int argc, char* argv[]) {
    string benchmark_file = "orders_benchmark.csv";
    if (argc > 1) {
        benchmark_file = argv[1];
    }

    cout << "========================================\n";
    cout << "   HFT LIMIT ORDER BOOK - BENCHMARK     \n";
    cout << "========================================\n";
    cout << "Loading orders from: " << benchmark_file << "...\n";

    vector<lob::Order> orders_to_process = load_orders_from_csv(benchmark_file);

    if (orders_to_process.empty()) {
        cout << "[Warning] No orders loaded. Please ensure the CSV file exists and is correctly formatted.\n";
        cout << "Format: TYPE(LIMIT/MARKET),SIDE(BID/ASK),PRICE(e.g. 100.50),QUANTITY(e.g. 10)\n";
        return 1;
    }

    cout << "Successfully loaded " << orders_to_process.size() << " orders.\n";
    cout << "Starting high-performance matching engine benchmark...\n\r";

    lob::MatchingEngine engine;
    
    // High-resolution timing using std::chrono
    auto start_time = chrono::high_resolution_clock::now();

    uint64_t total_trades_generated = 0;
    for (auto& order : orders_to_process) {
        vector<lob::Trade> trades = engine.process_order(order);
        total_trades_generated += trades.size();
    }

    auto end_time = chrono::high_resolution_clock::now();
    
    auto duration_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    double ns_per_order = static_cast<double>(duration_ns) / orders_to_process.size();

    cout << "\n========================================\n";
    cout << "          BENCHMARK RESULTS             \n";
    cout << "========================================\n";
    cout << " -> Total Orders Processed : " << orders_to_process.size() << "\n";
    cout << " -> Total Trades Generated : " << total_trades_generated << "\n";
    cout << " -> Total Elapsed Time     : " << duration_ms << " ms (" << duration_ns << " ns)\n";
    cout << " -> Performance Metric     : " << ns_per_order << " ns per order\n";
    cout << "========================================\n";

    return 0;
}