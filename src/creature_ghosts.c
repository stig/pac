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
#include "pac.h"
#include "creature_common.h"
#include "creature_ghosts.h"


/* 
 * This module is intended to include all operations that has to
 * do with the moving around of ghosts.
 */


static enum dir_t opposite_dir(enum dir_t dir);
static int turn_and_go_back(int range);
static int possible_directions(struct env *board, struct creature *ct,
                enum dir_t odir, enum dir_t *dirs, int cnt_dirs);

static enum dir_t pick_direction(enum dir_t *dirs, int cnt);

/* how likely it is we will turn and go back */
#define TURN_RATE 50
#define NUM_DIRS 4

/* This function returns as different initial direction for each
 * ghost based on their index */
enum dir_t ghost_initial_direction(int idx)
{
        return UP+idx;
}

/* 
 * "idx" is provided so we can know which ghost we are dealing
 * with if we later want to individualise the behaviour of the
 * ghosts.
 *
 * XXX: idx is not used
 */
void ghost_move(struct env *board, struct creature *ct, int idx)
{
        int row, col, cnt;
        enum dir_t direction, poss_dirs[NUM_DIRS];

        if (!turn_and_go_back(TURN_RATE)) 
                direction = opposite_dir(ct->dir);
        else 
                direction = ct->dir;

        cnt = possible_directions(board, ct, direction, poss_dirs, NUM_DIRS);
        if (cnt > 0)
                direction = pick_direction(poss_dirs, NUM_DIRS);
        
        next_location(board, ct, &row, &col, direction);

        ct->pos.row = row;
        ct->pos.col = col;
        ct->dir = direction;

	idx++; ++idx;
}

/* return a random valid direction */
static enum dir_t pick_direction(enum dir_t *dirs, int cnt)
{
        int rnd;
        do 
                rnd = myrand(cnt);
        while (dirs[rnd] == NO_INPUT);
        return dirs[rnd];
}

/* return the number of directions we can walk in */
static int possible_directions(struct env *board, 
                struct creature *ct, enum dir_t odir,
                enum dir_t *dirs, int cnt_dirs)
{
        int row, col, i;
        int cnt = 0;    /* number of possible directions */
        
        for (i=0; i<cnt_dirs; i++) {
                row = get_row(ct);
                col = get_col(ct);
                next_location(board, ct, &row, &col, UP+i);
                if (can_go_there(board, row, col) && (unsigned)UP+i != odir) {
                        dirs[i] = UP+i;
                        cnt++;
                }
                else 
                        dirs[i] = NO_INPUT;
        }
        return cnt;
}

/* simply return the opposite direction of the argument value. */
static enum dir_t opposite_dir(enum dir_t dir)
{
        switch (dir) {
                case UP: 
                        dir = DOWN;
                        break;
                case RIGHT:
                        dir = LEFT;
                        break;
                case DOWN:
                        dir = UP;
                        break;
                case LEFT:
                        dir = RIGHT;
                        break;
                default:
                        break;
        }
        return dir;
}

/* Once in a while we would like the ghosts to randomly turn and
 * go back where it came from.  We will only go back where we
 * came from 1 out of RANGE times, so we just return if we're not
 * going to change direction. 
 */
static int turn_and_go_back(int range)
{
        int rnd;

        rnd = myrand(range);
        if (rnd)        
                return 0;
        else    
                return 1;
}
