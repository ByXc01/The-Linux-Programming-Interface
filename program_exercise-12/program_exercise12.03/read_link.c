/* File: read_link.c
 * Author: ByXc
 * About: read link
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171229
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#ifndef LINE_MAX
    #define LINE_MAX 180
#endif

static bool isNumeric(const char * numeric);
static char * getFd(const char * pid, const char * fd);
static void printInfo(const char * pid, const char * fd);

int main(int argc, char * argv[])
{
    DIR * pid, * fd;
    struct dirent * dir_pid, * dir_fd;
    char pathname[PATH_MAX], * link_pathname;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pahtname\n", *argv);

    if ((pid = opendir("/proc")) == NULL)
        errExit("opendir");

    while ((dir_pid = readdir(pid)) != NULL)
    {
        if (!isNumeric(dir_pid->d_name))
            continue;

        snprintf(pathname, PATH_MAX, "/proc/%s/fd", dir_pid->d_name);
        if ((fd = opendir(pathname)) == NULL)
            continue;

        while ((dir_fd = readdir(fd)) != NULL)
        {
            if (!isNumeric(dir_fd->d_name))
                continue;

            if ((link_pathname = getFd(dir_pid->d_name, dir_fd->d_name)) == NULL)
                continue;

            if (strcmp(link_pathname, argv[1]) == 0)
                printInfo(dir_pid->d_name, dir_fd->d_name);
        }

        if (closedir(fd) == -1)
            continue; //errExit("closedir");
    }

    if (closedir(pid) == -1)
        errExit("closedir");

    return 0;
}

static bool isNumeric(const char * numeric)
{
    while (*numeric)
    {
        if (!isdigit(*numeric))
            return false;
        numeric++;
    }

    return true;
}

static char * getFd(const char * pid, const char * fd)
{
    char pathname[PATH_MAX];
    static char buffer[PATH_MAX];
    ssize_t n;

    snprintf(pathname, PATH_MAX, "/proc/%s/fd/%s", pid, fd);
    n = readlink(pathname, buffer, PATH_MAX);
    if (n == -1)
        return NULL;
    
    buffer[n] = '\0';
    return buffer;
}

static void printInfo(const char * pid, const char * fd)
{
    char pathname[PATH_MAX];
    FILE * file_pointer;
    char buffer[LINE_MAX];
    char name[LINE_MAX];

    snprintf(pathname, PATH_MAX, "/proc/%s/status", pid);
    if ((file_pointer = fopen(pathname, "r")) == NULL)
        return;

    while (fgets(buffer, LINE_MAX, file_pointer) != NULL)
        if (strncmp(buffer, "Name:", 5) == 0)
        {
            sscanf(buffer + 5, "%s", name);
            printf("%s(%s), fd: %s\n", name, pid, fd);
        }

    if (ferror(file_pointer) != 0)
        fputs("Error indicator set\n", stderr);

    if (fclose(file_pointer) != 0)
        fputs("fclose failed\n", stderr);
}
