#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <curses.h> // library for windows
#else
    #include <ncurses.h>
#endif

#include <panel.h>
#include <menu.h>
#include <signal.h>
#include "miniaudio.h"

ma_engine engine;

volatile sig_atomic_t need_resize = 0;  //detect window resize
void handle_winch(int sig) {
    need_resize = 1;
}

void operation(int *hours, int *minutes, int *seconds);   //testing if we use default timer or a edited one
void timer(int *hours, int *minutes, int *seconds);

int main() {
    initscr();

    int hours = 0, minutes = 50, seconds = 0;   //default values(I work the best with these values)

    operation(&hours, &minutes, &seconds);
    timer(&hours, &minutes, &seconds);
    
    getch();
    endwin();
    
    if (hours == 0 && minutes == 0 && seconds < 1) {  //timer stop when seconds reach -1, also there is a case when timer can be stopped at 1sec to display 0 0 0 remaining time, this is why I test seconds < 1 here
        printf("Timer complete!\n");
    } else {
        printf("Timer stopped! Remaining time: %dh %dm %ds\n", hours, minutes, seconds);
    }
    
    return 0;
}

void operation(int *hours, int *minutes, int *seconds) {
    int command = 0;
    
    printw("Press 'e' to configure the timer or 'ENTER' to start default timer(50min): ");
    command = getch();

    if ((char)command == 'e' || (char)command == 'E') {
        printw("\nAdd custom time(hours minutes seconds): ");
        scanw("%d %d %d", hours, minutes, seconds);
        if (*hours < 0 || *minutes < 0 || *seconds < 0) {
            endwin();
            fprintf(stderr, "Invalid values! (numbers must be >= 0)\n");
            exit(EXIT_FAILURE);
        }
    } else if ((char)command != '\n'){
        endwin();
        fprintf(stderr, "ERROR! Wrong key!\n");
        exit(EXIT_FAILURE);
    }

    ma_engine_uninit(&engine);
    clear();
    refresh();
}

void timer(int *hours, int *minutes, int *seconds) {
    if (ma_engine_init(NULL, &engine)) {
        printf("Failed to init sound engine!");
    }

    noecho(); 
    curs_set(0);
    WINDOW *clock_window;
    clock_window = newwin(3, 10, (LINES - 3) / 2, (COLS - 10) / 2);
    wborder(clock_window, 0, 0, 0, 0, 0, 0, 0, 0);

    signal(SIGWINCH, handle_winch);
    nodelay(stdscr, TRUE);  //nodelay to be able to get 'q' or 'Q' keys when wanting to exit the timer
    while (*hours >= 0 && *minutes >= 0 && *seconds >= 0) {
        if (need_resize != 0) {
            need_resize = 0;
            endwin();
            refresh();
            clear();
            // Move window to a new position
            wclear(clock_window);
            wrefresh(clock_window);
            mvwin(clock_window, (LINES - 3) / 2, (COLS - 10) / 2);
            wborder(clock_window, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        int ch = getch();
        if ((char)ch == 'q' || (char)ch == 'Q') {
            break;
        } else if ((char)ch == 'p' || (char)ch == 'P') {
            nodelay(stdscr, FALSE);
            wclear(clock_window);
            mvwprintw(clock_window, 1, 1, "PAUSE...");
            wrefresh(clock_window);
            getch();
            nodelay(stdscr, TRUE);
            wclear(clock_window);
            wborder(clock_window, 0, 0, 0, 0, 0, 0, 0, 0);
            wrefresh(clock_window);
        }
        if (*seconds == 0 && *minutes > 0) {
            *seconds = 59;
            (*minutes)--;
            if (*minutes == 0 && *hours > 0) {
                *minutes = 59;
                (*hours)--;
            }
        }
        mvwprintw(clock_window, 1, 1, "%02d:%02d:%02d", *hours, *minutes, *seconds);
        wrefresh(clock_window);
        napms(1000);
        (*seconds)--;
    }
    nodelay(stdscr, FALSE); 

    wclear(clock_window);
    wrefresh(clock_window);
    mvwprintw(clock_window, 1, 1, "DONE!");
    mvprintw(LINES - 1, 0, "Press any key to exit...");     //write at the bottom of window
    wrefresh(clock_window);
    ma_engine_play_sound(&engine, "51169__rutgermuller__telephone-placed-in-dock-beep.wav", NULL);   //add your path to a finish sound
    delwin(clock_window);
    curs_set(1);
    refresh();
    echo();
}