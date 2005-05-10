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
#include "main.h"
#include "ghost.h"

/* 
 * Set up the initial attributes for the creatures, such as
 * colour, looks initial position and direction etc. 
 */


/* function prototypes for local helper functions */
static int creature_location(const struct location *loc,
                const int who);
static int get_starting_position(const struct env *board,
                struct pos *loc, const int who);
static void set_row_height(struct pos *loc, int row, int col);


/* Set up the starting conditions for a ghost */
int init_players(const struct env *board, 
                struct creature *pacman,
                struct creature *ghost,
                int cnt)
{
        int i;

        fputs("\rInit pacman... ", stderr);
        if (!get_starting_position(board, &(pacman->position), 'P'))
                return 0;
        pacman->colour = 1;
        pacman->direction = LEFT;
        pacman->looks[0] = "/\257\\";
        pacman->looks[1] = ">\260|";
        pacman->looks[2] = "\\_/";

        for (i=0; i<cnt; i++) {
                fprintf(stderr, "\rInit ghost %i... ", i);
                if (!get_starting_position(board, &(ghost[i].position), 'W'+i))
                        return 0;
                ghost[i].colour = i + 2;
                ghost[i].direction = ghost_initial_direction(i);
                ghost[i].looks[0] = "/\257\\";
                ghost[i].looks[1] = "|\"|";
                ghost[i].looks[2] = "^^^";
        }

        fputs("\rOk, found all. Returning.\n", stderr);
        return 1;
}

/* return a struct containing the starting position of a player
 */
static int get_starting_position(const struct env *board,
                struct pos *loc, const int who)
{
        int i, j;
        
        for (i=0; i<board->rows; i++) {
                for (j=0; j<board->cols; j++) {
                        if (creature_location(board->pos[i][j], who)) {
                                fprintf(stderr, "found starting position: "
                                                "row=%i, col=%i\n", i, j);
                                set_row_height(loc, i, j);
                                return 1;
                        }
                }
        }
        return 0;
}

static void set_row_height(struct pos *loc, int row, int col)
{
        loc->row = row;
        loc->col = col;
}

/* return 1 if the creature is located here, 0 otherwise */
static int creature_location(const struct location *loc, const int who)
{
        if (loc->bg == who) 
                return 1;
        else
                return 0;
}
