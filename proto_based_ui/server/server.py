from flask import Flask, send_from_directory, Response, render_template
from flask_cors import CORS
import os
import message_pb2

app = Flask(__name__, static_folder='../', template_folder='templates')
CORS(app)

@app.route('/')
def home():
    return render_template('client.html')

@app.route('/<path:path>')
def serve_file(path):
    return send_from_directory(app.static_folder, path)

@app.route('/message')
def send_message():
    # Create a new message
    message = message_pb2.MyMessage()
    message.name = "Test"
    message.id = 123
    message.email.append("test@example.com")

    # Print the message
    print(f"Name: {message.name}")
    print(f"ID: {message.id}")
    print(f"Email: {message.email[0]}")

    # Serialize the message to a byte string
    message_bytes = message.SerializeToString()

    # Send the message as a response
    return Response(message_bytes, mimetype='application/octet-stream')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)