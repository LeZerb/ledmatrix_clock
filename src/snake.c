#include <stdlib.h>
#include "common.h"
#include "date.h"
#include "display.h"

#define MAX_SNAKE_LEN 90

typedef enum {
    eDIR_FIRST,
    eDIR_UP,
    eDIR_RIGHT,
    eDIR_DOWN,
    eDIR_LEFT,
    eDIR_COUNT
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
    _apple.u8Col = rand() % NUM_COLS;
    _apple.u8Row = rand() % (NUM_ROWS - 1);

    vSetInPattern(_apple.u8Col, _apple.u8Row, 1);
}

BOOL snakeRun(TE_BUTTONS button) {
    U8 nextCol;
    U8 nextRow;
    U8 index;

    if (button == eBUTTON_MENU) {
        _snake.headDirection--;
        if (_snake.headDirection == eDIR_FIRST) {
            _snake.headDirection = eDIR_COUNT - 1;
        }
    } else if (button == eBUTTON_SET) {
        _snake.headDirection++;
        if (_snake.headDirection == eDIR_COUNT) {
            _snake.headDirection = eDIR_FIRST + 1;
        }
    }

    if (msSinceLastStart(eMS_COUNT_SNAKE) < 200) {
        return FALSE;
    }
    msStart(eMS_COUNT_SNAKE);

    switch (_snake.headDirection) {
        case eDIR_LEFT:
            nextCol = _snake.snake[_snake.indexHead].u8Col--;
            if (nextCol >= NUM_COLS) {
                nextCol = NUM_COLS - 1;
            }
            nextRow = _snake.snake[_snake.indexHead].u8Row;
            break;

        case eDIR_RIGHT:
            nextCol = _snake.snake[_snake.indexHead].u8Col++;
            if (nextCol >= NUM_COLS) {
                nextCol = 0;
            }
            nextRow = _snake.snake[_snake.indexHead].u8Row;
            break;

        case eDIR_UP:
            nextRow = _snake.snake[_snake.indexHead].u8Row--;
            if (nextRow >= (NUM_ROWS - 1)) {
                nextRow = NUM_ROWS - 2;
            }
            nextCol = _snake.snake[_snake.indexHead].u8Col;
            break;

        case eDIR_DOWN:
            nextRow = _snake.snake[_snake.indexHead].u8Row++;
            if (nextRow >= (NUM_ROWS - 1)) {
                nextRow = 0;
            }
            nextCol = _snake.snake[_snake.indexHead].u8Col;
            break;
    }

    if (_apple.u8Col == nextCol && _apple.u8Row == nextRow) {
        //ate an apple -> we need a new
        _apple.u8Col = rand() % NUM_COLS;
        _apple.u8Row = rand() % (NUM_ROWS - 1);
    } else {
        //still hungry -> remove tail
        if (!(_apple.u8Col == _snake.snake[_snake.indexTail].u8Col &&
                _apple.u8Row == _snake.snake[_snake.indexTail].u8Row)) {
            //only remove the tail if there is no apple in the same place
            vSetInPattern(_snake.snake[_snake.indexTail].u8Col,
                    _snake.snake[_snake.indexTail].u8Row, 0);
        }
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

    index = _snake.indexTail;
    do {
        if ((_snake.snake[_snake.indexHead].u8Col ==
                _snake.snake[index].u8Col) &&
                (_snake.snake[_snake.indexHead].u8Row ==
                _snake.snake[index].u8Row)) {
            //we hit the snakes body -> the game is done
            return TRUE;
        }
        index++;
        if (index >= MAX_SNAKE_LEN) {
            index = 0;
        }
    } while (index != _snake.indexHead);

    return FALSE;
}
