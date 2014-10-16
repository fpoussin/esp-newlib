/* times.c -- get process times
 *
 * Copyright (c) 2006 Tensilica.
 */
#include <xtensa/config/core.h>
#include <sys/times.h>
#include <errno.h>
#include "../glue.h"


/*
 * times --  Use the Xtensa processor CCOUNT register if available.
 */

#ifdef REENTRANT_SYSCALLS_PROVIDED

#include <sys/reent.h>

clock_t
_times_r (struct _reent *reent, struct tms *buf)
{
# if XCHAL_HAVE_CCOUNT
  clock_t clk;
  __asm__ ("rsr.ccount %0" : "=a" (clk));
  if (buf) {
    buf->tms_utime  = clk;
    buf->tms_stime  = 0;
    buf->tms_cutime = 0;
    buf->tms_cstime = 0;
  }
  return clk;
# else
  if (buf) {
    buf->tms_utime  = 0;
    buf->tms_stime  = 0;
    buf->tms_cutime = 0;
    buf->tms_cstime = 0;
  }
  reent->_errno = ENOSYS;
  return (clock_t) -1;
# endif
}

#endif /* REENTRANT_SYSCALLS_PROVIDED */

