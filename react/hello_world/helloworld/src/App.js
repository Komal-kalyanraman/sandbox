import React, { useState } from "react";
import logo from "./logo.svg";
import "./App.css";

function App() {
  const [message, setMessage] = useState("");
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
  return (
    <div className="App">
      <button onClick={handleClick}> current time </button>
      <input type="text" value={message} readOnly />{" "}
      {/* Step 4: Display the message in a text box */}
    </div>
  );
}

export default App;
