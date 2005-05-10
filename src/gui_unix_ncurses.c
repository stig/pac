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
static void draw_pac_jaw(const struct env *board, const struct creature *pac);

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

/* Draw a creature, that is: either pac or a ghost. */
void draw_creature(const struct env* board, const struct creature *ct)
{
        int i, j, r, c, k = 1;

	r = board->rows;
	c = board->cols;

        attron(COLOR_PAIR(ct->colour));
        for (i=-k; i<=k; i++) {
		for (j=-k; j<=k; j++) {
			mvaddch((r + get_row(ct) + i) % r, 
					(c + get_col(ct) + j) % c, 
					ct->looks[i+k][j+k]);
		}
        }

	/* only pac has mouth */
	if (ct->is_pac) 
		draw_pac_jaw(board, ct);

        attroff(COLOR_PAIR(ct->colour));
}

/* draw pac's mouth */
static void draw_pac_jaw(const struct env *board, const struct creature *pac)
{ 
	static int i;
	int row, col;
	char *p = NULL;
	char *up =	"^^||^";
	char *down =	"VV||V";
	char *right =	"{<-=<";
	char *left =	"}>-=>";

	row = get_row(pac);
	col = get_col(pac);
	switch (pac->dir) {
		case DOWN:
			row = (board->rows + row + 1) % board->rows; 
			p = up;
			break;
		case UP:
			row = (board->rows + row - 1) % board->rows; 
			p = down;
			break;
		case LEFT:
			col = (board->cols + col - 1) % board->cols; 
			p = left;
			break;
		case RIGHT:
			col = (board->cols + col + 1) % board->cols; 
			p = right;
			break;
		default:
			/* do nothing, just return */
			return;
	}
	attron(COLOR_PAIR(pac->colour));
	mvaddch(row, col, (int)p[i]);
	attroff(COLOR_PAIR(pac->colour));

	i = ++i % 5;
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

enum dir_t get_user_input(void)
{
        int c;
        enum dir_t retval = NO_INPUT;

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
#if HAVE_ATEXIT
        atexit(&endwin);  /* man atexit */
#endif
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

/* Blank the screen. This is usefull when pac 'die' and the
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

