const socket = io();
socket.on("connect", (socket) => { //confirm connection with NodeJS server
    console.log("Connected");
});

$(document).ready(function () {
    socket.on('canvas', function (data) {
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
    window.setInterval(function(){updateTime()}, 1000);
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