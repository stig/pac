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

#ifndef CREATURE_COMMON_H
#define CREATURE_COMMON_H 1

enum dir {QUIT, NO_INPUT, UP, RIGHT, DOWN, LEFT};

struct pos {
        int col;
        int row;
};

struct creature {
        char *looks[3];
        int colour;
        enum dir direction;
        struct pos position;
};

int can_go_there(const struct env *, int, int);
int next_location(const struct env *, struct creature *ct, int *row, int *col, enum dir c);

int get_row(const struct creature *ct);
int get_col(const struct creature *ct);

int init_players(const struct env *board, 
                struct creature *pac,
                struct creature *ghost,
                int cnt);

#endif 
