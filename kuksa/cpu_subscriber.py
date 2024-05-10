from kuksa_client.grpc import VSSClient

with VSSClient('127.0.0.1', 55555) as client:

    for updates in client.subscribe_current_values([
        'Vehicle.Cpu',
    ]):
        cpu = updates['Vehicle.Cpu'].value
        print(f"Received updated CPU: {cpu}")