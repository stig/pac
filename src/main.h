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

#ifndef MAIN_H
#define MAIN_H 1

/* 
 * A position in the world of pacman can be broken down to a
 * column/row.
 */
struct pos {
        int col;
        int row;
};

/* 
 * This strictly don't need to be a struct, but it is implemented
 * such to facilitate easy updates.
 */
struct location {
        int bg;
};

/* 
 * This is the representation of the world of Pacman. 
 *
 */
struct env {
        unsigned long total_score;
        unsigned int cherries_left;
        unsigned int lives_left;
        int rows;
        int cols;
        struct location ***pos;
};

enum dir {QUIT, NO_INPUT, UP, RIGHT, DOWN, LEFT};

/* 
 * This struct is used to represent both pacman and the ghosts. 
 */
struct creature {
        char *looks[3];
        int colour;
        enum dir direction;
        struct pos position;
};

#endif /* MAIN_H */
