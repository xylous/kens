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
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define SCREEN_HEIGHT   10
#define SCREEN_WIDTH    10

#define GRID_HEAD   1
#define GRID_BODY   2
#define GRID_FRUIT  3

#define NUM_FRUIT   1

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
 * All we need to know is the location of the fruit.
 */
struct Fruit
{
    int x;
    int y;
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
                    printw("o");
                    break;
                case GRID_BODY:
                    printw("x");
                    break;
                case GRID_FRUIT:
                    printw("f");
                    break;
                default: /* empty cell */
                    printw("-");
            }
            /* Put some spaces in between so it doesn't look horrible */
            printw(" ");
        }
        printw("\n");
    }
    refresh();
    move(0, 0);
}

/**
 * Link two snake nodes together
 */
void link_nodes(struct SnakeNode *from, struct SnakeNode *to)
{
    from->next = to;
    to->prev = from;
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

/**
 * Update the head cell with the new position
 */
void move_head(struct SnakeNode *head)
{
    switch (head->dir) {
        /* Since the origin of the screen is in the top left corner, when the
         * snake is going down, the Y co-ordinate is increasing, and vice-versa
         */
        case up:
            head->y--;
            break;
        case down:
            head->y++;
            break;
        case right:
            head->x++;
            break;
        case left:
            head->x--;
            break;
    }
    if (head->x >= SCREEN_WIDTH) {
        head->x = 0;
    } else if (head->x < 0) {
        head->x = SCREEN_WIDTH - 1;
    } else if (head->y >= SCREEN_HEIGHT) {
        head->y = 0;
    } else if (head->y < 0) {
        head->y = SCREEN_HEIGHT - 1;
    }
}

/**
 * Move the snake along
 */
void move_snake(struct SnakeNode *head)
{
    /* black magic */
    if (head->next != NULL) {
        struct SnakeNode *last = last_node(head);
        last->prev->next = NULL;
        *last = *head;
        link_nodes(head, last);
    }
    move_head(head);
}

/**
 * Change the direction of the head based on the input character
 */
void change_direction(struct SnakeNode *head, int ch)
{
    enum direction dir = head->dir;
    switch (ch){
        case KEY_UP:
            if (dir != down)
                dir = up;
            break;
        case KEY_DOWN:
            if (dir != up)
                dir = down;
            break;
        case KEY_RIGHT:
            if (dir != left)
                dir = right;
            break;
        case KEY_LEFT:
            if (dir != right)
                dir = left;
            break;
    }
    head->dir = dir;
}

/**
 * Create a list of fruit in heap memory
 */
struct Fruit **create_fruit_list()
{
    struct Fruit **list = calloc(NUM_FRUIT, sizeof(struct Fruit *));
    return list;
}

/**
 * Create a new fruit at the given co-ordinates
 */
struct Fruit *new_fruit(int x, int y)
{
    struct Fruit *fruit = malloc(sizeof(struct Fruit));
    fruit->x = x;
    fruit->y = y;
    return fruit;
}

/**
 * Return a list of all possible fruit positions that don't overlap with the
 * snake and with other fruit.
 */
struct Fruit **possible_positions(struct Fruit **list, struct SnakeNode *snake)
{
    struct Fruit **possibilities = calloc(SCREEN_HEIGHT * SCREEN_WIDTH, sizeof(struct Fruit));
    int length = 0;

    for (int y = 0; y < SCREEN_HEIGHT; y ++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            possibilities[length++] = new_fruit(x, y);
        }
    }

    /* Remove all impossibilities */
    for (; snake != NULL; snake = snake->next) {
        possibilities[snake->x + snake->y] = NULL;
    }
    for (int i = 0; i < NUM_FRUIT; i++) {
        if (list[i] != NULL) {
            possibilities[list[i]->x + list[i]->y] = NULL;
        }
    }

    return possibilities;
}

/**
 * Pick a random entry in the list of possible fruit. At the same time, remove
 * it from the list of possibilities
 */
struct Fruit *pick_random(struct Fruit **possible)
{
    struct Fruit *result = new_fruit(0, 0);
    while (1) {
        int r = random() % (SCREEN_WIDTH * SCREEN_HEIGHT);
        if (possible[r] != NULL) {
            *result = *(possible[r]);
            possible[r] = NULL;
            return result;
        }
    }
}

/**
 * For every fruit slot available, create a fruit at a random position that
 * doesn't overlap with the snake
 */
void fill_list_of_fruit(struct Fruit **list, struct SnakeNode *snake)
{
    struct Fruit **possible = possible_positions(list, snake);
    for (int i = 0; i < NUM_FRUIT; i++) {
        if (list[i] == NULL) {
            list[i] = pick_random(possible);
        }
    }
}

int main(int argc, char **argv)
{
    /* Initialise ncurses */
    initscr();
    timeout(0.3);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    srandom(time(NULL));

    int **grid = create_grid();
    struct SnakeNode *head = new_snakenode(0, 0);
    head->dir = right;
    struct Fruit **fruit_list = create_fruit_list();

    /* Main game loop */
    int ch = 0;
    while((ch = getch()) != NULL) { /* if NULL, then we received a SIGTERM */
        fill_list_of_fruit(fruit_list, head);
        change_direction(head, ch);
        update_grid(&grid, head);
        usleep(1000 * 400);
        move_snake(head);
    }

    /* Close ncurses session */
    endwin();

    return 0;
}
