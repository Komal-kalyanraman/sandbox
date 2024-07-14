from flask import Flask, jsonify, request
from flask_cors import CORS
import datetime
import base64
from io import BytesIO
from PIL import Image
from kuksa_client.grpc import VSSClient
from kuksa_client.grpc import Datapoint
from driver_attention_monitor import process_image

app = Flask(__name__)
CORS(app)

@app.route('/api/time', methods=['GET'])
def currentTime():
    now = datetime.datetime.now()  # Step 2: Get current date and time
    formatted_now = now.strftime('%Y-%m-%d %H:%M:%S')  # Step 3: Format the date and time
    return jsonify(time=formatted_now)  # Step 4: Return the formatted time

@app.route('/api/slider', methods=['POST'])
def handle_slider():
    slider_value = request.json.get('value')  # Get the slider value from the request body
    print(f"Received slider value: {slider_value}")  # For demonstration, print the value
    with VSSClient('127.0.0.1', 55555) as client:
        client.set_current_values({
        'Vehicle.AverageSpeed': Datapoint(slider_value),
        })
    return jsonify(message="Slider value received", value=slider_value)

@app.route('/api/process_frame', methods=['POST'])
def process_frame():
    data = request.json.get('image')
    if data:
        # Strip the header from the base64-encoded image
        header, encoded = data.split(",", 1)
        image_data = base64.b64decode(encoded)
        # Convert the bytes to an image
        image = Image.open(BytesIO(image_data))
        # Process the image here (e.g., save, analyze, etc.)
        image.save("received_frame.jpeg", "JPEG")  # Example: saving the image
        result = process_image("received_frame.jpeg")  # Example: processing the image
        if result == "Person detected":
            flag = False
        else:
            flag = True
        
        with VSSClient('127.0.0.1', 55555) as client:
            client.set_current_values({
            'Vehicle.ADAS.DMS.IsWarning': Datapoint(flag),
            })
        return jsonify(message=result)
    else:
        return jsonify(message="No image data received"), 400

@app.route('/api/lock', methods=['POST'])
def handle_toggle():
    toggle_state = request.json.get('isToggled')  # Extract the toggle state from the request body
    if toggle_state == True:
        flag = False
    else:
        flag = True

    with VSSClient('127.0.0.1', 55555) as client:
        client.set_current_values({
        'Vehicle.Cabin.Door.Row1.DriverSide.IsLocked': Datapoint(flag),
        })
    return jsonify(message="Toggle state received", isToggled=toggle_state)

if __name__ == '__main__':
    app.run( host="0.0.0.0", port=3001, debug=True)