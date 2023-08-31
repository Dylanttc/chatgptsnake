#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define SNAKE_LENGTH 5
#define FOOD_SYMBOL 'F'
#define SNAKE_SYMBOL 'S'
#define EMPTY ' '

typedef struct {
    int x, y;
} Point;

Point snake[SNAKE_LENGTH];
Point food;
int direction = KEY_RIGHT;

void initialize() {
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(100);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
}

void draw() {
    clear();
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        mvaddch(snake[i].y, snake[i].x, SNAKE_SYMBOL | COLOR_PAIR(1));
    }
    mvaddch(food.y, food.x, FOOD_SYMBOL | COLOR_PAIR(2));
    refresh();
}

void moveSnake() {
    for (int i = SNAKE_LENGTH - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    switch (direction) {
        case KEY_RIGHT:
            snake[0].x++;
            break;
        case KEY_LEFT:
            snake[0].x--;
            break;
        case KEY_UP:
            snake[0].y--;
            break;
        case KEY_DOWN:
            snake[0].y++;
            break;
    }
}

int collision() {
    if (snake[0].x == food.x && snake[0].y == food.y) {
        // For simplicity, we just exit when the snake eats the food.
        endwin();
        printf("You ate the food!\n");
        exit(0);
    }
    if (snake[0].x < 0 || snake[0].x >= COLS || snake[0].y < 0 || snake[0].y >= LINES) {
        return 1;
    }
    return 0;
}

int main() {
    initialize();

    // Initialize snake starting position
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake[i].x = 10 + i;
        snake[i].y = 10;
    }

    // Place food
    food.x = 20;
    food.y = 10;

    while (1) {
        int ch = getch();
        switch (ch) {
            case KEY_RIGHT:
            case KEY_LEFT:
            case KEY_UP:
            case KEY_DOWN:
                direction = ch;
                break;
            case 'q':
                endwin();
                return 0;
        }

        moveSnake();
        if (collision()) {
            break;
        }
        draw();
        usleep(100000);
    }

    endwin();
    printf("Game over!\n");
    return 0;
}
