#!/bin/sh

# sed -i -e 's/\r$//' run.sh
sudo apt-get install figlet
clear



cd src/

rm -f parent
rm -f child
gcc -g child.c -o child
gcc -o opengl opengl.c -lGL -lGLU -lglut -lm
gcc -g parent.c -o parent
clear




echo "\e[34m"
echo "Running the program..."
echo "----------------------------------------------------------------"
figlet "THE COMPETITION"
echo "----------------------------------------------------------------"
echo "\e[0m"

sleep 2s

./parent $1