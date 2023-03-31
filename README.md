# motion project

Pre-req:

1. MUST have a folder called `project` in homedir (~). That is where the build files will be created
2. The developement and building of this project should be done on target due to the necessary libraies being on there, and idk how to cross compile
3. Clone this repo in the public folder, so both host and target can access
4. The server should be installed on the host instead of the target before proceeding, as running both the application and the server on the target machine may cause the system to run significantly slower.

To use:
1. To install the server, navigate to the server folder and run 'npm install' on the host.
2. Use the command 'node index.js' to run the server.
3. Run `make` on target
4. Then on target `make run.
5. To view the webpage, go to http://192.168.7.1:3000.


### Mounting the USB in order to save the video file:

1. Connect the USB drive to the BeagleBone board.

2. Log in to the BeagleBone board and run the command `lsblk` to see the list of disks and their mount points.

3. Identify the USB drive from the output of the `lsblk` command. It should have a name like /dev/sda1 or /dev/sdb1.

4. Create a directory to use as the mount point for the USB drive. For example, you can create a directory named /mnt/usb-mount by running the command `mkdir /mnt/usb-mount`.

5. Mount the USB drive to the directory you just created by running the command `sudo mount /dev/sda1 /home/debian/usb-mount -o umask=000` (replace /dev/sda1 with the name of the USB drive you identified in step 3).

6. Verify that the USB drive is mounted correctly by running the command `df -h` . You should see a line in the output showing the mount point directory /mnt/usb and the device name of the USB drive.

