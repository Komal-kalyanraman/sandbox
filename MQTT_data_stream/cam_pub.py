import cv2
import paho.mqtt.client as mqtt
import base64

# Load the Haar cascade xml file for face detection
face_cascade = cv2.CascadeClassifier('./haarcascade_frontalface_default.xml')
# full_body_cascade = cv2.CascadeClassifier('./haarcascade_fullbody.xml')
# upper_body_cascade = cv2.CascadeClassifier('./haarcascade_upperbody.xml')

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

    # Convert the image to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    
    # Blur each face
    for (x, y, w, h) in faces:
        face = frame[y:y+h, x:x+w]
        blurred_face = cv2.GaussianBlur(face, (99, 99), 30)
        frame[y:y+h, x:x+w] = blurred_face
    
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