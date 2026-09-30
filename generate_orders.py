import random

num_orders = 1_000_000
base_price = 100.0

print(f"Generating {num_orders:,} orders...")

with open("orders_benchmark.csv", "w") as f:
    for _ in range(num_orders):
        order_type = random.choice(["LIMIT", "LIMIT", "LIMIT", "MARKET"])
        side = random.choice(["BID", "ASK"])
        
        price_offset = random.uniform(-2.0, 2.0)
        price = round(base_price + price_offset, 2)
        quantity = random.randint(1, 100)
        
        if order_type == "MARKET":
            f.write(f"MARKET,{side},0.0,{quantity}\n")
        else:
            f.write(f"LIMIT,{side},{price},{quantity}\n")

print("Done! 'orders_benchmark.csv' generated successfully.")