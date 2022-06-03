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

int main(int argc, char **argv)
{
    printf("Hello snake!\n");
}
