# 🏫 UDP Check In/Check Out System
This repository contains an implementation of a UDP-based Check In/Check Out system written in C. The system allows users to check in and out by sending data packets from their host machines. The server processes these packets to manage attendance and provide appropriate responses.

# 💻 Features
#### 📥 Check In
* Marks attendance of a client and sends a welcome message.
#### 📤 Check Out: 
* Marks out attendance and updates the database accordingly.
#### 🔄 Attendance Management: 
* Ensures that each check-in/check-out request is handled properly and maintains a list of present members.

# 📂 Files 
* server.c: The server-side program that handles incoming UDP packets, processes check-in/check-out requests, and manages the attendance database.
* client.c: The client-side program that sends check-in/check-out requests to the server.
* attendance.txt: A text file that maintains the current attendance of the students.

# 🛠️ Requirements 
* GCC compiler (for compiling the C programs)
* Standard C libraries
