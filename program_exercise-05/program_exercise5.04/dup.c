/* File: dup.c
 * Author: ByXc
 * About: 模仿 dup() 和 dup2()
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171114
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
// #include "ename.c.inc"

// Returns (new) file descriptor on success, or -1 on error
int dup_b(int oldfd);
int dup2_b(int oldfd, int newfd);

int dup_b(int oldfd)
{
    return fcntl(oldfd, F_DUPFD, 0);
}

int dup2_b(int oldfd, int newfd)
{
    int file_descriptor;

    if (fcntl(oldfd, F_GETFL) == -1)
    {
        errno = EBADF;  // EBADF 應該定義在 errno.h
        /*
        for (int index = 1; index <= MAX_ENAME; index++)
            if (strcmp(ename[index], "EBADF") == 0)
            {
                errno = index;
                break;
            }
        */

        return -1;
    }

    if (oldfd == newfd)
        return newfd;

    close(newfd);
    file_descriptor = fcntl(oldfd, F_DUPFD, newfd);

    if (file_descriptor == -1)
        return -1;

    return file_descriptor;
}
