from kuksa_client.grpc import VSSClient
from kuksa_client.grpc import Datapoint

import time

with VSSClient('127.0.0.1', 55555) as client:
    for cpu in range(0,100):
        client.set_current_values({
        'Vehicle.Cpu': Datapoint(cpu),
        })
        print(f"Feeding Vehicle.Cpu to {cpu}")
        time.sleep(1)
print("Finished.")