import React, { useState } from "react";
import logo from "./logo.svg";
import "./App.css";

function App() {
  const [message, setMessage] = useState("");
  const [sliderValue, setSliderValue] = useState(0);

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
      <p>Slider Value: {sliderValue}</p> {/* Displaying the slider value */}
    </div>
  );
}

export default App;
