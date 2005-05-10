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
#include <stdlib.h>     /* provides EXIT_FAILURE etc */
#include <unistd.h>     /* needed for usleep() */
#include <time.h>       /* provides time() */
#include "main.h"
#include "read_layout.h"
#include "init-creatures.h"
#include "d2_alloc.h"
#include "pacman.h"
#include "ghost.h"
#include "score.h"
#include "move.h"
#include "gui.h"
#include "gui-ncurses.h"

/*
 * This is where the whole thing is glued together.
 */

/* 
 * Initial and minimum delay for each tick, and the amount of
 * time we should decrease the delay with for each tick. 
 */
#define INITIAL_DELAY 70000L
#define MIN_DELAY 1000L
#define DEC_DELAY 40

/* static (local) function prototypes */
static int main_game_loop(struct env *board, struct creature *pacman,
                struct creature *ghost, int cnt);
static int play_again(void);
static int game_won(const struct env *board);
static void finish(int won, int lives);

/* As is usual, main() just sets up a few things, then pass over
 * control to a different controlling function */
int main(void)
{
        int retval;
        struct env board;
        struct creature pacman;
        struct creature ghost[NUM_GHOSTS];

        if (!read_layout(&board)) 
                return EXIT_FAILURE;

        /* set up our gui things */
        if (!init_gui(board.rows, board.cols)) 
                return EXIT_FAILURE;

        /* initialise the random number generator */
        srand(time(NULL));

        /* main game loop */
        retval = main_game_loop(&board, &pacman, ghost, NUM_GHOSTS);

	d2_free_location_proper(board.pos, board.rows, board.cols);
        return EXIT_SUCCESS;
}

static int main_game_loop(struct env *board, 
                struct creature *pacman,
                struct creature *ghost, 
                int cnt)
{
        unsigned long time_to_sleep;
        enum dir direction;
        int i, won = 0;
        
        draw_board(board);
        box_print(0, 0, "Welcome. Hit any key to start :)");
        update_view();
        blocking_input();

        do {
                /* set up and place the players (ghosts etc) */
                if (!init_players(board, pacman, ghost, cnt))
                        return 0;

                /* blank out screen, ready for action */
                reset_view();
		draw_board(board);

                time_to_sleep = INITIAL_DELAY;
                for (;;) {
			/* check for user input */
                        direction = get_user_input();
                        if (direction == QUIT) break;

                        move_pacman(board, pacman, direction);
                        draw_creature(pacman);
			erase_tail(board, pacman);

                        /* Move and draw N ghosts. */
                        for (i=0; i<cnt; i++) {
                                ghost_move(board, &(ghost[i]), i);
                                draw_creature(&(ghost[i]));
				erase_tail(board, &(ghost[i]));
                        }

                        /* Up the score if we got any cherries,
                         * then print the new (or old) score. */
                        pick_up_cherries(board, pacman);
                        print_stat(board);

                        /* update the whole screen */
                        update_view();

                        /* Check if there's any more cherries to
                         * pick, otherwise the game is won. */
                        won = game_won(board);
                        if (won) break;

                        if (pacman_caught(pacman, ghost, cnt)) {
                                down_lives(board);
                                break;
                        }

                        /* 
                         * Sleep for a short while. This function
                         * call is the only non-ISO/ANSI-C call
                         * used by the program apart from the
                         * functions in gui.[hc], thus it may
                         * have to be replaced if ported to a
                         * non-unix platform. It conforms to the
                         * BSD 4.3 standard.
                         */
                        usleep(time_to_sleep);
                        if (time_to_sleep > MIN_DELAY)
                                time_to_sleep -= DEC_DELAY;
                } 
        } while (direction != QUIT && !won && lives_left(board) > 0 && play_again());
        
        finish(won, lives_left(board));
        return 1;
}

/* Just output some information/feedback as to whether the user
 * lost or chickened out... */
static void finish(int won, int lives)
{
        int c;
        if (lives == 0) {
                box_print(0, 0, "Sorry, you lost :(");
        }
        else if (!won) {
                box_print(0, 0, "So you don't want to play, eh? Meanie!");
        }
        while ((c = blocking_input()) && c != ' ' && c != 'q')
                ;
}

/* returns 1 if we want to play again, 0 if we don't */
static int play_again(void)
{
        int ch;
        char *s = "Hit space to continue, or q to quit";

        box_print(0, 0, s);
        while ((ch = blocking_input()) && ch != ' ' && ch != 'q')
                ;

        if (ch == 'q')
                return 0;
        else
                return 1;
}

static int game_won(const struct env *board)
{
        if (cherry_cnt(board) <= 0) {
                box_print(0, 0, "YOU WON!");
                return 1;
        }
        else 
                return 0;
}


