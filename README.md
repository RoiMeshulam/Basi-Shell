# Authors
Arik Tatievski

Roi Meshulam

# What is this project?

This project is a basic shell program

# The way we made our project:
Basiclly this is an infinite loop reciving commands from the user. 

If the command is in our "command arsenal" we will execute it, if not we will send it to a regular bash and try to commit it.

# Command arsenal

*EXIT* - exits the shell.

*getcwd* - prints the curret reposetory.

*ECHO* - prints what ever comes after the ECHO.

*DIR* - prints everything the curret reposetory contains.

*CD* - try to enter the reposetory after the CD command (MUST BE INSIDE THE CURRET REPOSETORY).

*TCP PORT* - connect to a local server (Notice Server.c) and transfers all the commands to the server.

*LOCAL* - disconnects the server and goes back to the shell.

*COPY* - copies the content of SRC file to DEST file.

*DELETE* - deletes the file comes after the DELETE command.


# How to use our project:

Write in a bash envioremnt "make all". 

Please run ./Shell


# Hope you find good usuage of this project!
