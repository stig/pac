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
#include "pac.h"
#include "creature_common.h"
#include "creature_pac.h"

/* 
 * This module contains functions that have to do with moving pacman
 * around on, plus detecting when/if he get caught by one of the ghosts.
 */

static int creature_contact(const struct pos_t *pac, const struct pos_t *ghost);

/* Given a direction, see if we can go to the place that would
 * lead us. If we cannot, continue in the old direction if we
 * can. otherwise, just stay where we are. */
void move_pac(const struct env *board, 
                struct creature *pac,
                enum dir_t direction) 
{
        int row, col;
        
        if (!next_location(board, pac, &row, &col, direction)) {
                direction = pac->dir;
                next_location(board, pac, &row, &col, direction);
        }
        
        if (can_go_there(board, row, col)) {
                pac->dir = direction;
                pac->pos.row = row;
                pac->pos.col = col;
        }
}

/* Check all the ghosts to see if pac has contact with any of
 * them.  Return 1 if pac is caught (there is contact) or 0 if
 * not. */
int pac_caught(const struct creature *pac, 
                const struct creature ghost[], 
                const int ghost_cnt)
{
        int i, retval = 0;

        for (i=0; i<ghost_cnt; i++) {
                if (creature_contact(&(pac->pos), 
                                        &(ghost[i].pos))) {
                        retval = 1;
                        break;
                }
        }
        return retval;
}

/* Return 0 if there is no contact between the creatures, or 1 if
 * there is contact. */
static int creature_contact(const struct pos_t *pac, 
                const struct pos_t *ghost)
{
        if (2 >= abs(pac->row - ghost->row) && 
                        2 >= abs(pac->col - ghost->col))
                return 1;
        else
                return 0;
        
}
