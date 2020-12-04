#include <iostream> //for cout and cin
#include <conio.h>  //for _kbhit and _getch
#include <windows.h>    //for sleep
#include <ctime>    //for time

using namespace std;
//=====================debugging code=========================
bool showflowDebugging = false;

void showflow(string location)
{
    if (showflowDebugging)
    {
        cout << "<--. at location: " << location << endl;
    }
}   //showflow
//=============================================================
bool gameOver;

bool hitWallOption;

const int screenWidth = 20;
const int screenHeight = 20;

char userInput;
const int numFruit = 3;
int SnakeX, SnakeY, fruitX[numFruit], fruitY[numFruit], score;
int sleepAmt;
const int maxTailLength = 100;
int tailX[maxTailLength], tailY[maxTailLength];
int tailLength;
enum directionType { STOP = 0, LEFT, RIGHT, UP, DOWN};
directionType direction;

void option ()
{
    char gameChoice;
    cout << "Do you want to: " << endl;
    cout << "a): Play with wall boundaries" << endl;
    cout << "b): Play without wall boundaries" << endl;
    cin >> gameChoice;
    if (gameChoice == 'a')
        hitWallOption = true;
}   //choose game mode option

void instructions ()
{
    cout << "Key Controls (Case Insensitive):" << endl;
    cout << "W = Up, S = Down, A = Left, D = Right" <<endl;
    cout << "P = Pause, X = Exit, Q = Exit" << endl;
    cout << "Collect Fruits (F) to increase your score" << endl;
    cout << "Aim for the highest score!" << endl;
}   //instructions

void snakeSpawn ()
{
    SnakeX = screenWidth / 2;
    SnakeY = screenHeight / 2;
}   //spawn snake

void fruitSpawn ()
{
    for (int i = 0; i < numFruit; i++)
    {
        fruitX[i] = rand() % screenWidth;
        fruitY[i] = rand() % screenHeight;
        //if (fruitX[i] == fruitX[i] - 1 || fruitY[i] == fruitY[i] - 1)
        //   continue;
    }
}   //spawn fruit

void restart ()
{
    snakeSpawn();
    fruitSpawn();
    gameOver = false;
    hitWallOption = false;
    direction = STOP;
    tailLength = 0;
    score = 0;
    sleepAmt = 10;
}   //play again restart

void Setup ()
{
    snakeSpawn();
    fruitSpawn();
    gameOver = false;
    hitWallOption = false;
    direction = STOP;
    srand(time(0)); // seed the random number generator
    score = 0;
    sleepAmt = 10;
}   //Setup

void drawTopOrBottomRow ()
{
    for (int i = 0; i < screenWidth+2; i++)
        cout << "#";
    cout << endl;
} //drawTopOrBottomRow

void hitWall ()
{
    if (SnakeX == screenWidth || SnakeX == 0 || SnakeY == screenHeight || SnakeY == -1)
    {
        gameOver = true;
        cout << "You hit a wall!" << endl;
    }
}   //hit a wall

