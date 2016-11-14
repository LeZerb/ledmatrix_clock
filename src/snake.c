#include <stdlib.h>
#include "common.h"
#include "date.h"
#include "display.h"

#define MAX_SNAKE_LEN 90
#define SNAKE_COLS NUM_COLS
#define SNAKE_ROWS (NUM_ROWS - 1)

typedef enum {
    eDIR_FIRST = 1, // start at one to make the check easier
    eDIR_UP = eDIR_FIRST,
    eDIR_RIGHT,
    eDIR_DOWN,
    eDIR_LEFT,
    eDIR_LAST = eDIR_LEFT
} TE_DIRECTION;

typedef struct {
    U8 u8Col : 4;
    U8 u8Row : 4;
} TS_COORDS;

typedef struct {
    TS_COORDS snake[MAX_SNAKE_LEN];
    U8 indexHead;
    U8 indexTail;
    TE_DIRECTION headDirection;
} TS_SNAKE;

static TS_SNAKE _snake;
static TS_COORDS _apple;
static BOOL _gameOver = FALSE;

void snakeInit() {
    TS_TIME time;
    //create the initial snake

    //initialize random generator
    timeGet(&time);
    srand(time.u8Hour * 60ul + time.u8Minute * 60 + time.u8Second);

    //clear the pattern
    vClearPattern();

    _snake.snake[0].u8Col = 1;
    _snake.snake[0].u8Row = 5;
    _snake.snake[1].u8Col = 2;
    _snake.snake[1].u8Row = 5;
    _snake.snake[2].u8Col = 3;
    _snake.snake[2].u8Row = 5;
    _snake.snake[3].u8Col = 4;
    _snake.snake[3].u8Row = 5;
    _snake.indexTail = 0;
    _snake.indexHead = 3;
    _snake.headDirection = eDIR_RIGHT;

    vSetInPattern(_snake.snake[0].u8Col, _snake.snake[0].u8Row, 1);
    vSetInPattern(_snake.snake[1].u8Col, _snake.snake[1].u8Row, 1);
    vSetInPattern(_snake.snake[2].u8Col, _snake.snake[2].u8Row, 1);
    vSetInPattern(_snake.snake[3].u8Col, _snake.snake[3].u8Row, 1);

    //create an apple
    _apple.u8Col = rand() % SNAKE_COLS;
    _apple.u8Row = rand() % SNAKE_ROWS;

    vSetInPattern(_apple.u8Col, _apple.u8Row, 1);

    _gameOver = FALSE;
}

BOOL snakeRun(TE_BUTTONS button) {
    U8 nextCol;
    U8 nextRow;
    U8 index;

    if (_gameOver) {
        return TRUE;
    }

    if (button == eBUTTON_MENU) {
        _snake.headDirection--;
        if (_snake.headDirection < eDIR_FIRST) {
            _snake.headDirection = eDIR_LAST;
        }
    } else if (button == eBUTTON_SET) {
        _snake.headDirection++;
        if (_snake.headDirection > eDIR_LAST) {
            _snake.headDirection = eDIR_FIRST;
        }
    }

    if (msSinceLastStart(eMS_COUNT_SNAKE) < 200) {
        return _gameOver;
    }
    msStart(eMS_COUNT_SNAKE);

    nextCol = _snake.snake[_snake.indexHead].u8Col;
    nextRow = _snake.snake[_snake.indexHead].u8Row;

    switch (_snake.headDirection) {
        case eDIR_LEFT:
            nextCol--;
            if (nextCol >= SNAKE_COLS) {
                nextCol = SNAKE_COLS - 1;
            }
            break;

        case eDIR_RIGHT:
            nextCol++;
            if (nextCol >= SNAKE_COLS) {
                nextCol = 0;
            }
            break;

        case eDIR_UP:
            nextRow--;
            if (nextRow >= SNAKE_ROWS) {
                nextRow = SNAKE_ROWS - 1;
            }
            break;

        case eDIR_DOWN:
            nextRow++;
            if (nextRow >= SNAKE_ROWS) {
                nextRow = 0;
            }
            break;
    }

    if (nextCol == _apple.u8Col && nextRow == _apple.u8Row) {
        // ate an apple -> we need a new
        _apple.u8Col = rand() % SNAKE_COLS;
        _apple.u8Row = rand() % SNAKE_ROWS;
        // no need to remove the apple from display - is the new head
    } else {
        //still hungry -> remove tail
        //only remove the tail if there is no apple in the same place
        vSetInPattern(_snake.snake[_snake.indexTail].u8Col,
                _snake.snake[_snake.indexTail].u8Row, 0);
        _snake.indexTail++;
        if (_snake.indexTail >= MAX_SNAKE_LEN) {
            _snake.indexTail = 0;
        }
    }

    //display new head and save
    vSetInPattern(nextCol, nextRow, 1);
    _snake.indexHead++;
    if (_snake.indexHead >= MAX_SNAKE_LEN) {
        _snake.indexHead = 0;
    }
    _snake.snake[_snake.indexHead].u8Col = nextCol;
    _snake.snake[_snake.indexHead].u8Row = nextRow;

    if (_snake.indexHead == _snake.indexTail) {
        //if tail is reached move tail forward
        _snake.indexTail++;
        if (_snake.indexTail >= MAX_SNAKE_LEN) {
            _snake.indexTail = 0;
        }
    }

    //display apple
    vSetInPattern(_apple.u8Col, _apple.u8Row, 1);

    index = _snake.indexTail;
    do {
        if ((_snake.snake[_snake.indexHead].u8Col ==
                _snake.snake[index].u8Col) &&
                (_snake.snake[_snake.indexHead].u8Row ==
                _snake.snake[index].u8Row)) {
            //we hit the snakes body -> game over
            _gameOver = TRUE;
            break;
        }
        index++;
        if (index >= MAX_SNAKE_LEN) {
            index = 0;
        }
    } while (index != _snake.indexHead);

    return _gameOver;
}
