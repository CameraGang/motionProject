# motion project

This repo is there to simply get the camera working and to learn how to interact with it.

main.c is created with reference to the exampleFile.c and the following resources:

- https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/v4l2.html

The steps for getting a stream from webcam are the following:

1. Open the video device
2. Set the video format and parameters
3. Request and map memory for the video buffers
4. Start capturing video
5. Read video frames from the buffers
6. Stop capturing video and clean up

The steps for detecting motion are:
-- TODO

Pre-req:

1. The developement and building of this project should be done on target due to the necessary libraies being on there, and idk how to cross compile
2. Clone this repo in the public folder, so both host and target can access
3. MUST have a folder called `project` in homedir (~). That is where the build files will be created

To use:

1. run `make` on target
2. Then on target `./cameraTest | ffmpeg -vcodec mjpeg -i pipe:0 -f mjpeg udp://192.168.7.1:1234` in the `~/project` folder.
3. Then on host open VLC and then in Media->open Network Stream->Network->Network Protocol->"Please enter a
   network URL" field enter: "udp://@:1234"