void drawRestOfBoard()
{
    for (int i = 0; i < screenHeight; i++)
    {
        bool fruitExist = false;
        bool snakeExist = false;
        for (int j = 0; j < screenWidth +2; j++)
        {
            for (int k = 0; k < numFruit; k++)
            {
                if (i == fruitY[k] && j == fruitX[k])
                {
                    fruitExist = true;
                    cout << "F";
                }
            }
            if (j == 0)
                cout << "#";

            else if ((!fruitExist && i == SnakeY && j == SnakeX)
                     || (fruitExist && i == SnakeY && j == SnakeX - 1)) //second case: there is a fruit, making the "F" take an extra space
            {                                                           //making the snake head  move to the right 1,
                cout << "O";                                            //fixed by making snake head move left by 1
                snakeExist = true;
            }

            else
            {
                bool print = false;
                for (int l = 0; l < tailLength; l++)
                {
                    if ((j == tailX[l] && i == tailY[l] && !fruitExist)
                        || (j == tailX[l] - 1 && i == tailY[l] && fruitExist)) //second case: there is a fruit, making the "F" take an extra space
                    {                                                          //making the snake head  move to the right 1,
                        snakeExist = true;                                     //fixed by making snake head move left by 1
                        cout << "o";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if ( (fruitExist && snakeExist && j == screenWidth - 1 -1) ||
                 (fruitExist && !snakeExist && j == screenWidth - 1 -1) ||
                 (!fruitExist && snakeExist && j == screenWidth - 1) ||
                  j == screenWidth - 1)
            {
                cout << "#";
                break;
            }
        }
        cout << endl;
    }
}   //drawRestOfBoard

void showUserScore()
{
    cout << "Score:" << score << endl;
}   //showUserScore

void Draw()
{
    system("cls"); //system("clear");
    instructions();
    drawTopOrBottomRow();
    drawRestOfBoard();
    drawTopOrBottomRow();
    showUserScore();
} //Draw Function

void Delay(int numMs)
{
    Sleep(numMs);
}   //Delay

void gameControls()
{
    if (_kbhit())
    {
        userInput = tolower(_getch());
        switch (userInput)
        {
        case 'a':
            direction = LEFT;
            break;
        case 'd':
            direction = RIGHT;
            break;
        case 'w':
            direction = UP;
            break;
        case 's':
            direction = DOWN;
            break;
        case '+':
            if (sleepAmt <= 100)
                sleepAmt += 5;
            break;
        case '-':
            if (sleepAmt >= 25)
                sleepAmt -= 5;
            break;
        case 'q':
        case 'x':
            cout << "You pressed 'q' or 'x' " << endl;
            gameOver = true;
            break;
        case 'p':
            cout << "The game is paused. Press any key to continue." << endl;
            while (!_kbhit())
            {
                Delay(sleepAmt);
            }
            break;
        }
    }
} //Input Function

void moveSnakeHead ()
{
    switch (direction)
    {
    case LEFT:
        SnakeX--;
        break;
    case RIGHT:
        SnakeX++;
        break;
    case UP:
        SnakeY--;
        break;
    case DOWN:
        SnakeY++;
        break;
    default:
        break;
    }
}   //moveSnakeHead

void repositionSnake ()
{
    //if (x > screenWidth || x < 0 || y > screenHeight || y < 0)
    //  gameOver = true;
    if (SnakeX >= screenWidth)
        SnakeX = 0;
    else if (SnakeX <= 0)
        SnakeX = screenWidth - 1;
    if (SnakeY >= screenHeight)
        SnakeY = 0;
    else if (SnakeY < 0)
        SnakeY = screenHeight - 1;
} //repositionSnake

void snakeEatsFruit ()
{
    for (int i = 0; i < numFruit; i++)
    {
        if (SnakeX == fruitX[i] && SnakeY == fruitY[i])
        {
            score += 10;
            fruitX[i] = rand() % screenWidth;
            fruitY[i] = rand() % screenHeight;
            tailLength++;
        }
    }
}   //snake gets fruit and bigger

void snakeRunIntoTail ()
{
    for (int i = 0; i < tailLength; i++)
        if (tailX[i] == SnakeX && tailY[i] == SnakeY)
        {
            gameOver = true;
            cout << "You ran into your own tail!" << endl;
        }
}   //snake runs into it's tail

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = SnakeX;
    tailY[0] = SnakeY;
    for (int i = 1; i < tailLength; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    moveSnakeHead();
    if (!hitWallOption)
        repositionSnake();
    else
        hitWall();
    snakeRunIntoTail();
    snakeEatsFruit();
}   //logic function

int main()
{
    Setup();
    option();
    char gameChoice, choice;
    while (true)
    {
        while (!gameOver)
        {
            Draw();
            gameControls();
            Logic();
            Delay(sleepAmt); //sleep(10);
        }
        if (gameOver)
        {
            restart();
            cout << "Do you want to (P)lay Again or (Q)uit?" << endl;
            cin >> choice;
            if (choice == 'q')
                break;
            else
            {
                option();
                continue;
            }
        }
    }
    return 0;
}
