// Code Referenced from https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/StreamingWebcamFromBeagleBoneToNodeJSServer.pdf

const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
const startRouter = require('./routers/page.js');
const { SERVER_PORT: port = 3000 } = process.env;
const udpManager = require("./udpManager.js");

const child = require('child_process');

const udp = require("dgram");
var client = udp.createSocket("udp4");

app.use('/', startRouter);
io.on('connection', (socket) => {
    console.log('a user connected');
    let ffmpeg = child.spawn("ffmpeg", [
        "-re",
        "-y",
        "-i",
        "udp://192.168.7.1:1234",
        "-preset",
        "ultrafast",
        "-f",
        "mjpeg",
        "pipe:1"
    ]);
    ffmpeg.on('error', function (err) {
        console.log(err);
        throw err;
    });
    ffmpeg.on('close', function (code) {
        console.log('ffmpeg exited with code ' + code);
    });
    ffmpeg.stderr.on('data', function (data) {
        // Don't remove this
        // Child Process hangs when stderr exceed certain memory
    });
    ffmpeg.stdout.on('data', function (data) {
        var frame = Buffer.from(data).toString('base64'); //convert raw data to string
        io.sockets.emit('canvas', frame); //send data to client
    });

    socket.on("action", (code) => {
        udpManager.sendMsg(client, code, socket);
    })

    udpManager.listenForResponse(client, socket);
});

server.listen({ port }, () => {
    console.log(`Server ready at http://192.168.7.1:${port}`);
});