/*
 *TODO:
 * Head cannot touch the body x's or y's
 * If going left can't go right, if going right can't go left... etc
 * Make all ints a percentage of the max screen size instead
 * Why doesn't the head register on certain food/borders?
 * Make main loop into method, then make menu choosing difficulty at start then run loop with DELAY as parameter
 */

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

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
        prev_x,
        prev_y,
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
void print_border(void);
void game_loop(int delay, int max_x, int max_y);

int main (int argc, char *argv[]){

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    WINDOW *win = init_screen();

    for(;;){
    
        game_loop(90000, max_x, max_y);
    }

    endwin();

    return 0;
}

void print_border(void) {

        mvprintw(1, 1, "+");
        mvprintw(49, 1, "+");
        mvprintw(1, 99, "+");
        mvprintw(49, 99, "+");
        for(int i = 2; i <= 48; i++) {
            mvprintw(i, 1, "|");
            mvprintw(i, 99, "|");
        }
        for(int i = 2; i <= 98; i++) {
            mvprintw(1, i, "-");
            mvprintw(49, i, "-");
        }
}

void init_body(Snake *snake) {

    for(int i = 1; i < MAX_SNAKE; i++) {
        snake[i].shape = "o";
        snake[i].x = 0;
        snake[i].y = 0;
        snake[i].next_x = 0;
        snake[i].next_y = 0;
        snake[i].prev_x = 0;
        snake[i].prev_y = 0;
        snake[i].x_direction = 3;
        snake[i].y_direction = 3;
    }
}

void init_head(Snake *snake){

    snake[0].shape = "O";
    snake[0].x = 2;
    snake[0].y = 2;
    snake[0].next_x = 0;
    snake[0].next_y = 0;
    snake[0].prev_x = 0;
    snake[0].prev_y = 0;
    snake[0].x_direction = RIGHT;
    snake[0].y_direction = RIGHT;
}

void random_food_generator(Food *food, int max_x, int max_y) {

        food->x = ((rand() % 95) + 2);
        food->y = ((rand() % 42) + 2);
}

void move_body(int total, Snake *snake) {

    snake[1].prev_x = snake[1].x;
    snake[1].prev_y = snake[1].y;
    snake[1].x = snake[0].x;
    snake[1].y = snake[0].y;
    for(int i = 2; i <= total; i++) {

        snake[i].prev_x = snake[i].x;
        snake[i].prev_y = snake[i].y;
        snake[i].x = snake[i-1].prev_x;
        snake[i].y = snake[i-1].prev_y;
    }
}

void auto_move(int *last_direction, int total, Snake *snake) {

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

void game_loop(int delay, int max_x, int max_y) {

    int ch = 0, last_direction = RIGHT, 
            food_taken = 1, total = 0;
    time_t t;
    Snake snake[MAX_SNAKE];
    init_head(snake);
    init_body(snake);
    Food food = {.shape = "x", .x = 0, .y = 0};
    
    for (;;) {
        // Print snake snake[0] and refresh screen
        srand((unsigned) time(&t));
        getmaxyx(stdscr, max_y, max_x);
        clear();
        print_border();
        if(food_taken) {
            random_food_generator(&food, max_x, max_y);
            food_taken = 0;
        }
        // Print snake
        for(int i = 0; i <= total; i++) {
            mvprintw(snake[i].y, snake[i].x, snake[i].shape);
        }
        mvprintw(food.y, food.x, food.shape);
        mvprintw(0, 46, "Total: %d", total);

        if(snake[0].y == food.y && snake[0].x == food.x) {
            food_taken = 1;
            total++;
        } else if (snake[0].y == 1 || snake[0].y == 49  || \
                    snake[0].x == 1 || snake[0].x == 98) {

            clear();
            print_border();
            mvprintw(25, 40, "GAME OVER");
            mvprintw(26, 40, "Press backspace to restart");
            for(;;) {
                ch = getch();
                if(ch == KEY_BACKSPACE) {
                    return;
                }
            }
        }
        
        refresh();
        usleep(DELAY);
        // Handle movements
        ch = getch();
        change_direction(ch, &last_direction, total, snake);
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
