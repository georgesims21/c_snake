/*
 *TODO:
 * Head cannot touch the body x's or y's
 * If going left can't go right, if going right can't go left... etc
 * Make all ints a percentage of the max screen size instead
 * Why doesn't the head register on certain food/borders?
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
void random_food_generator(Food *food);
void init_head(Snake *snake);
void init_body(Snake *snake);
void print_border(void);
void game_loop(int delay);
void menu_screen(void);
int game_over(void);

int max_x, max_y;

int main (int argc, char *argv[]){

    int ch = 0, max_x_tmp = 0, max_y_tmp = 0;
    WINDOW *win = init_screen();
    clear();

    for(;;){
        getmaxyx(stdscr, max_y_tmp, max_x_tmp);
        // Good box:screen ratio
        max_x = max_x_tmp * 0.7; max_y = max_y_tmp * 0.7;
        menu_screen();
        ch = getch();
        if(ch == KEY_UP) {
            // EASY
            game_loop(120000);
        } else if (ch == KEY_RIGHT) {
            // MED
            game_loop(90000);
        } else if (ch == KEY_DOWN) {
            // HARD
            game_loop(30000);
        } else if (ch == KEY_BACKSPACE) {
            // EXIT
            clear();
            endwin();
            exit(0);
        }
    }

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
        snake[i].prev_x = 0;
        snake[i].prev_y = 0;
        snake[i].x_direction = 3;
        snake[i].y_direction = 3;
    }
}

void init_head(Snake *snake){

    snake[0].shape = "O";
    // Avoid starting on border
    snake[0].x = 2;
    snake[0].y = 2;
    snake[0].next_x = 0;
    snake[0].next_y = 0;
    snake[0].prev_x = 0;
    snake[0].prev_y = 0;
    snake[0].x_direction = RIGHT;
    snake[0].y_direction = RIGHT;
}

void random_food_generator(Food *food) {

    // Avoid printing food on borders
    food->x = ((rand() % (max_x - 3)) + 2);
    food->y = ((rand() % (max_y - 3)) + 2);
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
        if(*last_direction != DOWN) {
            snake[0].y -= snake[0].y_direction;
            *last_direction = UP;
        }
    } else if (ch == KEY_RIGHT) {
        if(*last_direction != LEFT) {
            snake[0].x += snake[0].x_direction;
            *last_direction = RIGHT;
        }
    } else if (ch == KEY_DOWN) {
        if(*last_direction != UP) {
            snake[0].y += snake[0].y_direction;
            *last_direction = DOWN;
        }
    } else if(ch == KEY_LEFT) {
        if(*last_direction != RIGHT) {
            snake[0].x -= snake[0].x_direction;
            *last_direction = LEFT;
        }
    } else if (ch == KEY_BACKSPACE){
        endwin();
        exit(0);
    } else {
        // No key press
        auto_move(last_direction, total, snake);
    }
}

void game_loop(int delay) {

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
            random_food_generator(&food);
            food_taken = 0;
        }
        // Print snake
        for(int i = 0; i <= total; i++) {
            if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                /*if(game_over() == 0) {*/
                    /*return;*/
                /*} else {*/
                    /*exit(0);*/
                /*}*/
            }
            mvprintw(snake[i].y, snake[i].x, snake[i].shape);
        }
        mvprintw(food.y, food.x, food.shape);
        /*mvprintw(0, (max_x / 4), "Total: %d", total);*/
        mvprintw(0, (max_x / 4), "max_x: %d\tmax_y: %d", max_x, max_y);

        if(snake[0].y == food.y && snake[0].x == food.x) {
            food_taken = 1;
            total++;
        } else if (snake[0].y == 1 || snake[0].y == max_y  || \
                    snake[0].x == 1 || snake[0].x == max_x) {
            
            if(game_over() == 0) {
                return;
            } else {
                exit(0);
            }

        }
        
        refresh();
        usleep(delay);
        // Handle movements
        ch = getch();
        change_direction(ch, &last_direction, total, snake);
    }
}

int game_over(void) {

    int ch;
    clear();
    print_border();
    mvprintw((max_y * 0.35), (max_x / 2), "GAME OVER");
    mvprintw((max_y * 0.35) + 1, (max_x/2), "Press backspace to restart");
    for(;;) {
        ch = getch();
        if(ch == KEY_BACKSPACE) {
            return 0;
        }
    }
}

void print_border(void) {

        mvprintw(1, 1, "+");
        mvprintw(max_y, 1, "+");
        mvprintw(1, max_x, "+");
        mvprintw(max_y, max_x, "+");
        for(int i = 2; i <= (max_y - 1); i++) {
            mvprintw(i, 1, "|");
            mvprintw(i, max_x, "|");
        }
        for(int i = 2; i <= (max_x - 1); i++) {
            mvprintw(1, i, "-");
            mvprintw(max_y, i, "-");
        }
}

void menu_screen(void) {

    print_border();
    mvprintw((max_y * 0.35), (max_x * 0.25), "Welcome to snake!");
    mvprintw((max_y * 0.35) + 1, (max_x * 0.25), "Press the up arrow for EASY");
    mvprintw((max_y * 0.35) + 2, (max_x * 0.25), "Press the right arrow for MEDIUM");
    mvprintw((max_y * 0.35) + 3, (max_x * 0.25), "Press the down arrow for HARD");
    mvprintw((max_y * 0.35) + 4, (max_x * 0.25), "Press backspace to exit");
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
