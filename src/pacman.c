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
#include <stdlib.h>
#include "main.h"
#include "pacman.h"
#include "move.h"

/* 
 * This module contains functions that have to do with moving pacman
 * around on, plus detecting when/if he get caught by one of the ghosts.
 */

static void up_orient(struct creature *pac);
static void right_orient(struct creature *pac);
static void down_orient(struct creature *pac);
static void left_orient(struct creature *pac);
static void new_orientation(struct creature *pac, enum dir ndir);
static int creature_contact(const struct pos *pac, const struct pos *ghost);

/* Given a direction, see if we can go to the place that would
 * lead us. If we cannot, continue in the old direction if we
 * can. otherwise, just stay where we are. */
void move_pacman(const struct env *board, 
                struct creature *pac,
                enum dir direction) 
{
        int row, col;
        
        if (!next_location(pac, &row, &col, direction)) {
                direction = pac->direction;
                next_location(pac, &row, &col, direction);
        }
        
        if (can_go_there(board, row, col)) {
                pac->direction = direction;
                pac->position.row = row;
                pac->position.col = col;
        }
	new_orientation(pac, direction);
}

/* Check all the ghosts to see if pacman has contact with any of
 * them.  Return 1 if pacman is caught (there is contact) or 0 if
 * not. */
int pacman_caught(const struct creature *pacman, 
                const struct creature ghost[], 
                const int ghost_cnt)
{
        int i, retval = 0;

        for (i=0; i<ghost_cnt; i++) {
                if (creature_contact(&(pacman->position), 
                                        &(ghost[i].position))) {
                        retval = 1;
                        break;
                }
        }
        return retval;
}

/* Return 0 if there is no contact between the creatures, or 1 if
 * there is contact. */
static int creature_contact(const struct pos *pac, 
                const struct pos *ghost)
{
        if (2 >= abs(pac->row - ghost->row) && 
                        2 >= abs(pac->col - ghost->col))
                return 1;
        else
                return 0;
        
}

static void new_orientation(struct creature *pac, enum dir ndir)
{
	static enum dir odir;

	if (odir == ndir) return;
	odir = ndir;
	switch(odir) {
		case UP:
			up_orient(pac);
			break;
		case RIGHT:
			right_orient(pac);
			break;
		case DOWN:
			down_orient(pac);
			break;
		case LEFT:
			left_orient(pac);
			break;
		default:
			break;
	}
}

static void up_orient(struct creature *pac)
{
	pac->looks[0] = "/V\\";
	pac->looks[1] = "|\260|";
	pac->looks[2] = "\\_/";
}

static void right_orient(struct creature *pac)
{
	pac->looks[0] = "/\257\\";
	pac->looks[1] = "|\260<";
	pac->looks[2] = "\\_/";
}

static void down_orient(struct creature *pac)
{
	pac->looks[0] = "/\257\\";
	pac->looks[1] = "|\260|";
	pac->looks[2] = "\\^/";
}

static void left_orient(struct creature *pac)
{
	pac->looks[0] = "/\257\\";
	pac->looks[1] = ">\260|";
	pac->looks[2] = "\\_/";
}
