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
#include "gui.h"
#include "gui_unix_ncurses.h"

static void erase_tail_vertical(const struct env *base, 
		const struct creature *ct, int offset);
static void erase_tail_horisontal(const struct env *base, 
		const struct creature *ct, int offset);

/* Move to the start of the screen and print the board layout.
 * Call a function to print each cell. */
void draw_board(struct env *board)
{
        int i, j;
        
        for (i=0; i<board->rows; i++) {
                for (j=0; j<board->cols; j++) {
                        draw_location(board, i, j);
                }
        }
}

/* Erase the tail of a creature, so we don't have to redraw the whole
 * screen every iteration. This makes for a smoother game as well. */
void erase_tail(const struct env *base, const struct creature *ct)
{
	switch(ct->direction){
		case UP: 
			erase_tail_horisontal(base, ct, 2);
			break;
		case DOWN:
			erase_tail_horisontal(base, ct, -2);
			break;
		case LEFT:
			erase_tail_vertical(base, ct, 2);
			break;
		case RIGHT:
			erase_tail_vertical(base, ct, -2);
			break;
		default:
			break;
	}
}

static void erase_tail_horisontal(const struct env *base, 
		const struct creature *ct, int offset)
{
	int i;
	for (i=-1; i<2; i++) {
		draw_location(base, get_row(ct) + offset, get_col(ct)+i);
	}
}

static void erase_tail_vertical(const struct env *base, 
		const struct creature *ct, int offset)
{
	int i;
	for (i=-1; i<2; i++) {
		draw_location(base, get_row(ct)+i, get_col(ct) + offset);
	}
}
