#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

// Game variables
bool gameOver;
const int width = 35;
const int height = 20;
int x, y, fruitX, fruitY, score;

// Character variables
char head = 'O';
char tail = 'o';
char space = ' ';
char wall = '#';
char food = '*';

// Arrays to store tail coordinates
int tailX[100], tailY[100];
int nTail; // Length of the tail

// Enumeration for directions
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// Helper function to move cursor without clearing the screen (Fixes Flickering)
void SetCursorPosition(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

// 1. Setup the initial game state
void Setup() {
    srand(time(NULL)); // Seed random number generator
    gameOver = false;
    dir = STOP;
    x = width / 2;  // Start snake head in the center
    y = height / 2;
    fruitX = rand() % width; // Place food randomly
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

// 2. Draw the game board in the console
void Draw() {
    SetCursorPosition(0, 0); // Reset cursor to top-left instead of system("cls")

    // Top wall
    for (int i = 0; i < width + 2; i++) cout << wall;
    cout << endl;

    // Map body
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Left wall
            if (j == 0) cout << wall;

            // Draw Snake Head
            if (i == y && j == x) {
                cout << head;
            }
            // Draw Food
            else if (i == fruitY && j == fruitX) {
                cout << food;
            }
            // Draw Snake Tail
            else {
                bool printTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << tail;
                        printTail = true;
                    }
                }
                if (!printTail) cout << space;
            }

            // Right wall
            if (j == width - 1) cout << wall;
        }
        cout << endl;
    }

    // Bottom wall
    for (int i = 0; i < width + 2; i++) cout << wall;
    cout << endl;

    // Display Score
    cout << "Score: " << score << "   " << endl; // Extra spaces clear old characters
    cout << "Use W, A, S, D to move. Press X to exit." << endl;
}

// 3. Handle Keyboard Input
void Input() {
    if (_kbhit()) { // Checks if a keyboard key has been pressed
        switch (_getch()) {
        case 'a': if (dir != RIGHT) dir = LEFT; break;
        case 'd': if (dir != LEFT)  dir = RIGHT; break;
        case 'w': if (dir != DOWN)  dir = UP; break;
        case 's': if (dir != UP)    dir = DOWN; break;
        case 'x': gameOver = true; break;
        }
    }
}

// 4. Update Game Logic (Movement & Collisions)
void Logic() {
    // Save coordinates of the first tail segment
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    // Move the rest of the tail segments forward
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move the head based on direction
    switch (dir) {
    case LEFT:  x--; break;
    case RIGHT: x++; break;
    case UP:    y--; break;
    case DOWN:  y++; break;
    default: break;
    }

    // Wall collision checking
    if (x >= width || x < 0 || y >= height || y < 0) {
        gameOver = true;
    }

    // Self collision checking (hitting your own tail)
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
        }
    }

    // Food collision checking
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++; // Grow the tail
    }
}

// 5. Main Loop
int main() {
    // Hide the console cursor for a cleaner look
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(80); 
    }

    // Restore cursor before exiting
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    system("cls");
    cout << "\nGAME OVER!" << endl;
    cout << "Final Score: " << score << endl;
    return 0;
}
