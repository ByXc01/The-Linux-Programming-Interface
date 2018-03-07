/* File: test.c
 * Author: ByXc
 * About: look book.
 * Compiler: gcc 7.2.0 for ubuntu
 * Version: 1.0
 * Date:20180304
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <sys/times.h>
#include <fcntl.h>
#include <time.h>

#define RANDOM
#define MIN 1000
#define MAX 20000

static void generateFilename(int fileNum, char (* fileName)[8]);
static int randomNum(void);
static void deleteFile(char * pathname, int fileNum, char (*fileName)[8]);
static void printTime(void);

int main(int argc, char * argv[])
{
    int openFlags, count, fileNum;              // fileNum <= (MAX - MIN)
    int fd;
    mode_t filePerms;
    char fileName[8], pathname[255];            // xNNNNNN\0
    char (* nameArr)[8];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s [path name] [file number]\n", argv[0]);

    srand((unsigned long) time(NULL));
    fileNum = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "fileNum");
    nameArr = (char (*)[8]) malloc(fileNum * sizeof(fileName));
    if (nameArr == NULL)
        errExit("malloc");

    openFlags = O_WRONLY | O_CREAT | O_EXCL;    // 創建只寫文件
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;              // rw-rw-rw-
    generateFilename(fileNum, nameArr);
    for (count = 0; count < fileNum; count++)
    {
        snprintf(pathname, 255, "%s/%s", argv[1], nameArr[count]);
        if ((fd = open(pathname, openFlags, filePerms)) == -1)
            errExit("open");

        if (write(fd, "Z", 1) != 1)
            errExit("wirte");

        if (close(fd) == -1)
            errExit("close");
    }

    deleteFile(argv[1], fileNum, nameArr);
    free(nameArr);

    printTime();
    
    exit(EXIT_SUCCESS);
}

static void generateFilename(int  fileNum, char (* fileName)[8])
{
    int value, count;
    int * num;
    bool flag;
    static bool first = true;

    num = (int *) malloc(fileNum * sizeof(int));
    if (num == NULL)
        errExit("malloc");

#ifdef RANDOM


    for (count = 0; count < fileNum;)
    {
        flag = true;
        value = randomNum();

        if (first)
        {
            num[0] = randomNum();
            first = false;
        }
        else
        {
            for (int index = 0; index <= count && !first; index++)
                if (num[index] == value)
                {
                    flag = false;
                    break;
                }

        }

        if (flag)
            num[count++] = value;
    }
#elif defined(MIN) && defined(MAX)
    for (count = 0; count < fileNum; count++)
        num[count] = MIN + count;
#else
    for (count =0; count < fileNum; count++)
        num[count] = count;
#endif

    for (count = 0; count < fileNum; count++)
        snprintf(fileName[count], 8, "x%06d", num[count]);

    free(num);
}

static int randomNum(void)
{
    unsigned num;

#if defined (MAX) && defined (MIN)
    while ((num = rand() % MAX + 1) < MIN)
        continue;
#else
    num = rand() % 100000 + 1;
#endif

    return num;
}

static void deleteFile(char * pathname, int fileNum, char (* fileName)[8])
{
    char path[255];

    for (int count = 0; count < fileNum; count++)
    {
        snprintf(path, 255, "%s/%s", pathname, fileName[count]);
        if (remove(path) == -1)
            fprintf(stderr, "Delete failed, file: %s\n", path);
    }
}

static void printTime(void)
{
    clock_t consumeTime;
    struct tms t;
    long int clockTicks;
    double real, user, sys;

    if ((consumeTime = times(&t)) == (clock_t) -1)
        errExit("times");

    if ((clockTicks = sysconf(_SC_CLK_TCK)) == -1)
        errExit("sysconf");

    real = ((double) t.tms_utime + (double) t.tms_stime) / clockTicks;
    user = (double) t.tms_utime / clockTicks;
    sys  = (double) t.tms_stime / clockTicks;
    printf("real    %.3fs\n", real);
    printf("user    %.3fs\n", user);
    printf("sys     %.3fs\n", sys);
}
