/* sbrk.c -- allocate memory dynamically.
 * 
 * Copyright (c) 1995,1996 Cygnus Support
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */
#include <errno.h>
#include "glue.h"

/* just in case, most boards have at least some memory */
#ifndef RAMSIZE
#  define RAMSIZE             (caddr_t)0x100000
#endif

char *heap_ptr;

/* This is a variable so that we can override it.
   Coware crt run-time can do this.  */
extern char _heap_sentry;	/* Defined by the linker */
char *_heap_sentry_ptr = &_heap_sentry;

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */

#ifndef REENTRANT_SYSCALLS_PROVIDED

char *
sbrk (nbytes)
     int nbytes;
{
  char        *base;

  if (!heap_ptr)
    heap_ptr = (char *)&_end;
  base = heap_ptr;

  /* The heap is limited by _heap_sentry.
     We're out of memory if we exceed that.  */
  if (heap_ptr + nbytes >= _heap_sentry_ptr)
    {
      errno = ENOMEM;
      return (char *) -1;
    }

  heap_ptr += nbytes;

  return base;
}

#else /* REENTRANT_SYSCALLS_PROVIDED */

#include <sys/reent.h>

char *
_sbrk_r (ptr, nbytes)
     struct _reent *ptr;
     int nbytes;
{
  char *base;

  if (!heap_ptr)
    heap_ptr = (char *)&_end;
  base = heap_ptr;

  /* The heap is limited by _heap_sentry.
     We're out of memory if we exceed that.  */
  if (heap_ptr + nbytes >= _heap_sentry_ptr)
    {
      ptr->_errno = ENOMEM;
      return (char *) -1;
    }

  heap_ptr += nbytes;

  return base;
}

#endif /* REENTRANT_SYSCALLS_PROVIDED */
