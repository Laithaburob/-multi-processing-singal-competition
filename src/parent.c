// Include necessary header files
#include "header.h"
#include "functions.h"
#include "constants.h"

// Define global variables
int number_of_rounds = 0;
pid_t children[NUM_CHILDREN];
pid_t pid, openglPID;
int cnt = 0;
char msg[BUFFER_SIZE];
char msg_from_child[BUFFER_SIZE];
int fd[2];
char msg_sendto_opengl[BUFFER_SIZE];

float sum_of_teamA = 0;
float sum_of_teamB = 0;

int wins_of_teamA = 0;
int wins_of_teamB = 0;

// Declare function prototypes
void validateInput(int argc, char **argv);
void handler1();
void handler2();
void handler3();

int main(int argc, char **argv)
{

    printf("Starting Execution of The Competition....\n");
    // Set signal handler for SIGUSR2
    signal(SIGUSR2, handler1);

    // Validate command line input
    validateInput(argc, argv);

    openglPID = fork();
    switch (openglPID)
    {
    case -1: // failed to create opengl id
        exit(-1);
        break;

    case 0: // currently in child
        execl("./opengl", "OpenGL", NULL);
        break;
    default:
        sleep(2); // wait a bit for the opengl to run
        break;
    }

    // crete fifos
    if (mkfifo("openglFifo", 0777) == -1)
    {
        if (errno != EEXIST)
        {
            perror("Create fifo failed\n");
            exit(-1);
        }
    }

    // Create a pipe
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(-1);
    }
    int counter = 0;
    // Create children processes
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid = fork();

        // Check for errors when forking child processes
        if (pid == -1)
        {
            printf("Error: Failed to fork child %d\n", i + 1);
            exit(-1);
        }
        // Code executed in child process
        else if (pid == 0)
        {
            // If i is less than 4, execute child process
            if (i < 4)
            {
                execl("./child", "child", NULL);
            }
            // If i is greater than or equal to 4, execute co-processor process
            else
            {
                char fd0[10], fd1[10];
                sprintf(fd0, "%d", fd[0]);
                sprintf(fd1, "%d", fd[1]);

                execl("./child", "co-processor", fd0, fd1, NULL);
            }
            break;
        }
        // Code executed in parent process
        else
        {
            // Store the child process IDs in the children array
            children[i] = pid;
            counter++; // count the number of children created
            if (counter == NUM_CHILDREN)
            {
                for (int j = 0; j < 2; j++)
                {
                    teamA[j] = children[j];
                    teamB[j] = children[j + 2];
                }
                green();
                printf("\n\t\tID %d is in team A", teamA[0]);
                printf("\n\t\tID %d is in team A", teamA[1]);
                red();
                printf("\n\t\tID %d is in team B", teamB[0]);
                printf("\n\t\tID %d is in team B", teamB[1]);
                resetColor();
                printf("\n\n");
                // loop thought the number of rounds
                for (int i = 0; i < number_of_rounds; i++)
                {
                    blue();
                    printf("\n--------------------Round %d started--------------------\n\n", i + 1);
                    resetColor();
                    generateRandom(1, 100);

                    for (int i = 0; i < 4; i++)
                    {
                        // printf("pid sent is %d \n",children[i]);
                        kill(children[i], SIGUSR1);
                        sleep(1);
                    }

                    int rfd = open("openglFifo", O_WRONLY);
                    if (rfd == -1)
                    {
                        perror("open");
                        exit(-1);
                    }
                    int k = i + 1;
                    snprintf(msg_sendto_opengl, BUFFER_SIZE, "%d,%d,%d,%d,%0.2f,%0.2f",
                             number_of_rounds, k, wins_of_teamA, wins_of_teamB, sum_of_teamA, sum_of_teamB);

                    // printf("msg_sendto_opengl is %s\n", msg_sendto_opengl);
                    write(rfd, msg_sendto_opengl, strlen(msg_sendto_opengl) + 1);
                    close(rfd);

                    blue();
                    printf("\n--------------------Round %d is done--------------------\n\n", i + 1);
                    resetColor();
                    sleep(3);
                }
            }

            // waitpid(pid, NULL, 0);
        }
    }
    printf("\t\tTEAM A: %d  TEAM B: %d\n", wins_of_teamA, wins_of_teamB);

    if (wins_of_teamA > wins_of_teamB)
    {
        green();
        printf("\t\t\tTeam A wins\n");
        resetColor();
    }
    else if (wins_of_teamA < wins_of_teamB)
    {
        red();
        printf("\t\t\tTeam B wins\n");
        resetColor();
    }
    else
    {
        yellow();
        printf("\t\t\tDraw\n");
        resetColor();
    }
    blue();
    printf("\n-----------------The Competition Ended-----------------\n\n");
    resetColor();
    char file[20];
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        sprintf(file, "%d.txt", children[i]);
        remove(file);
        kill(children[i], SIGKILL);
    }

    sleep(10);
    kill(openglPID, SIGKILL);

    return 0;
}

