import Checkbox from "@mui/material/Checkbox";
import FormControlLabel from "@mui/material/FormControlLabel";
import Papa from "papaparse";
import React, { useEffect, useState } from "react";
import {
  Area,
  AreaChart,
  CartesianGrid,
  Tooltip,
  XAxis,
  YAxis,
} from "recharts";
import "./App.css";

const App = () => {
  const [data, setData] = useState([]);
  const [showValue1, setShowValue1] = useState(true);
  const [showValue2, setShowValue2] = useState(true);
  const [showValue3, setShowValue3] = useState(true);
  const [showValue4, setShowValue4] = useState(true);

  useEffect(() => {
    Papa.parse("/all_logs.csv", {
      download: true,
      header: false,
      skipEmptyLines: true,
      complete: (results) => {
        // Skip the first line (header) and start parsing from the second line
        const parsedData = results.data
          .slice(1)
          .map(([time, cpu, memory, qm, avp]) => ({
            time,
            cpu: Number(cpu),
            memory: Number(memory),
            qm: Number(qm),
            avp: Number(avp),
          }));
        setData(parsedData);
      },
    });
  }, []);

  return (
    <div>
      <div className="checkbox">
        <FormControlLabel
          label="Avg CPU Utilization"
          control={
            <Checkbox
              checked={showValue1}
              onChange={() => setShowValue1(!showValue1)}
            />
          }
        />
        <FormControlLabel
          label="Avg Memory Utilization"
          control={
            <Checkbox
              checked={showValue2}
              onChange={() => setShowValue2(!showValue2)}
            />
          }
        />
        <FormControlLabel
          label="QM"
          control={
            <Checkbox
              checked={showValue3}
              onChange={() => setShowValue3(!showValue3)}
            />
          }
        />
        <FormControlLabel
          label="AVP"
          control={
            <Checkbox
              checked={showValue4}
              onChange={() => setShowValue4(!showValue4)}
            />
          }
        />
      </div>
      <AreaChart
        width={1500}
        height={500}
        data={data}
        margin={{
          top: 100,
          right: 30,
          left: 0,
          bottom: 0,
        }}
      >
        <CartesianGrid strokeDasharray="3 3" />
        <XAxis dataKey="time" />
        <YAxis
          domain={[0, 100]}
          ticks={Array.from({ length: 11 }, (_, i) => i * 10)}
        />
        <Tooltip />
        {showValue1 && (
          <Area type="monotone" dataKey="cpu" stroke="#8884d8" fill="#8884d8" />
        )}
        {showValue2 && (
          <Area
            type="monotone"
            dataKey="memory"
            stroke="#82ca9d"
            fill="#82ca9d"
          />
        )}
        {showValue3 && (
          <Area type="monotone" dataKey="qm" stroke="#ffc658" fill="#ffc658" />
        )}
        {showValue4 && (
          <Area
            type="monotone"
            dataKey="avp"
            stroke="#ff7300" // Orange
            fill="#ff7300" // Orange
          />
        )}
      </AreaChart>
    </div>
  );
};

export default App;
