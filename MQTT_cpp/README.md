````markdown
# MQTT C++ Example

This project is a simple example of an MQTT client using the Mosquitto MQTT library in C++.

## Prerequisites

- A Linux-based operating system
- The Mosquitto MQTT library and its C++ wrapper. You can install them on Ubuntu with the following commands:

  ```bash
  sudo apt-get update
  sudo apt-get install libmosquitto-dev libmosquittopp-dev
  sudo apt-get install mosquitto mosquitto-clients
  ```
````

## Building the Project

1. Navigate to the project directory:

   ```bash
   cd ~/workspace/sandbox/MQTT_cpp
   ```

2. Compile the `mqtt_example.cpp` file:

   ```bash
   g++ -o mqtt_example mqtt_example.cpp -lmosquittopp -lmosquitto
   ```

## Running the Project

1. Start the Mosquitto broker (if it's not already running):

   ```bash
   sudo systemctl start mosquitto
   ```

2. Run the MQTT client:

   ```bash
   ./mqtt_example
   ```

## Troubleshooting

- If you're having trouble connecting to the broker, make sure the broker is running and listening on the correct port. You can check the status of the Mosquitto service with `sudo systemctl status mosquitto`.

- If you're getting a "Permission denied" error when trying to run the MQTT client, make sure the `mqtt_example` binary has execute permissions. You can add execute permissions with `chmod +x mqtt_example`.

- If you're getting a "No such file or directory" error when trying to compile the project, make sure the Mosquitto library and its C++ wrapper are installed and their header files are in the include path.
