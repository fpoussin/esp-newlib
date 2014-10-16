/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
FUNCTION
<<rename>>---rename a file

INDEX
	rename
INDEX
	_rename_r

ANSI_SYNOPSIS
	#include <stdio.h>
	int rename(const char *<[old]>, const char *<[new]>);

	int _rename_r(struct _reent *<[reent]>, 
                      const char *<[old]>, const char *<[new]>);

TRAD_SYNOPSIS
	#include <stdio.h>
	int rename(<[old]>, <[new]>)
	char *<[old]>;
	char *<[new]>;

	int _rename_r(<[reent]>, <[old]>, <[new]>)
	struct _reent *<[reent]>;
	char *<[old]>;
	char *<[new]>;

DESCRIPTION
Use <<rename>> to establish a new name (the string at <[new]>) for a
file now known by the string at <[old]>.  After a successful
<<rename>>, the file is no longer accessible by the string at <[old]>.

If <<rename>> fails, the file named <<*<[old]>>> is unaffected.  The
conditions for failure depend on the host operating system.

The alternate function <<_rename_r>> is a reentrant version.  The
extra argument <[reent]> is a pointer to a reentrancy structure.

RETURNS
The result is either <<0>> (when successful) or <<-1>> (when the file
could not be renamed).

PORTABILITY
ANSI C requires <<rename>>, but only specifies that the result on
failure be nonzero.  The effects of using the name of an existing file
as <<*<[new]>>> may vary from one implementation to another.

Supporting OS subroutines required: <<link>>, <<unlink>>, or <<rename>>.
*/

#include <_ansi.h>
#include <reent.h>
#include <stdio.h>
#include <sys/unistd.h>

#ifndef HAVE_RENAME_R

int
_DEFUN(_rename_r, (ptr, old, new),
       struct _reent *ptr _AND
       _CONST char *old   _AND
       _CONST char *new)
{
#ifdef HAVE_RENAME
  return _rename (old,new);
#else
  if (_link_r (ptr, old, new) == -1)
    return -1;

  if (_unlink_r (ptr, old) == -1)
    {
      /* ??? Should we unlink new? (rhetorical question) */
      return -1;
    }
#endif
  return 0;
}

#endif /* !HAVE_RENAME_R */

#ifndef _REENT_ONLY

int
_DEFUN(rename, (old, new),
       _CONST char *old _AND
       _CONST char *new)
{
  return _rename_r (_REENT, old, new);
}

#endif
