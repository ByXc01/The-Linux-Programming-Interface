/* File: large_file.c
 * Author: ByXc
 * About: 使用標準 I/O 系統調用(open() 和 lseek())　和 off_t　數據類型修改程序清單 5-3　中的程序。將宏 _FILE_OFFSET_BITS 的值設置爲 64 進行編譯，並測該程序是否能創建一個大文件。
 * Version: 2.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171113
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#define _FILE_OFFSET_BITS 64
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char * argv[])
{
    int file_descriptor;
    off_t offset;

    if(argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname offset\n", argv[0]);

    file_descriptor = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (file_descriptor == -1)
        errExit("open");

    offset = atoll(argv[2]);
    if (lseek(file_descriptor, offset, SEEK_SET) == -1)
        errExit("lseek");

    if (write(file_descriptor, "test", 4) == -1)
        errExit("write");

    return 0;
}
