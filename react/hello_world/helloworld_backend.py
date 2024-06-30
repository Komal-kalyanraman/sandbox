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

if __name__ == '__main__':
    app.run( host="0.0.0.0", port=3001, debug=True)