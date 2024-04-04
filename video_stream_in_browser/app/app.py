import os
import cv2
import time
import random
import string
import threading
from datetime import datetime
from flask import Flask, render_template, jsonify, url_for

app = Flask(__name__)

def process_video():
    while True:
        vidcap = cv2.VideoCapture('video.mp4')
        success, image = vidcap.read()
        while success:
            cv2.imwrite(os.path.join('static', f'img.jpg'), image)  # save frame as JPEG file
            success, image = vidcap.read()
            time.sleep(0.1)

@app.route('/')
def home():
    image_url = url_for('static', filename='img.jpg', _external=True) + '?' + str(datetime.now().timestamp())
    return render_template('index.html', image_url=image_url)

@app.route('/button')
def button_click():
    print("Hello, World!!")
    random_string = ''.join(random.choices(string.ascii_uppercase + string.digits, k=10))
    return jsonify({'message': random_string})

@app.route('/aws', methods=['POST'])
def request_from_aws():
    print("Request from AWS received")
    return jsonify({'message': 'You reached me successfully!'})

if __name__ == "__main__":
    video_thread = threading.Thread(target=process_video)
    video_thread.start()
    app.run(host='0.0.0.0', port=6600)
