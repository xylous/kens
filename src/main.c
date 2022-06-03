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
 * Basic directions for snake nodes
 */
enum direction
{
    up,
    down,
    right,
    left,
};

/**
 * Linked lists.
 */
struct SnakeNode
{
    int x;
    int y;
    enum direction dir;
    struct SnakeNode *prev;
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
    /* Reinitialise the grid */
    free(*grid);
    (*grid) = create_grid();

    (*grid)[head->y][head->x] = GRID_HEAD;

    for (struct SnakeNode *body = head->next; body != NULL; body = body->next) {
        (*grid)[body->y][body->x] = GRID_BODY;
    }
}

/**
 * Print a grid to stdout
 */
void render_grid(int **grid)
{
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            switch(grid[i][j]) {
                case GRID_HEAD:
                    printf("o");
                    break;
                case GRID_BODY:
                    printf("x");
                    break;
                case GRID_FRUIT:
                    printf("f");
                    break;
                default: /* empty cell */
                    printf("-");
            }
            /* Put some spaces in between so it doesn't look horrible */
            printf(" ");
        }
        printf("\n");
    }
}

/**
 * Link two snake nodes together
 */
void link(struct SnakeNode **from, struct SnakeNode **to)
{
    (*from)->next = *to;
    (*to)->prev = *from;
}

/**
 * Return the last element in a snake bundle.
 */
struct SnakeNode *last_node(struct SnakeNode *head)
{
    for (; ; head = head->next) {
        if (head->next == NULL) {
            return head;
        }
    }
}

int main(int argc, char **argv)
{
    printf("Hello snake!\n");

    int **grid = create_grid();
    struct SnakeNode *head = new_snakenode(0, 0);
    struct SnakeNode *last = new_snakenode(0, 1);
    head->dir = right;
    link(&head, &last);

    update_grid(&grid, head);
    render_grid(grid);

    return 0;
}
