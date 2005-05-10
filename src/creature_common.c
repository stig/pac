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
#include "pac.h"
#include "creature_common.h"
#include "creature_ghosts.h"

/* This file contains functions that are used by both the
 * pac.c and ghost.c modules. */

static int accessible_location(struct location *loc);
static int creature_location(const struct location *loc,
                const int who);
static int get_starting_position(const struct env *board,
                struct pos *loc, const int who);
static void set_row_height(struct pos *loc, int row, int col);

/* returns the row and column coordinates of the next location
 * given the current location and the direction to move in. */
int next_location(struct creature *ct, int *row, int *col, enum dir c)
{
        switch (c) {
        case LEFT:
                *row = get_row(ct);
                *col = get_col(ct) - 1;
                break;
        case RIGHT:
                *row = get_row(ct);
                *col = get_col(ct) + 1;
                break;
        case UP:
                *row = get_row(ct) - 1;
                *col = get_col(ct);
                break;
        case DOWN:
                *row = get_row(ct) + 1;
                *col = get_col(ct);
                break;
        default:
                return 0;
        }
        return 1;
}

/* Return 1 if a creature can go to a location (all 8 surrounding
 * locations must be freely accessible as well) or 0 if not. */
int can_go_there(const struct env *board, int row, int col)
{
        int i, j, k = 1;

        /* if we try to go outside the board, return error */
        if (row - k < 0 || board->rows < row - k) return 0;
        if (col - k < 0 || board->cols < col - k) return 0;
        
        /* check whether all 9 locations affected are free */
        for (i=-k; i<=k; i++) {
                for (j=-k; j<=k; j++) {
                        if (!accessible_location(board->pos[row+i][col+j]))
                                return 0;
                }
        }

        /* we can go to this location, so return true. */
        return 1;
}

/* return 1 if the location is accessible, 0 if not */
static int accessible_location(struct location *loc)
{
        int retval = 0;
        
        /* if it is off the screen, then it is not accessible */
        if (loc == NULL) return 0;
        
        switch(loc->bg) {
                case '#': 
                case '%': 
                        retval = 0;
                        break;
                default:
                        retval = 1;
                        break;
        }

        return retval;
}

int get_row(const struct creature *ct)
{
	return ct->position.row;
}

int get_col(const struct creature *ct)
{
	return ct->position.col;
}

/* 
 * Set up the initial attributes for the creatures, such as
 * colour, looks initial position and direction etc. 
 */



/* Set up the starting conditions for a ghost */
int init_players(const struct env *board, 
                struct creature *pac,
                struct creature *ghost,
                int cnt)
{
        int i;

        fputs("\rInit pac... ", stderr);
        if (!get_starting_position(board, &(pac->position), 'P'))
                return 0;
        pac->colour = 1;
        pac->direction = LEFT;
        pac->looks[0] = "/\257\\";
        pac->looks[1] = ">\260|";
        pac->looks[2] = "\\_/";

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
