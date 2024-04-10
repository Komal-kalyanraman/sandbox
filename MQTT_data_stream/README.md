# Webcam Stream Subscriber

This Python script uses the Paho MQTT client library to subscribe to an MQTT topic and receive webcam data. The data is then decoded and displayed using OpenCV.

## Prerequisites

- Python 3
- OpenCV Python library
- Paho MQTT Python client library
- An MQTT broker (this script uses the public broker at `broker.hivemq.com`)

## Setup

1. Install the required Python packages:

   ```sh
   pip install opencv-python paho-mqtt==1.5.1
   ```
