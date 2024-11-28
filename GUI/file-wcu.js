import React, { useState } from 'react';
import { FaArrowUp, FaArrowDown } from 'react-icons/fa';
import { Line } from 'react-chartjs-2';
import 'chart.js/auto';

const App = () => {
  const [data, setData] = useState([381, 317, 254, 190, 127, 63]);
  const [selected, setSelected] = useState(0);

  const handleSelect = (index) => {
    setSelected(index);
  };

  const chartData = {
    labels: Array(data.length).fill(''),
    datasets: [
      {
        label: 'Market Trend',
        data: data,
        borderColor: 'rgba(0, 255, 0, 0.8)',
        backgroundColor: 'rgba(0, 255, 0, 0.2)',
      },
    ],
  };

  return (
    <div className="flex flex-col items-center bg-black text-green-500 h-screen p-4">
      <div className="flex justify-between w-full mb-4">
        <div className="flex items-center">
          <button className="p-2 bg-green-700 rounded">Day 1</button>
        </div>
        <div className="text-center">
          <h1 className="text-xl">HEART OF THE MARKET</h1>
          <p>Kidney</p>
        </div>
        <div className="flex items-center">
          <span className="mr-2">3,688</span>
          <span>150</span>
        </div>
      </div>
      <div className="flex w-full">
        <div className="flex flex-col items-center mr-4">
          {['icon1', 'icon2', 'icon3', 'icon4', 'icon5', 'icon6'].map((icon, index) => (
            <button
              key={index}
              className={`p-2 mb-2 ${selected === index ? 'bg-green-700' : 'bg-green-900'}`}
              onClick={() => handleSelect(index)}
            >
              {icon}
            </button>
          ))}
          <div className="flex flex-col items-center">
            <FaArrowUp className="mb-2" />
            <FaArrowDown />
          </div>
        </div>
        <div className="flex-1">
          <Line data={chartData} options={{ responsive: true, maintainAspectRatio: false }} />
        </div>
        <div className="flex flex-col items-center ml-4">
          {data.map((value, index) => (
            <div key={index} className="mb-2">
              {value}
            </div>
          ))}
        </div>
      </div>
      <div className="flex justify-between w-full mt-4">
        {['MARKET', 'REQUEST', 'TRADERS', 'TRADE', 'FULFILL', 'CARGO', 'HOLDS', 'PERSONAL'].map((label, index) => (
          <button key={index} className="p-2 bg-green-700 rounded">
            {label}
          </button>
        ))}
      </div>
    </div>
  );
};

export default App;