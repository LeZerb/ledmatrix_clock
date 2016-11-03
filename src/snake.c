#include "common.h"
#include "display.h"
#include <stdlib.h>

#define MAX_SNAKE 90

typedef enum {
    eDIR_UP = 0,
    eDIR_RIGHT = 1,
    eDIR_DOWN = 2,
    eDIR_LEFT = 3
} TE_DIRECTION;

typedef struct {
    U8 u8Col : 4;
    U8 u8Row : 4;
}
TS_SNAKE;

typedef struct {
    U8 u8Col;
    U8 u8Row;
} TS_APPLE;

static U8 _u8SnakeLen = 0;
static TS_SNAKE _astSnake[MAX_SNAKE] = {0};
static TE_DIRECTION _eDir;
static TS_APPLE _stApple;

void snake_run(void) {
    if (_u8SnakeLen == 0) {
        //create the initial snake

        //initialize random generator
        srand(12);

        //clear the pattern
        vClearPattern();

        //snake's initial tail is at here
        _astSnake[1].u8Col = 4;
        _astSnake[1].u8Row = 5;

        vSetInPattern(4, 5, 1);

        //index 0 will always be the head
        _astSnake[0].u8Col = 5;
        _astSnake[0].u8Row = 5;

        vSetInPattern(5, 5, 1);

        _u8SnakeLen = 2;
        _eDir = eDIR_RIGHT;

        //create an apple
        _stApple.u8Col = rand() % NUM_COLS;
        _stApple.u8Row = rand() % (NUM_ROWS - 1);
    } else {
        U8 u8NextCol = _astSnake[0].u8Col,
                u8NextRow = _astSnake[0].u8Row,
                u8AddToTail = 0,
                u8CurPart = _u8SnakeLen - 1;

        switch (_eDir) {
            case eDIR_LEFT:
            {
                if (u8NextCol == 0) {
                    u8NextCol = NUM_COLS - 1;
                } else {
                    u8NextCol--;
                }
            }
                break;

            case eDIR_RIGHT:
            {
                if (u8NextCol == NUM_COLS - 1) {
                    u8NextCol = 0;
                } else {
                    u8NextCol++;
                }
            }
                break;

            case eDIR_UP:
            {
                if (u8NextRow == 0) {
                    u8NextRow = NUM_ROWS - 2;
                } else {
                    u8NextRow--;
                }
            }
                break;

            case eDIR_DOWN:
            {
                if (u8NextRow == NUM_ROWS - 2) {
                    u8NextRow = 0;
                } else {
                    u8NextRow++;
                }
            }
                break;
        }

        if (_stApple.u8Col == u8NextCol &&
                _stApple.u8Row == u8NextRow) {
            //we have found an apple - don't remove the tail
            u8AddToTail = 1;
        }

        if (u8AddToTail && (_u8SnakeLen <= MAX_SNAKE)) {
            //add an item to the tail
            _astSnake[u8CurPart + 1] = _astSnake[u8CurPart];
            _u8SnakeLen++;
        } else {
            //remove the tail item from display
            vSetInPattern(_astSnake[u8CurPart].u8Col, _astSnake[u8CurPart].u8Row, 0);
        }

        while (u8CurPart--) {
            if (u8CurPart == 0) {
                _astSnake[0].u8Col = u8NextCol;
                _astSnake[0].u8Row = u8NextRow;

                vSetInPattern(u8NextCol, u8NextRow, 1);
            } else {
                _astSnake[u8CurPart] = _astSnake[u8CurPart - 1];

                if ((u8NextCol == _astSnake[u8CurPart].u8Col) &&
                        (u8NextRow == _astSnake[u8CurPart].u8Row)) {
                    //we have hit the snake's body
                    _u8SnakeLen = 0;
                    return;
                }
            }
        }
    }
}

void mmain(void) {
    snake_run();
}
