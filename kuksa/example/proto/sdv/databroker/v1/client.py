import grpc

# Import the generated classes
import collector_pb2
import collector_pb2_grpc

# Open a gRPC channel
channel = grpc.insecure_channel('localhost:50051')

# Create a stub (client)
stub = collector_pb2_grpc.CollectorStub(channel)

# Create a valid request message
datapoint = collector_pb2.UpdateDatapointsRequest()

# Make the call
response = stub.UpdateDatapoints(datapoint)

# Et voilà
print(response)
