# motion project

Pre-req:

1. MUST have a folder called `project` in homedir (~). That is where the build files will be created
2. The developement and building of this project should be done on target due to the necessary libraies being on there, and idk how to cross compile
3. Clone this repo in the public folder, so both host and target can access

To use:

1. run `make` on target
2. Then on target `make run` or `./motionProject | ffmpeg -vcodec mjpeg -i pipe:0 -f mjpeg udp://192.168.7.1:1234` in the `~/project` folder.
3. Then on host open VLC and then in Media->open Network Stream->Network->Network Protocol->"Please enter a
   network URL" field enter: "udp://@:1234"