// Function to validate command line input
void validateInput(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("\nDefault number of rounds is selected: %d\n", DEFAULT_NUM_ROUNDS);
        number_of_rounds = DEFAULT_NUM_ROUNDS;
    }
    else
    {
        if (!(number_of_rounds = atoi(argv[1])))
        {
            perror("\nError: The first argument must be an integer");
            exit(-1);
        }
    }
}

// function to handle the signal SIGUSR1
void handler1()
{
    cnt++;
    printf("\nsignal %d recieved from child ....", cnt);
    if (cnt == 4)
    {
        memset(msg, 0, sizeof(msg));
        memset(msg_from_child, 0, sizeof(msg_from_child)); // Clear global variables
        cnt = 0;
        printf("\n\tAll four recieved\n\n");

        float values[4];
        FILE *fp;
        char filename[10];
        char buffer[10] = "";

        for (int i = 0; i < 4; i++)
        {
            sprintf(filename, "%d.txt", children[i]);
            fp = fopen(filename, "r");
            fscanf(fp, "%f", &values[i]);
            if (i > 0)
            {
                strcat(msg, ","); // add comma separator
            }
            sprintf(buffer, "%.2f", values[i]); // convert float to string
            strcat(msg, buffer);
        }
        // printf("Message: %s\n", msg);

        // sleep(5);
        if (write(fd[1], msg, strlen(msg)) != -1)
        {
            printf("Message sent by parent to the CO-Processor: [%s]\n", msg);
            fflush(stdout);
        }
        else
        {
            perror("Write");
            exit(5);
        }

        kill(children[4], SIGUSR1);

        signal(SIGINT, handler3);
        pause();

        read(fd[0], msg_from_child, BUFFER_SIZE);
        printf("Message received by parent: [%s]\n", msg_from_child);
        fflush(stdout);

        char *token;
        token = strtok(msg_from_child, ",");
        if (token != NULL)
            sum_of_teamA = atof(token);

        token = strtok(NULL, ",");
        if (token != NULL)
            sum_of_teamB = atof(token);
        printf("\n");
        green();
        printf("Sum of Team [A] = [%.2f]\n", sum_of_teamA);
        red();
        printf("Sum of Team [B] = [%.2f]\n", sum_of_teamB);
        resetColor();
        if (sum_of_teamA > sum_of_teamB)
        {
            wins_of_teamA++;
            green();
            printf("\n\t\t--Team A wins This round--\n");
            resetColor();
        }
        else if (sum_of_teamA < sum_of_teamB)
        {
            wins_of_teamB++;
            red();
            printf("\n\t\t--Team B wins This round--\n");
            resetColor();
        }
        else
        {
            yellow();
            printf("\n\t\t --Draw--\n");
            resetColor();
        }
    }
}

// Function to handle the signal SIGUSR2
void handler2()
{
    printf("Start Reading from the pipe\n");
    fflush(stdout);
}
void handler3()
{
    printf("Parent start Reading from the COPROCESSOR\n");
    fflush(stdout);
}