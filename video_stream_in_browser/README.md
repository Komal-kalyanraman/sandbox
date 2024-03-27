# Flask Video Streaming Application on open Internet

This is a Flask application that streams a video by converting it into frames and serving them as images.

## Setup

1. Clone the repository:

   ```bash
   git clone <repository-url>
   ```

2. Navigate into the project directory:

   ```bash
   cd <project-directory>
   ```

3. Install the required Python packages:

   ```bash
   pip install -r requirements.txt
   ```

4. Run the Flask application:
   ```bash
   python3 app.py
   ```

The application will start a server on `localhost` at port `6600`.

## Exposing the Application with Serveo for open internet

Serveo is a service that allows you to expose a local server to the internet. Here's how you can use it to expose this Flask application:

1. Make sure the Flask application is running.

2. Open a new terminal window and use the following command to set up a reverse tunnel with Serveo:
   ```bash
   ssh -R 80:localhost:6600 serveo.net
   ```

In this command, `80` is the port on the Serveo server that you want to use, `localhost:6600` is the address and port of your local server, and `serveo.net` is the Serveo server.

Once you run this command, Serveo will provide a URL that you can use to access your Flask application from anywhere on the internet.

Please note that Serveo is a free service and sometimes it may be down or unavailable due to high demand. If you're having issues with Serveo, you might want to consider using a different service like ngrok or LocalTunnel.
