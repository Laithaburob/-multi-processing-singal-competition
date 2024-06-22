// C Program to illustrate
// OpenGL animation for revolution

#include "header.h"
#include "functions.h"
#include "constants.h"

// global declaration
#define PLAYER_SIZE 20

float team_a_x_positions[TEAM_SIZE] = {0, 0};
float team_b_x_positions[TEAM_SIZE];

int scoreA = 0;
int scoreB = 0;
float sumA = 0;
float sumB = 0;
int round_score = 1;
int max_num_rounds = 0;
int tempRound = 1;

bool game_finished = false;
bool winner_is_teamA = false;
bool winner_is_teamB = false;
bool draw = true;

float teamAXOffset = 0;
float teamBXOffset = 0;
int roundfd;
char msg_receive[BUFFER_SIZE];

void drawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= num_segments; i++)
    {
        float angle = i * 2.0f * M_PI / num_segments; // angle in radians
        float x = r * cosf(angle);
        float y = r * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void display(void)
{

    // clear fram buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // reset coordinate system
    glLoadIdentity();

    gluOrtho2D(-800, 800, -800, 800);
    glPointSize(PLAYER_SIZE);

    glBegin(GL_POINTS);

    // GREEN A TEAM LOCATIONS
    glColor3f(0.0, 1.0, 0.0);
    glVertex2i(-400 + teamAXOffset, 130);
    // glVertex2i(200 + teamAXOffset, 100);

    // RED B TEAM LOCATIONS
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(-400 + teamBXOffset, -90);
    // glVertex2i(200, -100);

    glColor3f(1.0, 1.0, 1.0);

    glEnd();

    // Print Score for team 1
    char score_A_buffer[3];
    sprintf(score_A_buffer, "%d", scoreA);
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(-150, 500); // define position on the screen
    for (size_t i = 0; i < 2; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score_A_buffer[i]);
    }
    glEnd();

    // Print Score for team 2
    char score_B_buffer[3];
    sprintf(score_B_buffer, "%d", scoreB);
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(350, 500); // define position on the screen
    for (size_t i = 0; i < 2; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score_B_buffer[i]);
    }
    glEnd();

    // Print Round Number
    char round_buffer[3];
    sprintf(round_buffer, "%d", round_score);
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(200, -550); // define position on the screen
    for (size_t i = 0; i < 2; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, round_buffer[i]);
    }
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-400, 400); // define position on the screen
    char *string6 = "|___________________________________________|";
    while (*string6)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string6++);
    }

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-700, 0); // define position on the screen
    char *string5 = "START----------------------------------------------------FINISH";

    while (*string5)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string5++);
    }

    // print the competition
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-100, 650); // define position on the screen
    char *string1 = "The Competition";

    while (*string1)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string1++);
    }

    // print score of team A
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(-300, 500);
    char *stringA = "Team A: ";

    while (*stringA)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *stringA++);
    }

    // print score of team B
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(200, 500);
    char *stringB = "Team B: ";
    while (*stringB)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *stringB++);
    }

    // print round number
       // glRasterPos2f(-100, 650); // define position on the screen

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-75, -550);
    char *round_num = "Round Number:";
    while (*round_num)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *round_num++);
    }

    if (game_finished)
    {

        if (scoreA > scoreB)
        {
            glColor3f(0.0, 1.0, 0.0);
            glRasterPos2f(-75, -400);
            char *string4 = "Team A Wins";

            while (*string4)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string4++);
            }
        }
        else if (scoreA < scoreB)
        {
            glColor3f(1.0, 0.0, 0.0);
            glRasterPos2f(-75, -400);
            char *string4 = "Team B Wins";

            while (*string4)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string4++);
            }
        }
        else if (scoreA == scoreB)
        {
            glColor3f(1.0, 1.0, 0.0);
            glRasterPos2f(-75, -400);
            char *string4 = "Draw (^_^)";

            while (*string4)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string4++);
            }
        }
    }

    // to fix jitters
    glutSwapBuffers();
}

void update()
{

    read(roundfd, &msg_receive, BUFFER_SIZE);
    sscanf(msg_receive, "%d,%d,%d,%d,%f,%f", &max_num_rounds, &round_score, &scoreA, &scoreB, &sumA, &sumB);

    if (max_num_rounds == round_score)
    {
        game_finished = true;
    }

    if (!game_finished)
    {
        teamAXOffset += 60;
        teamBXOffset += 60;
        if (round_score == tempRound + 1)
        {
            
            tempRound++;
            teamAXOffset = 0;
            teamBXOffset = 0;
            
        }
        if (teamAXOffset > sumA*4)
        {
            teamAXOffset = sumA*4;
        }
        if (teamBXOffset > sumB*4)
        {
            teamBXOffset = sumB*4;
        }
    }
    else
    {
        teamAXOffset = 0;
        teamBXOffset = 0;
    }

    glutPostRedisplay(); // recall display for OpenGL animation
    glutTimerFunc(200, update, 0);
}

int main(int argc, char **argv)
{
    roundfd = open("openglFifo", O_RDONLY | O_NONBLOCK);
    if (roundfd == -1)
    {
        perror("roundFifo");
        exit(1);
    }

    printf("Starting UI...\n");
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // giving window size in X- and Y- direction
    glutInitWindowPosition(200, 300);
    glutInitWindowSize(800, 400);

    // Giving name to window
    glutCreateWindow("The competition");

    // glClearColor(0.5, 0.5, 1.0, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutDisplayFunc(display);
    glutTimerFunc(500, update, 0);
    glutMainLoop();
    // window background color

    close(roundfd);
    return 0;
}
