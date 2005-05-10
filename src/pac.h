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

#ifndef PAC__PAC_H
#define PAC__PAC_H 1

#define PACKAGE_NAME "Pac"
#define PACKAGE_VERSION "0.3"
#define PACKAGE_BUGREPORT "stig@brautaset.org"


/* 
 * This strictly don't need to be a struct, but it is implemented
 * such to facilitate easy extensions.
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
	unsigned int current_level;
	
        int rows;
        int cols;
        struct location ***pos;
};

#endif 

