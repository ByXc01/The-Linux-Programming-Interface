/* File: atomic_append.c
 * Author: ByXc
 * About: atomic append
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171114
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char * argv[])
{
    int file_descriptor, flags;
    int number;
    bool x = false;

    if ((argc < 3 || argc > 4) || strcmp(argv[1], "--help") == 0)
        usageErr("%s file num-bytes [x]", argv[0]);

    flags = O_WRONLY | O_CREAT | O_APPEND;
    if (argc == 4) 
    {
        if (strcmp(argv[3], "x") == 0)
        {
            flags &= ~O_APPEND;
            x = true;
        }
        else
            usageErr("%s file num-bytes [x]", argv[0]);
    }

    file_descriptor = open(argv[1], flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                                           S_IROTH | S_IWOTH);      // rw-rw-rw
    if (file_descriptor == -1)
        errExit("open");

    number = getInt(argv[2], GN_ANY_BASE | GN_GT_0, argv[2]);
    if (x)
        lseek(file_descriptor, 0, SEEK_END);

    for (int count = 0; count < number; count++)
        write(file_descriptor, "C", 1);
    
    if (close(file_descriptor) == -1)
        errExit("close");

    return 0;
}

