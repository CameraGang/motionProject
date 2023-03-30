const UDP_SERVER_PORT = 1235;
const UDP_SERVER_URL = "192.168.7.2";

var errorTimer;
const ERROR_TIMEOUT = 2000;

function sendMsg(client, code, socket) {
  let data = Buffer.from(code);

  errorTimer = setTimeout(() => {
    socket.emit("error", { code: 1, msg: "C application did not respond in " + ERROR_TIMEOUT + "ms. Please check if it's running." });
  }, ERROR_TIMEOUT);

  client.send(data, 0, data.length, UDP_SERVER_PORT, UDP_SERVER_URL, (err) => {
    if (err) {
      socket.emit("error", "There was an error sending " + code);
    } else {
      socket.emit("success", code + "success");
    }
  });
}

function listenForResponse(client, socket) {
  client.on("message", (msg, info) => {
    socket.emit("response", msg.toString());
    socket.emit("error", { code: 0, msg: "" }); // emit 0 error to indicate no error
    if (errorTimer) {
      clearTimeout(errorTimer);
    }
  });

  client.on("error", (err) => {
    console.log(err);
  });
}

module.exports = {
  listenForResponse,
  sendMsg,
};
