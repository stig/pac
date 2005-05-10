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
#include "move.h"
#include "misc.h"

/* This file contains functions that are used by both the
 * pacman.c and ghost.c modules. */

static int accessible_location(struct location *loc);

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
