import React, { useState, useEffect, useRef } from "react";
import logo from "./logo.svg";
import "./App.css";

function App() {
  const [message, setMessage] = useState("");
  const [sliderValue, setSliderValue] = useState(0);
  const videoRef = useRef(null); // Ref for the video element
  const canvasRef = useRef(null);

  useEffect(() => {
    navigator.mediaDevices
      .getUserMedia({ video: true })
      .then((stream) => {
        let video = videoRef.current;
        if (video) {
          video.srcObject = stream;
          // Listen for the loadedmetadata event to know when the video is ready
          video.onloadedmetadata = () => {
            // Once the video is ready, call play()
            video.play().catch((err) => {
              console.error("Error attempting to play the video", err);
            });
          };
        }
      })
      .catch((err) => {
        console.error("Error accessing the webcam", err);
      });

    // Set up an interval to send the frame to the backend every 5 seconds
    const intervalId = setInterval(() => {
      sendFrameToBackend();
    }, 5000);

    // Cleanup function to stop video stream and clear the interval
    return () => {
      const video = videoRef.current;
      if (video && video.srcObject) {
        video.srcObject.getTracks().forEach((track) => track.stop());
      }
      clearInterval(intervalId);
    };
  }, []); // Empty array to run the effect only once after the initial render

  const captureFrame = () => {
    const video = videoRef.current;
    const canvas = canvasRef.current;
    if (video && canvas) {
      const context = canvas.getContext("2d");
      canvas.width = video.videoWidth;
      canvas.height = video.videoHeight;
      context.drawImage(video, 0, 0, canvas.width, canvas.height);
      // Convert the canvas to a JPEG image
      const imageDataUrl = canvas.toDataURL("image/jpeg");
      return imageDataUrl; // This is a base64-encoded JPEG image
    }
  };

  const sendFrameToBackend = () => {
    const imageDataUrl = captureFrame();
    if (imageDataUrl) {
      fetch("http://localhost:3001/api/process_frame", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ image: imageDataUrl }),
      })
        .then((response) => response.json())
        .then((data) => {
          console.log("Success:", data);
        })
        .catch((error) => {
          console.error("Error:", error);
        });
    }
  };

  const handleClick = () => {
    fetch("http://localhost:3001/api/time")
      .then((res) => res.json())
      .then((data) => {
        setMessage(data.time);
      })
      .catch((error) => {
        console.error("Error fetching data:", error);
        setMessage("Error fetching data");
      });
  };

  const sendSliderValueToServer = (value) => {
    fetch("http://localhost:3001/api/slider", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ value: value }), // Send the slider value in the request body
    })
      .then((response) => response.json())
      .then((data) => {
        console.log("Success:", data);
      })
      .catch((error) => {
        console.error("Error:", error);
      });
  };

  const handleSliderChange = (event) => {
    const newValue = event.target.value;
    setSliderValue(newValue);
    sendSliderValueToServer(newValue);
    // setSliderValue(event.target.value);
  };

  return (
    <div className="App">
      <button onClick={handleClick}> current time </button>
      <input type="text" value={message} readOnly />{" "}
      {/* <input type="text" value={message} readOnly /> */}
      {/* Slider input */}
      <input
        type="range"
        min="0"
        max="250"
        value={sliderValue}
        onChange={handleSliderChange}
      />
      <p>Vehicle Speed km/h: {sliderValue}</p>{" "}
      {/* Displaying the slider value */}
      <video autoPlay playsInline ref={videoRef}></video>
      <canvas ref={canvasRef} style={{ display: "none" }}></canvas>
    </div>
  );
}

export default App;
