/* File: test.c
 * Author: ByXc
 * About: test
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171113
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
    FILE * file_pointer;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "Usage: %s pathname string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((file_pointer = fopen(argv[1], "ab+")) == NULL)
    {
        fprintf(stderr, "Open file %s failure!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    rewind(file_pointer);
    fputs(argv[2], file_pointer);

    if (fclose(file_pointer) != 0)
        fputs("Close file failure!\n", stderr);

    return 0;
}
