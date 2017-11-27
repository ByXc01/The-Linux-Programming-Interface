/* File: append.c
 * Author: ByXc
 * About: 編寫一個程序，使用 O_APPEND 標志並以寫方式打開一個已存在的文件，且將文件偏移量至於文件的起始出，再寫入數據。數據會顯示在文件的那個位置？
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171113
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char * argv[])
{
    int file_descriptor;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname string\n", argv[0]);
    
    file_descriptor = open(argv[1], O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR |
                           S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // rw-rw-rw
    if (file_descriptor == -1)
        errExit("open");

    lseek(file_descriptor, 0, SEEK_SET);
    if (write(file_descriptor, argv[2], 3) < 0)
        errExit("write");

    if (close(file_descriptor) == -1)
        errExit("close");

    return 0;
}
