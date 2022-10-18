/*
 * On Linux, mode_t is 2 bytes. On z/OS, it is 4.
 * Make it 2 for rsync. This header HAS to be used
 * in conjunction with modes.h. Both together are used
 * to map a 16 bit mode_t from rsync to/from a 32 bit mode_t for z/OS
 */
#if !defined(__mode_t)
          #define __mode_t  1
          typedef unsigned short mode_t ;
#endif

#include_next <sys/types.h>

