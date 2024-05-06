const protobuf = require("protobufjs");
const messages = require("../proto/message_pb.js");
const services = require("../proto/message_grpc_pb.js");

protobuf.load("message.proto", function (err, root) {
  if (err) throw err;

  // Obtain the message type
  const MyMessage = root.lookupType("MyMessage");

  // Define the fetchMessage function
  function fetchMessage() {
    fetch("http://localhost:5000/message")
      .then((response) => response.arrayBuffer())
      .then((buffer) => {
        // Decode the protobuf message
        const message = MyMessage.decode(new Uint8Array(buffer));

        // Convert the message to a JavaScript object
        const object = MyMessage.toObject(message, {
          longs: String,
          enums: String,
          bytes: String,
        });

        // Use the message
        console.log(`Name: ${object.name}`);
        console.log(`ID: ${object.id}`);
        console.log(`Email: ${object.email}`);
      });
  }

  // Call the fetchMessage function
  fetchMessage();
});
