/* File: b_tee.c
 * Author: ByXc
 * About: 模仿簡單 tee 命令
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171029
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>      /* 爲 isprint() 提供函數原型 */
#include "tlpi_hdr.h"

#define BUFFER_SIZE 100
#define PRINTABLE(ch) (isprint(ch) ? ch : '#')

extern int optopt, opterr, optind;
extern char * optarg;

/* Print "usage" message and exit */
static void usageError(char * progNmae, char * msg, int opt)
{
    if (msg != NULL && opt != 0)
        fprintf(stderr, "%s (-%c)\n", msg, PRINTABLE(opt));
    fprintf(stderr, "Usage: %s [-a] filename\n", progNmae);

    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    int opt, fd;
    char buffer[BUFFER_SIZE];
    ssize_t numRead;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageError(argv[0], NULL, 0);

    if (argc == 2)
        fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                            S_IROTH | S_IWOTH);     /* rw-rw-rw */
    else
    {
        if ((opt = getopt(argc, argv, "a:")) != -1)
            switch (opt)
            {
                case 'a':
                    fd = open(optarg, O_WRONLY | O_CREAT | O_APPEND,
                              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                              S_IROTH | S_IWOTH);
                      break;
                default:
                      fatal("Unexpected case in switch()");
            }
    }

    while ((numRead = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(fd, buffer, numRead) != numRead || write(STDIN_FILENO, buffer, numRead) != numRead)
            fatal("couldn't write whole buffer");
    }

    if (numRead == -1)
        errExit("read()");

    if (close(fd) == -1)
        errExit("close output");

    return 0;
}
