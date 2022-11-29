#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <conio.h>
#include <Windows.h>

using namespace std;

struct Cell
{
    int x;
    int y;
};

struct Direction
{
    int x;
    int y;
};

const char SYMBOL_APPLE = '@';
const char SYMBOL_SNAKE_BODY = 'o';
const char SYMBOL_SNAKE_HEAD = '0';
const char SYMBOL_EMPTY = '.';
const int LOSS = 0;
const int VICTORY = 1;
const int FIELD_SIZE_ROWS = 6;
const int FIELD_SIZE_COLUMNS = 8;
const int NEXT_FRAME_TIMEOUT_MS = 300;

vector<vector<char>> NewField(int countRows, int countColumns)
{
    vector<vector<char>> field;
    for (int i = 0; i < countRows; ++i)
    {
        vector<char> NewRow(countColumns, SYMBOL_EMPTY);
        field.push_back(NewRow);
    }
    return field;
}

void PrintLine(vector<char> line)
{
    copy(line.begin(), line.end(), ostream_iterator<char>(cout, " "));
    cout << endl;
}

void PrintField(vector<vector<char>> field)
{
    for_each(field.begin(), field.end(), PrintLine);
}

void DrawSnake(vector<vector<char>>& field, deque<Cell> snake)
{
    for (Cell cell : snake) field[cell.y][cell.x] = SYMBOL_SNAKE_BODY;
    Cell head = snake.back();
    field[head.y][head.x] = SYMBOL_SNAKE_HEAD;
}

void DrawApple(vector<vector<char>>& field, Cell& apple)
{
    field[apple.y][apple.x] = SYMBOL_APPLE;
}

void DrawField(vector<vector<char>>& field, deque<Cell>& snake, Cell& apple)
{
    // Reset console position
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });

    DrawApple(field, apple);
    DrawSnake(field, snake);
    PrintField(field);
}

void FinishGame(int endCode)
{
    if (endCode == LOSS) cout << "YOU LOST :(";
    else cout << "YOU WON! :)";
    exit(0);
}

void UpdateApple(deque<Cell>& snake, Cell& apple)
{
    vector<Cell> freeCells;
    for (int i = 0; i < FIELD_SIZE_COLUMNS; ++i)
    {
        for (int j = 0; j < FIELD_SIZE_ROWS; ++j)
        {
            Cell newCell = {i, j};
            freeCells.push_back(newCell);
        }
    }
    for (auto cell : snake)
    {
        remove_if(freeCells.begin(), freeCells.end(), [&cell](Cell freeCell) mutable {
            return freeCell.x == cell.x && freeCell.y == cell.y;
        });
    }

    random_shuffle(freeCells.begin(), freeCells.end());
    apple.x = freeCells[0].x;
    apple.y = freeCells[0].y;
}

void UpdateSnake(deque<Cell>& snake, Cell& apple, Direction& snakeDirection)
{
    Cell snakeHead = snake.back();
    Cell newElem;
    newElem.x = snakeHead.x + snakeDirection.x;
    newElem.y = snakeHead.y + snakeDirection.y;
    snake.push_back(newElem);

    // If on apple skip remove => snake will grow for 1 cell
    if (snakeHead.x != apple.x || snakeHead.y != apple.y)
    {
        snake.pop_front();
    }
    else
    {
        UpdateApple(snake, apple);
    }
}

void UpdateInputDirection(Direction& snakeDirection)
{
    // Get input
    char inputKey = ' ';
    if (_kbhit())
    {
        inputKey = _getch();
    }

    if (inputKey == 'w' && snakeDirection.y != 1)
    {
        snakeDirection.x = 0;
        snakeDirection.y = -1;
    }
    if (inputKey == 's' && snakeDirection.y != -1)
    {
        snakeDirection.x = 0;
        snakeDirection.y = 1;
    }
    if (inputKey == 'a' && snakeDirection.x != 1)
    {
        snakeDirection.x = -1;
        snakeDirection.y = 0;
    }
    if (inputKey == 'd' && snakeDirection.x != -1)
    {
        snakeDirection.x = 1;
        snakeDirection.y = 0;
    }
}

void UpdateWallCollision(deque<Cell>& snake, Direction& snakeDirection)
{
    Cell snakeHead = snake.back();
    if (snakeHead.x >= FIELD_SIZE_COLUMNS ||
        snakeHead.y >= FIELD_SIZE_ROWS ||
        snakeHead.x < 0 ||
        snakeHead.y < 0
        )
    {
        if (snake.size() == 1) FinishGame(LOSS);

        snakeDirection = {0, 0};
        if (snake[1].x + 1 == snake[0].x) snakeDirection.x = 1;
        else if (snake[1].x - 1 == snake[0].x) snakeDirection.x = -1;
        else if (snake[1].y + 1 == snake[0].y) snakeDirection.y = 1;
        else if (snake[1].y - 1 == snake[0].y) snakeDirection.y = -1;

        reverse(snake.begin(), snake.end());
        snake.erase(snake.begin());
    }
}

bool CheckVictory(deque<Cell>& snake)
{
    return snake.size() == FIELD_SIZE_COLUMNS * FIELD_SIZE_ROWS;
}

int main()
{
    // INIT
    vector<vector<char>> field = NewField(FIELD_SIZE_ROWS, FIELD_SIZE_COLUMNS);

    deque<Cell> snake;
    snake.push_back({0, 0});
    snake.push_back({1, 0});

    Cell apple;
    apple.x = FIELD_SIZE_COLUMNS / 2;
    apple.y = FIELD_SIZE_ROWS / 2;

    Direction snakeDirection = {1, 0};

    // UPDATE AND DRAW LOOP
    while (true)
    {
        Sleep(NEXT_FRAME_TIMEOUT_MS);
        DrawField(field, snake, apple);

        UpdateInputDirection(snakeDirection);

        // Update field
        field = NewField(FIELD_SIZE_ROWS, FIELD_SIZE_COLUMNS);

        UpdateSnake(snake, apple, snakeDirection);

        // Check loss
        UpdateWallCollision(snake, snakeDirection);
        for (int i = 0; i < snake.size(); ++i)
        {
            for (int j = i + 1; j < snake.size(); ++j)
            {
                if (snake[i].x == snake[j].x && snake[i].y == snake[j].y) FinishGame(LOSS);
            }
        }

        if (CheckVictory(snake)) FinishGame(VICTORY);
    }
}
