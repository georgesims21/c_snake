/*
 *TODO:
 *    * Randomly create food inside box
 *    * Print a longer snake depending on direction and size of the snake
 */

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define DELAY 60000
#define HALF_SCREEN_SIZE 100
#define RIGHT 1
#define LEFT -1
#define UP 0
#define DOWN 2

int max_y, max_x;

typedef struct Object {
    const char *shape;
    int x, y,
        next_x,
        next_y,
        y_direction,
        x_direction;
} Object;

/*typedef struct Food {*/
    /*const char *shape = 'o';*/
    /*int x, y;*/
/*} Food;*/

void auto_move(int *last_direction, Object *circle);
void change_direction(int ch, int *last_direction, Object *circle);
WINDOW *init_screen(void);

int main (int argc, char *argv[]){

    int ch = 0, last_direction = RIGHT;
    getmaxyx(stdscr, max_y, max_x);
    Object circle = {.shape = "O", .x = 0, .y = 0, .next_x = 0, 
                    .next_y = 0, .y_direction = RIGHT, .x_direction = RIGHT};
    WINDOW *win = init_screen();

    for (;;) {
        // Print snake head and refresh screen
        getmaxyx(stdscr, max_y, max_x);
        clear();
        mvprintw(circle.y, circle.x, circle.shape);
        refresh();
        usleep(DELAY);

        // Handle movements
        ch = getch();
        change_direction(ch, &last_direction, &circle);

    }


    //while(1){

        // Get screen size each iteration
     //   getmaxyx(stdscr, max_y, max_x);
        // Clear current item(s) on screen
      //  clear();
       // if(ch = getch() == KEY_UP) {
        //    mvprintw(circle.y, circle.x, circle.shape);
       // }
        
        //refresh();

        //mvprintw(cap_circle.y, cap_circle.x, cap_circle.shape);
        //refresh();

        //usleep(DELAY);
        //circle.next_x = circle.x + circle.x_direction;
        //cap_circle.next_x = cap_circle.x + cap_circle.x_direction;

        //if(circle.next_x >= max_x || circle.next_x < 0) {
        //    circle.x_direction *= -1;
        //} else {
            //circle.x += circle.x_direction;
        //}

        //if(cap_circle.next_x >= max_x || cap_circle.next_x < 0) {
            //cap_circle.x_direction *= -1;
        //} else {
            //cap_circle.x += cap_circle.x_direction;
        //}
   // }

    endwin();

    return 0;
}

void auto_move(int *last_direction, Object *circle) {

    if(*last_direction == UP) {
        circle->y -= circle->y_direction;
    } else if (*last_direction == RIGHT) {
        // Double to match speed of y axis
        circle->x += 2*(circle->x_direction);
    } else if (*last_direction == DOWN) {
        circle->y += circle->y_direction;
    } else if (*last_direction == LEFT) {
        circle->x -= 2*(circle->x_direction);
    }
}

void change_direction(int ch, int *last_direction, Object *circle) {

    if(ch == KEY_UP) {
        circle->y -= circle->y_direction;
        *last_direction = UP;
    } else if (ch == KEY_RIGHT) {
        circle->x += circle->x_direction;
        *last_direction = RIGHT;
    } else if (ch == KEY_DOWN) {
        circle->y += circle->y_direction;
        *last_direction = DOWN;
    } else if(ch == KEY_LEFT) {
        circle->x -= circle->x_direction;
        *last_direction = LEFT;
    } else if (ch == KEY_BACKSPACE){
        exit(0);
    } else {
        // No key press
        auto_move(last_direction, circle);
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
