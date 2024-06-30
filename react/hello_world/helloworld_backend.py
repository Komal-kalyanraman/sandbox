from flask import Flask, jsonify, request
from flask_cors import CORS
import datetime

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
    return jsonify(message="Slider value received", value=slider_value)

if __name__ == '__main__':
    app.run( host="0.0.0.0", port=3001, debug=True)