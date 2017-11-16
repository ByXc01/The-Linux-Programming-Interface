#ifndef GET_NUM_H
#define GET_NUM_H

#define GN_NONNEG       01      /* Value must be >= 0                */
#define GN_GT_0         02      /* Value must be > 0                 */

                                /* By defalut, integers are decimal  */
#define GN_ANY_BASE     0100    /* Can use nay base - like strtol()  */
#define GN_BASE_8       0200    /* Value is expressed in octal       */
#define GN_BASE_16      0400    /* Value is expressed in hexadecimal */

long getLong(const char * arg, int flags, const char * name);

int getInt(const char * arg, int falgs, const char * name);

#endif
