import cv2
import paho.mqtt.client as mqtt
import base64

# Set up MQTT client
client = mqtt.Client()
client.connect("broker.hivemq.com", 1883, 60)

# Open webcam
cap = cv2.VideoCapture(0)

count = 0

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Resize the frame
    frame = cv2.resize(frame, (640, 480))

    if count == 10:
        # Convert the image to jpg format
        _, buffer = cv2.imencode('.jpg', frame)
        jpg_as_text = base64.b64encode(buffer)

        # Publish the frame
        client.publish("webcam/frame", jpg_as_text)

        count = 0
    
    count += 1

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()