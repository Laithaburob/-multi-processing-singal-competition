# 🏆 The Competition 🚀

This project is a multi-processing application based on the signals and pipes/fifos facilities under Linux. The goal is to have a parent process fork 5 children processes, and then simulate a competition between two teams.

## 🚀 How it Works

1. On startup, the parent process will fork the 5 children processes.
2. The parent process will write down 2 comma-separated integer values in a file called range.txt.
3. Once instructed to do so, the first 4 processes will generate 4 floating values in the range decided on by the parent and inform the parent once its value is ready to be picked.
4. Once the parent process gets the 4 values, it will send them as a message (e.g. comma-separated message) through a pipe to its fifth child process (the co-processor).
5. The co-processor will sum the first 2 values and create the value Sum1. In addition, it will sum the third and fourth values to create the value Sum2. The co-processor will then return the 2 sums to the parent as a message (e.g. comma-separated message).
6. The parent process decides which team has the higher sum.
7. The application runs the steps 2 . . . 6 as many times as instructed by the user. If no argument is provided, assume 5 rounds.
8. On termination, the parent declares the winner of the competition, kills all its child processes, removes any allocated resource, and exits.



## 🤖 Tech Stack

- Linux OS
- Docker
- VNC client
- C programming language
- OpenGL library



## 📝 Todo List

- [x] Create the parent.c file for the parent process
- [ ] Create the child.c file for the children processes
- [ ] Test the program for bugs using gdb debugger
- [ ] Add graphics elements from OpenGL library to illustrate the application
- [ ] Insert delays between rounds

