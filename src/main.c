#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 10

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

int main(int argc, char **argv)
{
    printf("Hello snake!\n");
}
