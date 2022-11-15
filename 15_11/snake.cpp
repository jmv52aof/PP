#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <Windows.h>

using namespace std;

struct Cell
{
    int x;
    int y;
};

const char SYMBOL_APPLE = '@';
const char SYMBOL_SNAKE = '#';
const char SYMBOL_EMPTY = '.';

void printLine(vector<char> line)
{
    copy(line.begin(), line.end(), ostream_iterator<char>(cout, ""));
    cout << endl;
}

void printField(vector<vector<char>> field)
{
    for_each(field.begin(), field.end(), printLine);
}

void setCursorPos(int x, int y) 
{
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

void drawSnake(vector<vector<char>>& field, vector<Cell> snake)
{
    for (Cell cell : snake)
    {
        field[cell.x][cell.y] = SYMBOL_SNAKE;
    }
}

void drawApple(vector<vector<char>>& field, Cell apple)
{
    field[apple.x][apple.y] = SYMBOL_APPLE;
}

vector<vector<char>> newField(int countRows, int countColumns)
{
    vector<vector<char>> field;
    for (int i = 0; i < countRows; ++i)
    {
        vector<char> newRow(countColumns, SYMBOL_EMPTY);
        field.push_back(newRow);
    }
    return field;
}

void draw(vector<vector<char>>& field, vector<Cell>& snake, Cell& apple)
{
    system("cls"); // Empty console
    drawSnake(field, snake);
    drawApple(field, apple);
    printField(field);
}

int main()
{
    // INIT
    const int FIELD_SIZE_ROWS = 20;
    const int FIELD_SIZE_COLUMNS = 20;
    vector<vector<char>> field = newField(FIELD_SIZE_ROWS, FIELD_SIZE_COLUMNS);

    vector<Cell> snake;
    snake.push_back({0, 0});
    snake.push_back({0, 1});

    Cell apple;
    apple.x = 2;
    apple.y = 2;

    int directionX = 0;
    int directionY = 1;

    // UPDATE AND DRAW LOOP
    while (true)
    {
        system("timeout /t 0");
        draw(field, snake, apple);

        // Get input
        char inputKey = ' ';
        if (_kbhit())
        {
            inputKey = _getch();
        }

        // Update field
        field = newField(FIELD_SIZE_ROWS, FIELD_SIZE_COLUMNS);

        // Update snake
        if (inputKey == 'w')
        {
            directionX = -1;
            directionY = 0;
        }
        if (inputKey == 's')
        {
            directionX = 1;
            directionY = 0;
        }
        if (inputKey == 'a')
        {
            directionX = 0;
            directionY = -1;
        }
        if (inputKey == 'd')
        {
            directionX = 0;
            directionY = 1;
        }

        Cell snakeHead = snake.back();
        Cell newElem;
        newElem.x = snakeHead.x + directionX;
        newElem.y = snakeHead.y + directionY;
        snake.push_back(newElem);

        // If on apple skip remove => snake will grow for 1 cell
        if (snakeHead.x != apple.x || snakeHead.y != apple.y)
        {
            snake.erase(snake.begin());
        }
        else
        {
  
        }

        // Check fail
        snakeHead = snake.back();
        if (snakeHead.x >= FIELD_SIZE_ROWS
            || snakeHead.y >= FIELD_SIZE_COLUMNS
            || snakeHead.x < 0
            || snakeHead.y < 0
            )
        {
            cout << "YOU ARE LOOSE" << endl;
            break;
        }
    }
}
