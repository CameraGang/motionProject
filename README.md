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

### Mounting the USB in order to save the video file:

1. Connect the USB drive to the BeagleBone board.

2. Log in to the BeagleBone board and run the command `lsblk` to see the list of disks and their mount points.

3. Identify the USB drive from the output of the `lsblk` command. It should have a name like /dev/sda1 or /dev/sdb1.

4. Create a directory to use as the mount point for the USB drive. For example, you can create a directory named /mnt/usb by running the command `sudo mkdir /mnt/usb`.

5. Mount the USB drive to the directory you just created by running the command `sudo mount /dev/sda1 /mnt/usb` (replace /dev/sda1 with the name of the USB drive you identified in step 3).

6. Verify that the USB drive is mounted correctly by running the command `df -h` . You should see a line in the output showing the mount point directory /mnt/usb and the device name of the USB drive.
