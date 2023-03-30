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