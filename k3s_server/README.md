Here's a basic `README.md` for your `k3s_server.py`:

````md
# Kubernetes Server Metrics Monitor

This Python script uses the Kubernetes client library to monitor the CPU and memory usage of pods running in a Kubernetes cluster.

## Prerequisites

- Python 3
- Kubernetes Python client library
- Access to a Kubernetes cluster

## Setup

1. Install the required Python packages:

   ```sh
   pip install kubernetes
   ```
````

2. Make sure your kubeconfig file is located at `~/.kube/config` or set the `KUBECONFIG` environment variable to the path of your kubeconfig file.

## Usage

Run the script with the following command:

```sh
python3 k3s_server.py
```

The script will continuously print the CPU and memory usage of each pod in the 'default' namespace.

## Note

This script only fetches metrics for the 'default' namespace. If you want to fetch metrics for a different namespace, you can modify the `namespace` parameter in the `list_namespaced_pod` and `get_namespaced_custom_object` function calls.

```

Please adjust the content as needed to fit your project's requirements.
```
