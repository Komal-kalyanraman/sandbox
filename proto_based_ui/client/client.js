const protobuf = require("protobufjs");
const messages = require("../proto/message_pb.js");
const services = require("../proto/message_grpc_pb.js");

protobuf.load("../proto/message.proto", function (err, root) {
  if (err) throw err;

  // Obtain the message type
  const MyMessage = root.lookupType("MyMessage");

  // Fetch the message from the server
  fetch("http://localhost:5000")
    .then((response) => response.arrayBuffer())
    .then((buffer) => {
      // Deserialize the message from bytes
      const message = MyMessage.decode(new Uint8Array(buffer));

      // Use the message
      console.log(`Name: ${message.name}`);
      console.log(`ID: ${message.id}`);
      console.log(`Email: ${message.email[0]}`);
    });
});
