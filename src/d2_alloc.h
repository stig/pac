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

#ifndef D2_ALLOC_H
#define D2_ALLOC_H 1

struct location ***d2_alloc_location_null(int rows, int cols);
struct location ***d2_alloc_location(int rows, int cols);

void d2_free_location_proper(struct location ***base, int rows, int cols);
void d2_free_location(struct location ***base, int rows);

#endif	/* D2_ALLOC_H */
