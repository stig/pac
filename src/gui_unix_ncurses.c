/* 
 * Pac - an ncurses wannabe pacman clone
 *
 * Copyright (C) 2002 Stig Brautaset
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
 */
#include <stdio.h>
#include <stdlib.h>     /* provides atexit() */
#include <string.h>     /* needed for strlen() */
#include <ncurses.h>    /* i/o routines */
#include "pac.h"
#include "creature_common.h"
#include "gui_unix_ncurses.h"
#include "score.h"


/* This is the input/output module. Functions that use calls from
 * the ncurses library to do its input/output are found in this
 * file.
 */

/* functions local to this module */
static void init_colours(void);
static void kill_ncurses(void);

/* 
 * Set up the with and breath with a call to the function giving
 * a zero-length string as its third argument and it will
 * remember the geometry between calls...
 */
void box_print(int rows, int cols, char *s)
{
        static int rw, cl;
        int i, len = strlen(s);

        if (*s == '\0') {
                rw = rows;
                cl = cols;
                return;
        }
        
        attron(COLOR_PAIR(9));
        for (i=-3; i<4; i++)
                mvprintw(rw/2+i, (cl-len)/2-6, "%*c", len+12, ' ');
        attroff(COLOR_PAIR(9));
        
        for (i=-2; i<3; i++)
                mvprintw(rw/2+i, (cl-len)/2-5, "%*c", len+10, ' ');
        
        mvprintw(rw/2, (cl-len)/2, "%s", s);
        mvprintw(LINES-1, COLS-1, " ");
}

void print_stat(struct env *board)
{
        attron(A_BOLD);
        mvprintw(board->rows, 0, "Total score: %-10lu ", get_score(board));
        attroff(A_BOLD);

        printw("Cherries left: %-10u", cherry_cnt(board));
        printw("Lives: %-10u", lives_left(board));
        
}

/* Draw a creature, that is: either pacman or a ghost. */
void draw_creature(const struct creature *ct)
{
        int i;
        attron(COLOR_PAIR(ct->colour));
        for (i=-1; i<2; i++) {
                mvprintw(get_row(ct) + i, get_col(ct) - 1, 
                                "%3.3s", ct->looks[i+1]);
        }
        attroff(COLOR_PAIR(ct->colour));
}

/* Draws on the screen a single location */
void draw_location(const struct env *base, int row, int col)
{
        if (base->pos[row][col] == NULL) {
                addch(' ');
                return;
        }
        switch (base->pos[row][col]->bg) {
        case '#':
                attron(COLOR_PAIR(8)); 
                mvaddch(row, col, '#'); 
                attroff(COLOR_PAIR(8)); 
                break;
        case 'c':
                attron(COLOR_PAIR(7) | A_BOLD); 
                mvaddch(row, col, 183); 
                attroff(COLOR_PAIR(7) | A_BOLD); 
                break;
        case 'C':
                attron(COLOR_PAIR(7) | A_BOLD); 
                mvaddch(row, col, '*');
                attroff(COLOR_PAIR(7) | A_BOLD); 
                break; 
        default:
                mvaddch(row, col, ' ');
                break;
        }
}

enum dir get_user_input(void)
{
        int c;
        enum dir retval = NO_INPUT;

        if ((c = getch()) && c != ERR)
                while (ERR != getch());

        switch(c) {
                case ERR:
                        retval = NO_INPUT;
                        break;
                case KEY_UP:
                        retval = UP;
                        break;
                case KEY_RIGHT:
                        retval = RIGHT;
                        break;
                case KEY_DOWN:
                        retval = DOWN;
                        break;
                case KEY_LEFT:
                        retval = LEFT;
                        break;
                case 'q':
                        retval = QUIT; 
                        break;
        }
        return retval;
}

int init_gui(int rows, int cols)
{
        int x, y;

        fputs("Starting ncurses gui subsystem.\n", stderr);
        initscr();           /* set up ncurses */
        atexit(&kill_ncurses);  /* man atexit */
        getmaxyx(stdscr, y, x);
        if (y < rows || x < cols) {
                fputs("\rBoard too large for the virtual terminal.\n", stderr);
                return 0;
        }

        start_color();
        if (!has_colors()) {
                fputs("\rCannot use or change colours.\n", stderr);
                return 0;
        }

        nonl();             
        noecho();            /* don't echo keys to screen */
        cbreak();            /* don't buffer until '\n' */
        nodelay(stdscr, TRUE);  /* make getch non-blocking */
        keypad(stdscr, TRUE);   /* represent function keys sanely */
        init_colours();         /* set up the colours I want */
	raw();

        /* initialise print_box() */
        box_print(rows, cols, "");

        return 1;
}

/* The other print routines does not actually print anything to
 * the screen, they just change the information that will be
 * printed when this function is called. */
void update_view(void)
{
        /* remove the cursor */
        move(LINES-1, COLS-1);
        addch(' ');

        /* make updates on screen */
        wnoutrefresh(stdscr);
        doupdate();
}

/* Blank the screen. This is usefull when pacman 'die' and the
 * creatures' positions are evaluated again, since this print some
 * output on stderr that mucks up the screen, but that is rectified by
 * this call. */
void reset_view(void)
{
        clear();
}

/* Sometimes we want to hang and wait for the user to input
 * something. */
int blocking_input(void)
{
        int ch;
        nodelay(stdscr, FALSE);
        ch = getch();
        nodelay(stdscr, TRUE);
        return ch;
}

/* this fuction is registered to run at normal exit of the
 * program. It returns the term's settings to normal. */
static void kill_ncurses(void)
{
        endwin();
        fputs("\nKilling ncurses windows.\n", stderr);
        puts("Pacman(R) 2002, Stig Brautaset.\n");
}

/* initialise colours for the ncurses routines to use */
static void init_colours(void)
{
        fprintf(stderr, "\rInitialising colour pairs.\n");

        init_pair(1, COLOR_YELLOW, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        init_pair(3, COLOR_BLUE, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
        init_pair(8, COLOR_CYAN, COLOR_CYAN);
        init_pair(9, COLOR_BLACK, COLOR_YELLOW);
}

