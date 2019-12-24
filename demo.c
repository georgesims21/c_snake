#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define DELAY 60000
#define HALF_SCREEN_SIZE 100
#define RIGHT 1
#define LEFT -1

int max_y, max_x;

typedef struct Object {
    const char *shape;
    int x, y,
        next_x,
        next_y,
        y_direction,
        x_direction;
} Object;

int main (int argc, char *argv[]){

    getmaxyx(stdscr, max_y, max_x);
    Object circle = {.shape = "o", .x = 0, .y = 0, .next_x = 0, 
                    .next_y = 0, .y_direction = RIGHT, .x_direction = RIGHT};

    Object cap_circle = {.shape = "O", .x = HALF_SCREEN_SIZE, .y = 0, .next_x = 0, 
                        .next_y = 0, .y_direction = LEFT, .x_direction = LEFT};

    initscr();
    raw();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(FALSE);
    int ch = 0;

    for (;;) {
        getmaxyx(stdscr, max_y, max_x);
        clear();
        mvprintw(circle.y, circle.x, circle.shape);
        refresh();
        usleep(DELAY);

        ch = getch();
        if(ch == KEY_UP) {
            circle.y -= circle.y_direction;
        } else if (ch == KEY_RIGHT) {
            circle.x += circle.x_direction;
        } else if (ch == KEY_DOWN) {
            circle.y += circle.y_direction;
        } else if(ch == KEY_LEFT) {
            circle.x -= circle.x_direction;
        } else if (ch == KEY_BACKSPACE){
            exit(0);
        }
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
