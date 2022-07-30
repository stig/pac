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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pac.h"
#include "creature_common.h"
#include "score.h"
#include "d2_alloc.h"
#include "read_layout.h"

/* 
 * Checks the size of the board in the file, allocates a
 * 2-dimensional array to hold the board and reads into this
 * array the board layout. 
 */


#define MAP_DIR "maps"

#define MINSIZE 16      /* minimum hor/vert size of board */

/* helper functions local to this file */
static void get_board_size(FILE *fp, int *rows, int *cols);
static struct location* new_location(struct env *base, int tok);
static int read_file(struct env *base, FILE *fp);


/* 
 * Reads in the layout of a board from a file called
 * "layout.dat".
 *
 * FIXME: need to facilitate several layouts.
 */
int read_layout(struct env *base)
{
        FILE *fp;    
        char fname[FILENAME_MAX] = MAP_DIR "/level-1.pac";  /* XXX: ugly hack! */
        int rows, cols;

        /* if we cannot open the file, we cannot read from it
         * either. */
        fp = fopen(fname, "r");
        if (!fp) return 0;

        get_board_size(fp, &rows, &cols);
        if (rows < MINSIZE || cols < MINSIZE) {
                fputs("Board not large enough.\n", stderr);
                return 1;
        }
        base->rows = rows;
        base->cols = cols;
        base->total_score = 0;
        base->cherries_left = 0;
        base->lives_left = 3;
        base->pos = d2_alloc_location_null(rows, cols);
        if (!base->pos) return 0;
        
        if (!read_file(base, fp)) {
                d2_free_location_proper(base->pos, base->rows, base->cols);
                return 0;
        }
        
        return 1;
}

static int read_file(struct env *base, FILE *fp)
{
        struct location *tmp;
        int i, j, len, cnt = 0;
        char line[BUFSIZ];
        char *ch;

        /* read datafile and populate data array */
        rewind(fp);
        for (i=0; i<base->rows; i++) {
                if (!fgets(line, sizeof line, fp)) break;
                ch = strchr(line, '\0');
                len = ch - line;        
                
                for (j=0; j<len; j++) {
                        /* what is after the newline is undefined */
                        if ('\n' == line[j]) break;

                        /* get a position type */
                        tmp = new_location(base, line[j]);
                        if (!tmp) {
                                fputs("Could not allocate location holder.\n",
                                                stderr); 
                                return 0;
                        }       
                        base->pos[i][j] = tmp;
                        
                        cnt++;
                }
        }

        if (ferror(fp) || ferror(fp)) {
                if (ferror(fp)) fputs("Error while reading layout.\n", stderr);
                else fputs("Premature EOF.\n", stderr);
                return 0;
        }
        fprintf(stderr, "Read location info: %i non-blank positions\n", cnt);
        return 1;
}

static void get_board_size(FILE *fp, int *rows, int *cols)
{
        char line[BUFSIZ], *c;
        int len = 0;

        rewind(fp);
        *rows = *cols = 0;
        while ((c = fgets(line, sizeof line, fp))) {
                ++*rows;
                c = strchr(line, '\0');
                len = c - line;
                while(isspace(line[len-1])) len--; 
                if (len > *cols) *cols = len;
        }
        if (ferror(fp)) {
                fputs("Error finding size of layout.\n", stderr);
                *rows = *cols = 0;
        } 

        fprintf(stderr, "Board size: rows=%d, cols=%d\n", *rows, *cols);
}

static struct location* new_location(struct env *board, int tok)
{
        struct location* tmp;

        tmp = malloc(sizeof(struct pos_t));
        if (!tmp) return NULL;  

        switch (tok) {
        case 'c': /* small cherry */
                up_cherries(board);
                tmp->bg = 'c';
                break; 
        case 'C':       /* big cherry */
                up_cherries(board);
                tmp->bg = 'C';
                break;
        case 'w': /* ghost 1 */
        case 'W':
                tmp->bg = 'W';
                break;
        case 'x': /* ghost 2 */
        case 'X': 
                tmp->bg = 'X';
                break;
        case 'y': /* ghost 3 */
        case 'Y': 
                tmp->bg = 'Y';
                break;
        case 'z': /* ghost 4 */
        case 'Z': 
                tmp->bg = 'Z';
                break;
        case 'p': /* pacman */
        case 'P': 
                tmp->bg = 'P';
                break;
        case ' ': /* space */
        case '\t': 
                tmp->bg = ' ';
                break;  
        default: /* inaccessible */
                tmp->bg = '#';
                break; 
        }

        return tmp;     
}
