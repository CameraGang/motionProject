const socket = io();
socket.on("connect", (socket) => {
    console.log("Connected");
});

socket.on("response", (data) => {
    data = JSON.parse(data);
    console.log(data);
    document.getElementById('motionStatus').innerHTML = data["motion"] == 0 ? "no motion 😌" : "MOTION DETECTED! 🚨";
    document.getElementById('frameNum').innerHTML = "Frame: " + data["frameNum"];
});

setInterval(() => {
    performAction("ping");
},250);

$(document).ready(function () {
    var canvasVideo = document.getElementById('videostream');
    var canvasImage = document.getElementById('capturedImage');
    var ctxVideo = canvasVideo.getContext('2d');
    var ctxImage = canvasImage.getContext('2d');

    socket.on('canvas', function (data) {
        // Stream video at canvas tag
        const canvas = $("#videostream");
        const context = canvas[0].getContext('2d');
        const image = new Image();
        image.src = "data:image/jpeg;base64," + data;
        image.onload = function () {
            context.height = image.height;
            context.width = image.width;
            context.drawImage(image, 0, 0, context.width, context.height);
        }
    });
    showDate();
    window.setInterval(function () { updateTime() }, 1000);

    $('#capture').click(function () {
        // Capture image from videostream canvas tag
        var imgData = ctxVideo.getImageData(0, 0, canvasVideo.width, canvasVideo.height);

        // Draw image onto capturedImage canvas tag
        ctxImage.putImageData(imgData, 0, 0);
    });

    $('#save').click(function () {
        // Save image from captuedImage canvas tag

        // file name format : cameragang + date + time
        const now = new Date();
        const year = now.getFullYear();
        const month = (now.getMonth() + 1).toString().padStart(2, '0');
        const day = now.getDate().toString().padStart(2, '0');
        const hours = now.getHours().toString().padStart(2, '0');
        const minutes = now.getMinutes().toString().padStart(2, '0');
        const seconds = now.getSeconds().toString().padStart(2, '0');
        const filename = `cameragang_${year}:${month}:${day} ${hours}-${minutes}-${seconds}.png`;

        const dataUrl = canvasImage.toDataURL("image/png");
        const link = document.createElement("a");
        link.download = filename;
        link.href = dataUrl;
        link.click();
    });
    
    $('#togglealarm').click(function () {
        performAction('togglealarm');
    });
});

function updateTime() {
    var now = new Date();
    var timeStr = now.getHours() + ':' + now.getMinutes() + ':' + now.getSeconds();
    $('#clock').html("Current Time : " + timeStr);
}

function showDate() {
    const now = new Date();
    const isoString = now.toISOString(); // e.g. '2022-04-06T14:30:00.000Z'
    const [year, month, day] = isoString.slice(0, 10).split('-');
    const formattedDate = `${year}:${month}:${day}`; // e.g. '2022:04:06'
    $('#date').html("Date : " + formattedDate);
}

function performAction(code) {
    socket.emit("action", code);
}