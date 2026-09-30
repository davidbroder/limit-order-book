# Quantitative Finance: High-Performance C++ Limit Order Book & Matching Engine

Financial markets operate at microsecond scales, demanding robust software architecture capable of processing continuous order flows with deterministic priority and ultra-low latency. This project is a high-performance C++ Limit Order Book (LOB) and Matching Engine engineered from scratch to simulate real-world electronic exchange infrastructure.

Designed with rigorous low-latency principles, the system implements strict Price-Time priority rules, $O(1)$ order cancellations, and cache-conscious memory layouts. It serves as a practical demonstration of advanced data structures, algorithmic efficiency, and systems-level optimization tailored for quantitative trading and High-Frequency Trading (HFT) environments.

## 1. Core Architecture & Market Mechanics

The engine coordinates the lifecycle of financial orders through an event-driven matching loop, handling resting liquidity and aggressive sweeps with absolute determinism.

* **Price-Time Priority (FIFO):** The system enforces strict matching rules where orders are prioritized first by price competitiveness (highest bid / lowest ask) and second by arrival timestamp.
* **Dual-Container Hybrid Indexing:** To achieve $O(1)$ cancellations—a critical requirement in HFT where participants constantly modify or withdraw quotes—the architecture combines `std::map` (for sorted price-level aggregation) with an `std::unordered_map` that maps `OrderId` directly to list iterators (`std::list::iterator`). This completely eliminates linear searches during cancellation routines.
* **Integer-Based Price Representation:** To prevent floating-point rounding inaccuracies and performance overhead, all prices are mapped to unsigned 64-bit integers (`uint64_t`) representing discrete ticks (e.g., multiplied by 100).

## 2. Low-Latency Engineering & Memory Optimization

Performance in quantitative infrastructure is dictated by memory access patterns and CPU cache utilization. This engine incorporates several hardware-level optimizations:

* **Cache-Conscious Object Layout:** The `Order` class is meticulously structured in descending order of member sizes (8-byte, 4-byte, and 1-byte primitives). This minimizes structural padding and fits the entire object into **32 bytes**—exactly half of a standard 64-byte CPU cache line, significantly reducing cache misses.
* **Stateless and Allocation-Free Hot Paths:** The matching logic avoids dynamic heap reallocations during high-frequency execution cycles by reusing container nodes and operating via efficient references.
* **High-Resolution Microsecond Benchmarking:** Performance is quantified using `std::chrono::high_resolution_clock`, measuring end-to-end execution latency and throughput across datasets containing millions of synthetic orders.

## 3. Project Structure

The codebase is organized into modular components adhering to modern C++ separation of concerns:

```text
limit_order_book/
│
├── types.hh            # Primitive type aliases (OrderId, Price, Quantity) and enums (Side, OrderType)
├── order.hh            # Core Order class optimized for cache locality and memory alignment
├── order_book.hh       # OrderBook class declaration (Bids/Asks maps + O(1) hash index)
├── order_book.cc       # OrderBook business logic implementation (Add, Cancel, Match helpers)
├── matching_engine.hh  # MatchingEngine interface for order routing
├── matching_engine.cc  # Core matching logic (Limit/Market execution, partial fills, trades)
├── main.cc             # High-performance CSV benchmark driver using std::chrono
├── generate_orders.py  # Python script to generate large-scale benchmark datasets (CSV)
└── Makefile            # Automated build configuration (Supports Release -O3 and Debug modes)