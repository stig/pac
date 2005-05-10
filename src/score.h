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

#ifndef SCORE_H
#define SCORE_H 1

void pick_up_cherries(struct env *board, struct creature *ct);
unsigned long get_score(const struct env *board);
int cherry_cnt(const struct env *board);
void up_cherries(struct env *board);

void up_lives(struct env *board);
unsigned int lives_left(const struct env *board);
void down_lives(struct env *board);

#endif /* SCORE_H */
