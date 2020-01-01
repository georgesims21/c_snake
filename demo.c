/*
 *TODO:
 *    * Print a longer snake depending on direction and size of the snake
 *      - Each node must follow the next node, like a linked list. Copies node infront's direction and next x and y.
 */

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define DELAY 60000
#define MAX_SNAKE 100
#define RIGHT 1
#define LEFT -1
#define UP 0
#define DOWN 2

typedef struct Snake {
    const char *shape;
    int x, y,
        next_x,
        next_y,
        y_direction,
        x_direction;
} Snake;

typedef struct Food {
    const char *shape;
    int x, y;
} Food;

void auto_move(int *last_direction, int total, Snake *snake);
void change_direction(int ch, int *last_direction, int total, Snake *snake);
void move_body(int total, Snake *snake);
WINDOW *init_screen(void);
void random_food_generator(Food *food, int max_x, int max_y);
void init_head(Snake *snake);
void init_body(Snake *snake);

int main (int argc, char *argv[]){

    int ch = 0, last_direction = RIGHT, max_y = 0, max_x = 0, food_taken = 1, total = 0;
    time_t t;
    Snake snake[MAX_SNAKE];
    init_head(snake);
    init_body(snake);

    getmaxyx(stdscr, max_y, max_x);

    Food food = {.shape = "x", .x = 0, .y = 0};

    WINDOW *win = init_screen();

    for (;;) {
        // Print snake snake[0] and refresh screen
        srand((unsigned) time(&t));
        getmaxyx(stdscr, max_y, max_x);
        clear();
        if(food_taken) {
            random_food_generator(&food, max_x, max_y);
            food_taken = 0;
        }
        mvprintw(snake[0].y, snake[0].x, snake[0].shape);
        mvprintw(food.y, food.x, food.shape);
        mvprintw(0, 100, "Total: %d", total);

        // Print snake
        if(total > 0) {
            for(int i = 1; i <= total; i++) {
                mvprintw(snake[i].y, snake[i].x, snake[i].shape);
            }
        }

        if(snake[0].y == food.y && snake[0].x == food.x) {
            food_taken = 1;
            total++;
        }
        refresh();
        usleep(DELAY);

        // Handle movements
        ch = getch();
        change_direction(ch, &last_direction, total, snake);
    }
        //if(snake[0].next_x >= max_x || snake[0].next_x < 0) {
    endwin();

    return 0;
}

void init_body(Snake *snake) {

    for(int i = 1; i < MAX_SNAKE; i++) {
        snake[i].shape = "o";
        snake[i].x = 0;
        snake[i].y = 0;
        snake[i].next_x = 0;
        snake[i].next_y = 0;
        snake[i].x_direction = 3;
        snake[i].y_direction = 3;
    }
}

void init_head(Snake *snake){

    snake[0].shape = "O";
    snake[0].x = 0;
    snake[0].y = 0;
    snake[0].next_x = 0;
    snake[0].next_y = 0;
    snake[0].x_direction = RIGHT;
    snake[0].y_direction = RIGHT;
}

void random_food_generator(Food *food, int max_x, int max_y) {

        food->x = (rand() % max_x);
        food->y = (rand() % max_y);
}

void move_body(int total, Snake *snake) {

    for(int i = 1; i <= total; i++) {

        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }
}

void auto_move(int *last_direction, int total, Snake *snake) {

    move_body(total, snake);

    if(*last_direction == UP) {
        snake[0].y -= snake[0].y_direction;
    } else if (*last_direction == RIGHT) {
        // Double to match speed of y axis
        snake[0].x += 2*(snake[0].x_direction);
    } else if (*last_direction == DOWN) {
        snake[0].y += snake[0].y_direction;
    } else if (*last_direction == LEFT) {
        snake[0].x -= 2*(snake[0].x_direction);
    }

}

void change_direction(int ch, int *last_direction, int total, Snake *snake) {

    move_body(total, snake);

    if(ch == KEY_UP) {
        snake[0].y -= snake[0].y_direction;
        *last_direction = UP;
    } else if (ch == KEY_RIGHT) {
        snake[0].x += snake[0].x_direction;
        *last_direction = RIGHT;
    } else if (ch == KEY_DOWN) {
        snake[0].y += snake[0].y_direction;
        *last_direction = DOWN;
    } else if(ch == KEY_LEFT) {
        snake[0].x -= snake[0].x_direction;
        *last_direction = LEFT;
    } else if (ch == KEY_BACKSPACE){
        endwin();
        exit(0);
    } else {
        // No key press
        auto_move(last_direction, total, snake);
    }
}

WINDOW *init_screen(void) {

    WINDOW *win = initscr();
    raw();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(FALSE);
    wrefresh(win);

    return win;
}
