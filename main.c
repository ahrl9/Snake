#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define WIDTH 40
#define HEIGHT 25

#define EMPTY ' '
#define SNAKE_HEAD '#'
#define SNAKE_BODY 'O'
#define APPLE '*'

unsigned char screen[HEIGHT][WIDTH];

typedef struct {
    unsigned char x;
    unsigned char y;
} pos_t;

const pos_t nullPos = {0xff, 0xff};

typedef enum {
    GE_GAME_OVER,
    GE_ATE_APPLE,
    GE_ONLY_MOVED,
    GE_NOT_MOVED
} gameEvent_t;

pos_t snakeHead;
pos_t snakeBody[WIDTH * HEIGHT];
int snakeBodyStart = 0;
int snakeBodyEnd = 0;
pos_t apple;



pos_t randPos (void) {
    pos_t pos;

    pos.x = rand() % WIDTH;
    pos.y = rand() % HEIGHT;

    return pos;
}

gameEvent_t onInput(char input) {
    switch (input) {
        case 'a':
            --snakeHead.x;
            break;
        case 'd':
            ++snakeHead.x;
            break;
        case 'w':
            --snakeHead.y;
            break;
        case 's':
            ++snakeHead.y;
            break;
        default:
            return GE_NOT_MOVED;
    }

    // unsigned char Max = 255
    if (snakeHead.x == 255) snakeHead.x = WIDTH - 1;
    if (snakeHead.x == WIDTH) snakeHead.x = 0;
    if (snakeHead.y == 255) snakeHead.y = HEIGHT - 1;
    if (snakeHead.y == HEIGHT) snakeHead.y = 0;

    unsigned char charAtPos = screen[snakeHead.y][snakeHead.x];
    if (charAtPos != EMPTY) {
        if (charAtPos == APPLE) {
            return GE_ATE_APPLE;
        } else {
            return GE_GAME_OVER;
        }
    } else return GE_ONLY_MOVED;
}

void moveSnakeBodyStart(pos_t oldHeadPos) {
    screen[snakeHead.y][snakeHead.x] = SNAKE_HEAD;
    if (snakeBodyStart == snakeBodyEnd) screen[oldHeadPos.y][snakeHead.x] = EMPTY;
    else {
        if (++snakeBodyStart > (sizeof snakeBody / sizeof(pos_t))) snakeBodyStart = 0;
        snakeBody[snakeBodyStart] = oldHeadPos;
        screen[oldHeadPos.y][oldHeadPos.x] = SNAKE_BODY;
    }
}

void moveSnakeBodyEnd(void) {
    screen[snakeBody[snakeBodyEnd].y][snakeBody[snakeBodyEnd].x] = EMPTY;
    snakeBody[snakeBodyEnd] = nullPos;
    if (++snakeBodyEnd > (sizeof snakeBody / sizeof(pos_t))) snakeBodyEnd = 0;
    screen[snakeBody[snakeBodyEnd].y][snakeBody[snakeBodyEnd].x] = SNAKE_BODY;
}

void generateApple(void) {
    pos_t pos = randPos();

    if (screen[pos.y][pos.x] == EMPTY) {
        apple = pos;
        screen[pos.y][pos.x] = APPLE;
        return;
    }

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            pos_t newPos;
            newPos.x = x + pos.x;
            newPos.y = y + pos.y;
            if (newPos.x > WIDTH) newPos.x -= WIDTH;
            if (newPos.y > HEIGHT) newPos.y -= HEIGHT;

            unsigned char charAtNewPos = screen[newPos.y][newPos.x];

            if (charAtNewPos == EMPTY) {
                apple = newPos;
                screen[newPos.y][newPos.x] = APPLE;
                return;
            }
        }
    }
}

void printScreen(void) {
    system("cls");

    char outputLine[WIDTH + 1];
    for (int y = 0; y < HEIGHT; ++y) {
        memcpy(outputLine, &screen[y][0], WIDTH);
        outputLine[WIDTH] = '\0';
        printf("%s\n\r", outputLine);
    }
}

void init(void) {
    for (unsigned char x = 0; x < WIDTH; ++x) {
        for (unsigned char y = 0; y < HEIGHT; ++y) {
            screen[y][x] = EMPTY;
        }
    }

    snakeHead.x = WIDTH / 2;
    snakeHead.y = HEIGHT / 2;
    screen[snakeHead.y][snakeHead.x] = SNAKE_HEAD;

    memset((void*) snakeBody, 0xff, sizeof snakeBody);

    generateApple();
}

int main(void) {
    init();
    printScreen();

    while (true) {
        unsigned char c;
        scanf("%c", &c);

        pos_t oldPos = snakeHead;

        switch (onInput(c)) {
            case GE_GAME_OVER:
                return 0;
            case GE_ATE_APPLE:
                moveSnakeBodyStart(oldPos);
                generateApple();
                printScreen();
                break;
            case GE_ONLY_MOVED:
                moveSnakeBodyStart(oldPos);
                moveSnakeBodyEnd();
                printScreen();
                break;
            case GE_NOT_MOVED:
                break;
        }
    }

    return 0;
}
