// Include necessary header files
#include "header.h"
#include "functions.h"
#include "constants.h"

// Declare global variables
int max, min;

// Declare function prototype
void handler(int pid);
void readRange();

int fd[1];
char *type;
char msg[BUFFER_SIZE];
char msg_to_parent[BUFFER_SIZE];

// Main function
int main(int argc, char *argv[])
{
    memset(msg, 0, sizeof(msg));
    memset(msg_to_parent, 0, sizeof(msg_to_parent));

    type = argv[0];

    if (strcmp(type, "co-processor") == 0)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            exit(-1);
        }
        fd[0] = atoi(argv[1]);
        fd[1] = atoi(argv[2]);
    }

    signal(SIGUSR1, handler); // Register handler function for SIGUSR1 signal
    pause();

    while (1)
        ;

    return 0;
}

// Define handler function
void handler(int sig)
{
    if (strcmp(type, "co-processor") == 0)
    {

        // printf("hi im in co-processor and i want to read from pipe\n");
        fflush(stdout);

        if (read(fd[0], msg, BUFFER_SIZE) != -1)
        {
            printf("Message received by the Co-Processor: [%s]\n", msg);
            fflush(stdout);
        }

        // Parse the message received from the pipe
        float a, b, c, d;
        sscanf(msg, "%f,%f,%f,%f", &a, &b, &c, &d);

        // Calculate the sums of the numbers received from the pipe
        double sum1 = a + b;
        double sum2 = c + d;
        printf("Sum of Child 1 and chlid 2 numbers: %.2f\n", sum1);
        fflush(stdout);
        printf("Sum of Child 1 and chlid 2 numbers: %.2f\n", sum2);
        fflush(stdout);

        sprintf(msg_to_parent, "%.2f,%.2f", sum1, sum2);
        fflush(stdout);
        printf("Message : [%s]\n", msg_to_parent);
        fflush(stdout);

        if (write(fd[1], msg_to_parent, strlen(msg_to_parent)) != -1)
        {

            printf("Message sent by the Co-Processor: [%s]\n", msg_to_parent);
            fflush(stdout);
        }
        else
        {
            perror("*");
        }

        kill(getppid(), SIGINT);
    }

    else if (strcmp(type, "child") == 0)
    {

        // Call readRange function to read range values from file
        readRange();
        // printf("hi im the pid%d\n",getpid());

        srand(time(0) * 5.333 * getpid()); // to make it more random
        float value = ((float)rand() / RAND_MAX) * (max - min) + min;

        char filename[32];
        snprintf(filename, sizeof(filename), "%d.txt", getpid());
        // printf("file name is %s",filename);
        FILE *fp = fopen(filename, "w"); // create file's name with PID num
        if (fp == NULL)
        {
            perror("fopen");
            exit(-2);
        }
        fprintf(fp, "%.2f", value); // contains the random number
        fclose(fp);

        // Send SIGUSR2 signal to parent process to indicate completion

        kill(getppid(), SIGUSR2);
    }
}

void readRange()
{
    // Open the file for reading
    FILE *fp = fopen("range.txt", "r");
    if (fp == NULL)
    {
        printf("Failed to open file\n");
        exit(-1);
    }
    // Read max and min values from file
    fscanf(fp, "%d,%d", &max, &min);
    fclose(fp);
}