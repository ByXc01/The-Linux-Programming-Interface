/* File: scatter_gather.c
 * Author: ByXc
 * About: 使用 read()、write() 以及 malloc 函數包中必要的函數以實現 readv() 和 writev() 功能。
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171116
 * Github: ByXc01
 * Blog: http://ByXc01.githu.io
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns number of bytes read, 0 on EOF, or -1 on error
ssize_t readv_b(int fd, const struct iovec * iov, int iovcnt);

// Returns number of bytes written, or -1 on error
ssize_t writev_b(int fd, const struct iovec * iov, int iovcnt);


ssize_t readv_b(int fd, const struct iovec * iov, int iovcnt)
{
    ssize_t size, total;
    int count, length;
    char * buffer;

    for (count = size = 0; count < iovcnt; count++)
        size += iov[count].iov_len;

    buffer = (char *) malloc(sizeof (char) * size);
    if (buffer == NULL)
        return -1;
        
    total = read(fd, buffer, size);
    if (total == -1)
        return -1;

    for (count = length = 0; count < iovcnt; count++)
    {
        memcpy(iov[count].iov_base, &buffer[length], iov[count].iov_len);
        length += iov[count].iov_len;
    }

    free(buffer);

    return total;
}

ssize_t writev_b(int fd, const struct iovec * iov, int iovcnt)
{
    ssize_t size, total;
    int count, length;
    char * buffer;

    for (count = size = 0; count < iovcnt; count++)
        size += iov[count].iov_len;

    buffer = (char *) malloc(sizeof (char) * size);
    if (buffer == NULL)
        return -1;

    for (count = length = 0; count < iovcnt; count++)
    {
        memcpy(&buffer[length], iov[count].iov_base, iov[count].iov_len);
        length += iov[count].iov_len;
    }

    total = write(fd, buffer, size);
    free(buffer);

    return total;
}
