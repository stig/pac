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
#include "main.h"
#include "misc.h"
#include "score.h"

static void up_score(struct env *board, int score);
static int pick_cherry(struct env *board, struct location *loc);
static void down_cherries(struct env *board);

/* Check all locations pacman occupies at the moment whether they
 * contain any cherries */
void pick_up_cherries(struct env *board, struct creature *ct)
{
        int i, j;
        int score = 0;
        for (i=-1; i<2; i++) {
                for (j=-1; j<2; j++) {
                        score += pick_cherry(board, (board->pos[get_row(ct)+i][get_col(ct)+j]));
                }
        }

	/* give a higher score the more lives we have left */
        up_score(board, score * lives_left(board));
}

/* Check if the current location holds a cherry, and take/eat it
 * if it does. */
static int pick_cherry(struct env *board, struct location *loc)
{
        int points;
        switch(loc->bg) {
                case 'c' :
                        loc->bg = ' ';
                        points = 10;
                        down_cherries(board);
                        break;
                case 'C':
                        loc->bg = ' ';
                        points = 100;
                        down_cherries(board);
                        break;
                default:
                        points = 0;
                        break;
        }
        return points;
}


unsigned int lives_left(const struct env *board)
{
        return board->lives_left;
}

void up_lives(struct env *board)
{
        board->lives_left++;
}

void down_lives(struct env *board)
{
        board->lives_left--;
}


unsigned long get_score(const struct env *board)
{
        return board->total_score;
}

static void up_score(struct env *board, int score)
{
        board->total_score += score;
}

int cherry_cnt(const struct env *board)
{
        return board->cherries_left;
}

void up_cherries(struct env *board)
{
        board->cherries_left++;
}

static void down_cherries(struct env *board)
{
        board->cherries_left--;
}


