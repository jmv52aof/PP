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
const int FIELD_SIZE_ROWS = 10;
const int FIELD_SIZE_COLUMNS = 10;

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

void printLine(vector<char> line)
{
    copy(line.begin(), line.end(), ostream_iterator<char>(cout, " "));
    cout << endl;
}

void printField(vector<vector<char>> field)
{
    for_each(field.begin(), field.end(), printLine);
}

void drawSnake(vector<vector<char>>& field, vector<Cell> snake)
{
    for (Cell cell : snake)
    {
        field[cell.y][cell.x] = SYMBOL_SNAKE;
    }
}

void drawApple(vector<vector<char>>& field, Cell& apple)
{
    field[apple.y][apple.x] = SYMBOL_APPLE;
}

void updateApple(vector<Cell>& snake, Cell& apple)
{
    vector<Cell> freeCells;
    for (int i = 0; i < FIELD_SIZE_ROWS; ++i)
    {
        for (int j = 0; j < FIELD_SIZE_COLUMNS; ++j)
        {
            Cell newCell = {i, j};
            freeCells.push_back(newCell);
        }
    }
    for (auto cell : snake)
    {
        remove_if(freeCells.begin(), freeCells.end(), [&cell] (Cell freeCell) mutable {
            return freeCell.x == cell.x && freeCell.y == cell.y;
        });
    }

    random_shuffle(freeCells.begin(), freeCells.end());
    apple.x = freeCells[0].x;
    apple.y = freeCells[0].y;
}

void draw(vector<vector<char>>& field, vector<Cell>& snake, Cell& apple)
{
    // system("cls"); // Clear console
    cout << "\x1B[2J\x1B[H";
    drawApple(field, apple);
    drawSnake(field, snake);
    printField(field);
}

int main()
{
    // INIT
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
        // system("timeout /t 0");
        Sleep(200);
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
        if (inputKey == 'w' && directionY != 1)
        {
            directionX = 0;
            directionY = -1;
        }
        if (inputKey == 's' && directionY != -1)
        {
            directionX = 0;
            directionY = 1;
        }
        if (inputKey == 'a' && directionX != 1)
        {
            directionX = -1;
            directionY = 0;
        }
        if (inputKey == 'd' && directionX != -1)
        {
            directionX = 1;
            directionY = 0;
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
            updateApple(snake, apple);
        }

        // Check fail
        snakeHead = snake.back();
        if (snakeHead.x >= FIELD_SIZE_ROWS ||
            snakeHead.y >= FIELD_SIZE_COLUMNS ||
            snakeHead.x < 0 ||
            snakeHead.y < 0
           )
        {
            if (snake.size() == 1)
            {
                cout << "YOU ARE LOOSE" << endl;
                return 0;
            }

            if (snake[1].x + 1 == snake[0].x) directionX = -1;
            if (snake[1].y + 1 == snake[1].y) directionY = -1;
            reverse(snake.begin(), snake.end());
            snake.erase(snake.begin());
            directionY = 1;
            directionX = 0;
        }

        for (int i = 0; i < snake.size(); ++i)
        {
            for (int j = i + 1; j < snake.size(); ++j)
            {
                if (snake[i].x == snake[j].x && snake[i].y == snake[j].y)
                {
                    cout << "YOU ARE LOOSE" << endl;
                    return 0;
                }
            }
        }

    }
}
