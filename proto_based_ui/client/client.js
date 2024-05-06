const protobuf = require("protobufjs");
const messages = require("../proto/message_pb.js");
const services = require("../proto/message_grpc_pb.js");

protobuf.load("../proto/message.proto", function (err, root) {
  if (err) throw err;

  // Obtain the message type
  const MyMessage = root.lookupType("MyMessage");

  // Define the fetchMessage function
  function fetchMessage() {
    fetch("http://localhost:5000/message")
      .then((response) => response.json())
      .then((data) => {
        // Use the message
        console.log(`Name: ${data.name}`);
        console.log(`ID: ${data.id}`);
        console.log(`Email: ${data.email}`);
      });
  }

  // Call the fetchMessage function
  fetchMessage();
});
