/* File: name_uid.c
 * Author: ByXc
 * About: get name and uid
 * Version: 2.0
 * Compiler: gcc 7.10 for ubuntu
 * Date: 20171224
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

#include <dirent.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"

#define MAX_LINE 100

static bool check_pid(const char * numeric);
static void print_name_uid(uid_t uid, const char * filename);

int main(int argc, char * argv[])
{
    DIR * dir_stream;
    struct dirent * dir;
    uid_t uid;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) usageErr("%s username\n", argv[0]);

    uid = userIdFromName(argv[1]);
    if (uid == -1)
        cmdLineErr("Bad username: %s\n", argv[1]);

    if ((dir_stream = opendir("/proc/")) == NULL)
        errExit("opendir");

    while (1)
    {
        errno = 0;
        dir = readdir(dir_stream);
        if (dir == NULL)
            break;

        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        if (check_pid(dir->d_name))
            print_name_uid(uid, dir->d_name);
    }

    if (errno != 0)
        errExit("readdir");

    if (closedir(dir_stream) == -1)
    errMsg("closedir");

    return 0;
}

static bool check_pid(const char * numeric)
{
    int file_descriptor;
    char * endptr, * find, line[MAX_LINE];
    ssize_t n;
    pid_t pid, pid_max;
    bool flag = true;;

    file_descriptor = open("/proc/sys/kernel/pid_max", O_RDONLY);
    if (file_descriptor == -1)
        errExit("open");

    n = read(file_descriptor, line, MAX_LINE - 1);
    if (n == -1)
        errExit("read");

    find = strchr(line, '\n');                                      // 讀取的時候可能包含換行符
    if (find != NULL)
        *find = '\0';

    pid_max = (pid_t) strtol(line, &endptr, 0);
    if (*endptr != '\0')
        errExit("strtol() pid_max"); 

    pid = (pid_t) strtol(numeric, &endptr, 0);                             
    if (*endptr != '\0')
        flag = false;                                               // 不是進程號

    if (close(file_descriptor) == -1)
        errExit("close");

    flag = pid <= pid_max && flag ? true : false;

    return flag;
}

static void print_name_uid(uid_t uid, const char * filename)
{
    FILE * file_pointer;
    long int ruid;
    char path[PATH_MAX];
    char line[MAX_LINE], name[MAX_LINE];

    sprintf(path, "/proc/%s/status", filename);                     // 完整路徑
    if ((file_pointer = fopen(path, "r")) == NULL)
    {
        errMsg("The process is over, pid: %s\n", filename);         // 打開失敗即視爲進程已結束
        return;
    }

    while (fgets(line, MAX_LINE,file_pointer))
    {
        if (strncmp(line, "Name:", 5) == 0)
            sprintf(name, "%s", line);

        if (strncmp(line, "Uid:", 4) == 0)
        {
            sscanf(line + 4, "%ld", &ruid);                         /* 因爲 Uid 的格式是: Uid:  0   0   0   0
                                                                     * 我們要跳過前面的 UID: ，後面的空白符靠
                                                                     * sscanf() 來跳過
                                                                     */
            if (uid == (uid_t) ruid)                                // 用進程的真實用戶ID作比較
            {
                printf("%s", name, line);
                printf("Pid: ")
            }

            break;
        }
    }

    if (fclose(file_pointer) != 0)
        errExit("fclose");
}
