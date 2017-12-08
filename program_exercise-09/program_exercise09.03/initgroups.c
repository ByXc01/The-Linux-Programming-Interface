/* File: initgroups.c
 * Author: ByXc
 * About: 模仿 initgroups() 函數
 * Version: 1.0
 * Compiler: gcc 7.1.0 for ubuntu
 * Date: 20171208
 * Github: ByXc01
 * Blog: http://ByXc01.github.io
 */

/* Ps: 進程必須享有特權 */

#define _BSD_SOURCE
#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

#define SG_SIZE (NGROUPS_MAX + 1)

/* Return 0 on success, or -1 on error */
int initgroups_b(const char * user, gid_t group_id);

/* 測試宏(gcc -DTEST), 使用程序清單 9.1 測試 */
#ifdef TEST

#include <sys/fsuid.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"

void show_group(int count, gid_t group_list[]);

int main(void)
{
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t group_list[SG_SIZE];
    int numGroups, count;
    char * p;

    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");
    if (getresgid(&rgid, &egid, &sgid) == -1)
        errExit("getresgid");

    /* Attemps to change the file-system IDs are always ignored
     * for unprivileged proccesses, but even so, the following
     * calls return the current file-system IDs
     */

    fsuid = setfsuid(0);
    fsgid = setfsgid(0);

    printf("UID: ");
    p = userNameFromId(ruid);
    printf("real = %s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
    p = userNameFromId(euid);
    printf("effective = %s (%ld); ", (p == NULL) ? "???" : p, (long) euid);
    p = userNameFromId(suid);
    printf("saved = %s (%ld); ", (p == NULL) ? "???" : p, (long) suid);
    p = userNameFromId(fsuid);
    printf("file-system = %s (%ld);\n", (p == NULL) ? "???" : p, (long) fsuid);
    
    printf("GID: ");
    p = groupNameFromId(rgid);
    printf("real = %s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
    p = groupNameFromId(egid);
    printf("effective = %s (%ld); ", (p == NULL) ? "???" : p, (long) egid);
    p = groupNameFromId(sgid);
    printf("saved = %s (%ld); ", (p == NULL) ? "???" : p, (long) sgid);
    p = groupNameFromId(fsgid);
    printf("file-system = %s (%ld);\n", (p == NULL) ? "???" : p, (long) fsgid);

    numGroups = getgroups(SG_SIZE, group_list);
    if (numGroups == -1)
        errExit("getgroups");
    
    printf("Supplementary groups (%d): ", numGroups);
    show_group(numGroups, group_list);

    p = userNameFromId(ruid);
    if (p == NULL)
        errExit("userNameFromId");
        
    initgroups_b(p, rgid);
    numGroups = getgroups(SG_SIZE, group_list);
    if (numGroups == -1)
        errExit("getgroups");

    printf("Initialized supplementary group(%d): ", numGroups);
    show_group(numGroups, group_list);

    return 0;
}

void show_group(int number, gid_t group_list[])
{
    int count;

    for (count = 0; count < number; count++)
    {
        p = groupNameFromId(group_list[count]);
        printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) group_list[count]);
    }
    puts("");
}

#endif

int initgroups_b(const char * user, gid_t group_id)
{
    gid_t group_list[SG_SIZE];
    struct group * grp;
    char ** mem;
    int gid_set_size = 0;

    while ((grp = getgrent()) != NULL)
        for (mem = grp->gr_mem; *mem != NULL; mem++)
            if (strcmp(*mem, user) == 0)
                group_list[gid_set_size++] = grp->gr_gid;
    endgrent();

    group_list[gid_set_size++] = group_id;
    if (setgroups(gid_set_size, group_list) == -1)
        return -1;

    return 0;
}

