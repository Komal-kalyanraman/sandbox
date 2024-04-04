from kubernetes import client, config
import time

# Load the kube config from the default location
config.load_kube_config()

# Create an instance of the CoreV1Api
v1 = client.CoreV1Api()

# Create an instance of the CustomObjectsApi
api = client.CustomObjectsApi()

while True:
    # Get the list of pods
    pods = v1.list_namespaced_pod(namespace='default')

    for pod in pods.items:
        pod_name = pod.metadata.name

        try:
            # Get the metrics for the pod
            metrics = api.get_namespaced_custom_object(
                "metrics.k8s.io", "v1beta1", "default", "pods", pod_name)

            # Get the CPU and memory usage
            cpu_usage = metrics['containers'][0]['usage']['cpu']
            memory_usage = metrics['containers'][0]['usage']['memory']

            print(f"Pod: {pod_name}")
            print(f"CPU usage: {cpu_usage}")
            print(f"Memory usage: {memory_usage}")
        except client.exceptions.ApiException as e:
            print(f"Failed to get metrics for pod {pod_name}: {e}")

    # Wait for a while before getting the metrics again
    time.sleep(1)