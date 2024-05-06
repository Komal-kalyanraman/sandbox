from flask import Flask, jsonify, render_template
from flask_cors import CORS
import message_pb2

app = Flask(__name__, template_folder='templates')
CORS(app)

@app.route('/')
def home():
    return render_template('client.html')

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

    # Convert the message to a dictionary
    message_dict = {"name": message.name, "id": message.id, "email": message.email[0]}

    # Send the message as a response
    return jsonify(message_dict)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)