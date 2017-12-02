/* File: getpwnam.c
 * Author: ByXc
 * About: 使用 setpwent()、getpwent()、endpwent() 來實現 getpwnam()。
 * Version: 1.0
 * Compiler: gcc 7.1.0  for ubuntu 
 * Date: 20171202
 * Github: ByXc01
 * Blog: http://byxc01.github.io
 */

#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <errno.h>

/* Return a pointer on success, or NULL on error */
struct passwd * getpwnam_b(const char * name);

#ifdef TEST
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    struct passwd * password;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "Usage: %s [user name]\n", *argv);
        exit(EXIT_FAILURE);
    }

    if ((password = getpwnam_b(argv[1])) == NULL)
    {
        if (errno == 0)
            fputs("Not found\n", stderr);
        else
            puts(strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("%s:%s:%ld:%ld:%s:%s:%s\n", password->pw_name, password->pw_passwd,
                                       (long) password->pw_uid, (long) password->pw_gid, 
                                       password->pw_gecos, password->pw_dir, password->pw_shell);

    return 0;
}
#endif

struct passwd * getpwnam_b(const char * name)
{
    struct passwd * password;

    while ((password = getpwent()) != NULL)
        if (strcmp(name, password->pw_name) == 0)
        {
            errno = 0;
            break;
        }
    endpwent();

    return password;
}
