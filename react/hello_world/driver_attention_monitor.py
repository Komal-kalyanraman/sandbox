import cv2

def process_image(image_path):
    # Load the Haar cascade xml file for face detection
    face_cascade = cv2.CascadeClassifier('./haarcascade_frontalface_default.xml')

    # Load the image
    image = cv2.imread(image_path)

    # Convert the image to grayscale
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Detect faces
    faces = face_cascade.detectMultiScale(gray_image, 1.1, 4)

    # Check if faces are detected
    if len(faces) > 0:
        return "Person detected"
    else:
        return "No person detected"

# Example usage
result = process_image("received_frame.jpeg")
print(result)