#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 9;
const int HEIGHT = 9;
const int MINES = 10;

char board[WIDTH][HEIGHT];        // Board displaying the current state of the game (revealed squares)
char displayBoard[WIDTH][HEIGHT]; // Board with the mines and numbers for the player to see

void initializeBoard() {
    // Initialize the board
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            board[i][j] = '-';        // Mark all squares as unrevealed
            displayBoard[i][j] = '-'; // Same for the display board
        }
    }

    // Place mines randomly
    int minesPlaced = 0;
    while (minesPlaced < MINES) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

        // Only place a mine if there isn't already one at this location
        if (displayBoard[x][y] != '*') {
            displayBoard[x][y] = '*'; // Place a mine
            minesPlaced++;
        }
    }

    // Fill numbers indicating how many mines are adjacent to each square
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            if (displayBoard[i][j] == '*') {
                continue;
            }
            int mineCount = 0;
            // Check all adjacent squares
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (i + dx >= 0 && i + dx < WIDTH && j + dy >= 0 && j + dy < HEIGHT) {
                        if (displayBoard[i + dx][j + dy] == '*') {
                            mineCount++;
                        }
                    }
                }
            }
            if (mineCount > 0) {
                displayBoard[i][j] = '0' + mineCount; // Convert the count to a char
            }
        }
    }
}

void printBoard() {
    cout << "Minesweeper Board:" << endl;
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

bool isValidMove(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && board[x][y] == '-';
}

bool revealSquare(int x, int y) {
    if (board[x][y] != '-') {
        return false;
    }

    // Reveal the square
    board[x][y] = displayBoard[x][y];

    // If it's a mine, the game is over
    if (board[x][y] == '*') {
        return true;
    }

    // If it's an empty square, we reveal adjacent squares
    if (board[x][y] == '0') {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (isValidMove(x + dx, y + dy)) {
                    revealSquare(x + dx, y + dy);
                }
            }
        }
    }

    return false;
}

bool isGameWon() {
    // Check if all non-mine cells are revealed
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            if (displayBoard[i][j] != '*' && board[i][j] == '-') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    srand(time(0)); // Seed the random number generator

    initializeBoard(); // Initialize the game board with mines and numbers

    int x, y;
    bool gameOver = false;

    while (!gameOver && !isGameWon()) {
        printBoard(); // Print the current board

        // Get user input
        cout << "Enter row and column to reveal (0-based index): ";
        cin >> x >> y;

        if (!isValidMove(x, y)) {
            cout << "Invalid move! Try again." << endl;
            continue;
        }

        // Reveal the selected square
        gameOver = revealSquare(x, y);
    }

    if (gameOver) {
        cout << "Game Over! You hit a mine!" << endl;
    } else {
        cout << "Congratulations! You've won!" << endl;
    }

    return 0;
}
