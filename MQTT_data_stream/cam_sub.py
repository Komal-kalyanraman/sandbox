import cv2
import paho.mqtt.client as mqtt
import base64
import numpy as np

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("webcam/frame")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

    # Convert bytes to image
    img = base64.b64decode(msg.payload)
    npimg = np.frombuffer(img, dtype=np.uint8)
    source = cv2.imdecode(npimg, 1)

    # Save image
    cv2.imwrite('./image.jpg', source)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.hivemq.com", 1883, 60)
client.loop_forever()