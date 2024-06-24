import logo from "./logo.svg";
import "./App.css";

function App() {
  const handleClick = () => {
    fetch("http://localhost:3001/api/time")
      .then((res) => res.json())
      .then((data) => {
        console.log(data.message);
      })
      .catch((error) => {
        console.error("Error fetching data:", error);
      });
  };
  return (
    <div className="App">
      <button onClick={handleClick}> current time </button>
    </div>
  );
}

export default App;
