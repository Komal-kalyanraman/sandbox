import grpc
from concurrent import futures
import time

# Import the generated classes
import collector_pb2
import collector_pb2_grpc

# Create a class to define the server functions, derived from
# collector_pb2_grpc.CollectorServicer
class CollectorServicer(collector_pb2_grpc.CollectorServicer):

    def UpdateDatapoints(self, request, context):
        # Implement your server logic here
        pass

# Create a gRPC server
server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))

# Use the generated function `add_CollectorServicer_to_server`
# to add the defined class to the server
collector_pb2_grpc.add_CollectorServicer_to_server(
        CollectorServicer(), server)

# Listen on port 50051
print('Starting server. Listening on port 50051.')
server.add_insecure_port('[::]:50051')
server.start()

# Keep thread alive
try:
    while True:
        time.sleep(86400)
except KeyboardInterrupt:
    server.stop(0)
