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

#ifndef GUI_NCURSES_H
#define GUI_NCURSES_H 1

int init_gui(int rows, int cols);
void draw_creature(const struct creature *ct);
enum dir get_user_input(void);
void update_view(void);
void reset_view(void);
int blocking_input(void);
void box_print(int rows, int cols, char *s);
void print_stat(struct env *board);
void draw_location(const struct env *base, int row, int col);

#endif /* GUI-NCURSES_H */
