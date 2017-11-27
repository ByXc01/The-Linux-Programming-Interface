/* File: share.c
 * Author: ByXc
 * About: share open file flag and share file offset
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171115
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER 11

int main(int argc, char * argv[])
{
    int file_descriptor, duplicate;
    int open_file_flag1, open_file_flag2;
    char buffer[BUFFER];
    ssize_t size;

    if (argc != 2)
        usageErr("%s filename\n", argv[0]);

    file_descriptor = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWUSR |
                           S_IROTH | S_IWOTH);      // rw-rw-rw-
    duplicate = dup(file_descriptor);

    // 打開文件狀態的標志
    open_file_flag1 = fcntl(file_descriptor, F_GETFL);
    if (open_file_flag1 == -1)
        errExit("fcntl1");

    open_file_flag2 = fcntl(duplicate, F_GETFL);
    if (open_file_flag2 == -1)
        errExit("fcntl2");

    // 檢查是否共享打開文件的狀態標志
    if (open_file_flag1 == open_file_flag2)
        printf("share open file flag %d\n", open_file_flag1);

    if (write(file_descriptor, "Hello", 5) == -1)
        errExit("write1");

    if (write(duplicate, " ByXc", 5) == -1)
        errExit("write2");

    // 檢查是否共享文件的偏移量
    lseek(file_descriptor, 0, SEEK_SET);
    while ((size = read(duplicate, buffer, BUFFER)) > 0)
    {
        buffer[size] = '\0';
        puts(buffer);
    }

    if (size == -1)
        errExit("read");

    if (close(file_descriptor) == -1)
        errExit("close1");

    if (close(duplicate) == -1)
        errExit("close2");

    return 0;
}
