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

void auto_move(int *last_direction, Snake *head);
void change_direction(int ch, int *last_direction, Snake *head);
WINDOW *init_screen(void);
void random_food_generator(Food *food, int max_x, int max_y);

int main (int argc, char *argv[]){

    int ch = 0, last_direction = RIGHT, max_y = 0, max_x = 0, food_taken = 0;
    time_t t;
    Snake body[MAX_SNAKE];
    getmaxyx(stdscr, max_y, max_x);

    Snake head = {.shape = "O", .x = 0, .y = 0, .next_x = 0, 
                    .next_y = 0, .y_direction = RIGHT, .x_direction = RIGHT};
    Food food = {.shape = "o", .x = 0, .y = 0};

    WINDOW *win = init_screen();

    for (;;) {
        // Print snake head and refresh screen
        srand((unsigned) time(&t));
        getmaxyx(stdscr, max_y, max_x);
        clear();
        if(food_taken) {
            random_food_generator(&food, max_x, max_y);
            food_taken = 0;
        }
        mvprintw(head.y, head.x, head.shape);
        mvprintw(food.y, food.x, food.shape);
        if(head.y == food.y && head.x == food.x) {
            food_taken = 1;
        }
        refresh();
        usleep(DELAY);

        // Handle movements
        ch = getch();
        change_direction(ch, &last_direction, &head);
    }


    //while(1){

        // Get screen size each iteration
     //   getmaxyx(stdscr, max_y, max_x);
        // Clear current item(s) on screen
      //  clear();
       // if(ch = getch() == KEY_UP) {
        //    mvprintw(head.y, head.x, head.shape);
       // }
        
        //refresh();

        //mvprintw(cap_head.y, cap_head.x, cap_head.shape);
        //refresh();

        //usleep(DELAY);
        //head.next_x = head.x + head.x_direction;
        //cap_head.next_x = cap_head.x + cap_head.x_direction;

        //if(head.next_x >= max_x || head.next_x < 0) {
        //    head.x_direction *= -1;
        //} else {
            //head.x += head.x_direction;
        //}

        //if(cap_head.next_x >= max_x || cap_head.next_x < 0) {
            //cap_head.x_direction *= -1;
        //} else {
            //cap_head.x += cap_head.x_direction;
        //}
   // }

    endwin();

    return 0;
}

void random_food_generator(Food *food, int max_x, int max_y) {

        food->x = (rand() % max_x);
        food->y = (rand() % max_y);
}

void auto_move(int *last_direction, Snake *head) {

    if(*last_direction == UP) {
        head->y -= head->y_direction;
    } else if (*last_direction == RIGHT) {
        // Double to match speed of y axis
        head->x += 2*(head->x_direction);
    } else if (*last_direction == DOWN) {
        head->y += head->y_direction;
    } else if (*last_direction == LEFT) {
        head->x -= 2*(head->x_direction);
    }
}

void change_direction(int ch, int *last_direction, Snake *head) {

    if(ch == KEY_UP) {
        head->y -= head->y_direction;
        *last_direction = UP;
    } else if (ch == KEY_RIGHT) {
        head->x += head->x_direction;
        *last_direction = RIGHT;
    } else if (ch == KEY_DOWN) {
        head->y += head->y_direction;
        *last_direction = DOWN;
    } else if(ch == KEY_LEFT) {
        head->x -= head->x_direction;
        *last_direction = LEFT;
    } else if (ch == KEY_BACKSPACE){
        exit(0);
    } else {
        // No key press
        auto_move(last_direction, head);
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
