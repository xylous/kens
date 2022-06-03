/* Copyright (c) 2022 xylous <xylous.e@gmail.com>
 *
 * This file is part of kens.
 * kens is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * kens is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * kens.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#define SCREEN_HEIGHT   10
#define SCREEN_WIDTH    10

#define GRID_HEAD   1
#define GRID_BODY   2
#define GRID_FRUIT  3

/**
 * Linked lists.
 */
struct SnakeNode
{
    int x;
    int y;
    struct SnakeNode *next;
};

/**
 * Create a new SnakeNode at the specified position
 */
struct SnakeNode *new_snakenode(int x, int y)
{
    struct SnakeNode *node = calloc(1, sizeof(struct SnakeNode));
    node->x = x;
    node->y = y;
    return node;
}

/**
 * Create a 2D array in heap of size [SCREEN_HEIGHT][SCREEN_WIDTH]
 */
int **create_grid(void)
{
    int **rows = calloc(SCREEN_HEIGHT, sizeof(int*));
    int *values = calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(int));

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        rows[i] = values + i * SCREEN_WIDTH;
    }

    return rows;
}

/**
 * Map out an entire snake onto a grid
 */
void update_grid(int ***grid, struct SnakeNode *head)
{
    for (; head != NULL; head = head->next) {
        (*grid)[head->y][head->x] = 1;
    }
}

/**
 * Print a grid to stdout
 */
void render_grid(int **grid)
{
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if (grid[i][j] != 0) {
                printf("x");
            } else {
                printf("-");
            }
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    printf("Hello snake!\n");

    int **grid = create_grid();
    struct SnakeNode *head = new_snakenode(0, 0);
    update_grid(&grid, head);
    render_grid(grid);

    return 0;
}
