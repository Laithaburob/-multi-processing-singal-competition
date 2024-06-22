#!/bin/sh

# sed -i -e 's/\r$//' run.sh

cd src/

rm -f opengl

gcc -o opengl opengl.c -lGL -lGLU -lglut -lm

./opengl

