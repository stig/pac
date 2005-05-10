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
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "d2_alloc.h"

/* 
 * This file contains functions to allocate and de-allocate a
 * two-dimensional array of pointers to 'struct location'.
 */


/* Free up a 2-dimensional array */
void d2_free_location(struct location ***base, int rows)
{
        int i;
        for (i=0; i<rows; i++) {
                free(base[i]);
        }
        free(base);
}

/* This function frees the memory pointed to by by the array
 * members as well. */
void d2_free_location_proper(struct location ***base, int
                rows, int cols)
{
        int i, j;
        for (i=0; i<rows; i++) {
                for (j=0; j<cols; j++) {
                        free(base[i][j]);
                }
                free(base[i]);
        }
        free(base);
}

/* return a two-dimensional array of pointers */
struct location ***d2_alloc_location(int rows, int cols)
{
        struct location ***base;
        struct location **tmp;
        int i;

        base = malloc(rows * sizeof(struct location **));
        if (!base) return NULL;
        
        for (i=0; i<rows; i++) {
                tmp = malloc(cols * sizeof(struct location *));
                if (!tmp) {
                        d2_free_location(base, i);
                        return NULL;
                }
                base[i] = tmp;
        }
        return base;
}

/* Return a two-dimensional array of pointers to NULL */
struct location ***d2_alloc_location_null(int rows, int cols)
{
        int i, j;
        struct location ***this;

        this = d2_alloc_location(rows, cols);
        if (!this) return NULL;

        for (i=0; i<rows; i++) {
                for (j=0; j<cols; j++) {
                        this[i][j] = NULL;
                }
        }
        return this;
}
