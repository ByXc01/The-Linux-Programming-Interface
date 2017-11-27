#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "malloc_b.h"

#define MAX 10

int main(int argc, char * argv[])
{
    int * odd, * even;
    char * temp;
    int count;

    printf("program break is now: %10p\n", sbrk(0));
    odd = (int *) calloc_b(sizeof (int), MAX);
    printf("program break is now: %10p\n", sbrk(0));
    temp = (char *) malloc_b(sizeof (char));
    printf("program break is now: %10p\n", sbrk(0));
    even = (int *) malloc_b(sizeof (int) * MAX);
    printf("program break is now: %10p\n", sbrk(0));

    if (odd == NULL || temp == NULL || even == NULL)
    {
        puts(strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (count = 0; count < MAX; count++)
    {
        odd[count] = count * 2 + 1;
        printf("%2d ", odd[count]);
    }
    puts("");

    *temp = 'B';
    printf("%c\n", *temp);

    char * tmp = (char *) realloc_b(temp, 5);
    if (tmp != NULL)
        temp = tmp;
    
    putchar(*temp);
    temp[1] = 'y', temp[2] = 'X', temp[3] = 'c', temp[4] = '\0';
    puts(temp);
    free_b(temp);

    printf("program break is now: %10p\n", sbrk(0));

    for (count = 0; count < MAX; count++)
    {
        even[count] = count * 2 + 2;
        printf("%2d ", even[count]);
    }
    
    printf("\n%zu\n", sizeof(struct s_block));

    free_b(even);
    printf("program break is now: %10p\n", sbrk(0));
    free_b(odd);
    printf("program break is now: %10p\n", sbrk(0));

    return 0;
}
