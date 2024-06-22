#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"


void generateRandom(int min, int max) // by nasser
{
    int max_range;
    int min_range;

    srand(time(NULL)*3.33);
    int n1 = (int)(min + (rand() % (max - min)));
    int n2 = (int)(min + (rand() % (max - min)));

    max_range = (n1 > n2 ? n1 : n2);
    min_range = (n1 > n2 ? n2 : n1);

    FILE *fp = fopen("range.txt", "w");
    if (fp == NULL)
    {
        printf("Error: Failed to open file range.txt\n");
        exit(-1);
    }
    fprintf(fp, "%d,%d", min_range, max_range);
    fclose(fp);
}


// set the text color to green
void green() {
    printf("\033[32;1m");
    fflush(stdout);
}

// set the text color to red
void red() {
    printf("\033[31;1m");
    fflush(stdout);
}

void blue() {
    printf("\033[34;1m");
    fflush(stdout);
}

// set the text color to purple
void purple() {
    printf("\033[35m");
    fflush(stdout);
}

// set the text color to yellow
void yellow() {
    printf("\033[33;1m");
    fflush(stdout);
}

// reset the text attributes
void resetColor() {
    printf("\033[0m");
    fflush(stdout);
}


#endif