from flask import Flask, jsonify, request
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/api/time', methods=['GET'])
def hello():
    print("hello")
    return jsonify(message='Hello, world!')

if __name__ == '__main__':
    app.run( host="0.0.0.0", port=3001, debug=True)