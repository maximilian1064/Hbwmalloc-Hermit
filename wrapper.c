/* VxWorks provides its own version of malloc, and we can't use this
   one because VxWorks does not provide sbrk.  So we have a hook to
   not compile this code.  */

/* The routines here are simple cover fns to the routines that do the real
   work (the reentrant versions).  */
/* FIXME: Does the warning below (see WARNINGS) about non-reentrancy still
   apply?  A first guess would be "no", but how about reentrancy in the *same*
   thread?  */

#ifdef MALLOC_PROVIDED

int _dummy_malloc = 1;

#else

////////////////////////
///  1-mallocs-1  //////
////////////////////////


/*
FUNCTION
<<malloc>>, <<realloc>>, <<free>>---manage memory

INDEX
	malloc
INDEX
	realloc
INDEX
	reallocf
INDEX
	free
INDEX
	memalign
INDEX
	malloc_usable_size
INDEX
	_malloc_r
INDEX
	_realloc_r
INDEX
	_reallocf_r
INDEX
	_free_r
INDEX
	_memalign_r
INDEX
	_malloc_usable_size_r

ANSI_SYNOPSIS
	#include <stdlib.h>
	void *malloc(size_t <[nbytes]>);
	void *realloc(void *<[aptr]>, size_t <[nbytes]>);
	void *reallocf(void *<[aptr]>, size_t <[nbytes]>);
	void free(void *<[aptr]>);

	void *memalign(size_t <[align]>, size_t <[nbytes]>);

	size_t malloc_usable_size(void *<[aptr]>);

	void *_malloc_r(void *<[reent]>, size_t <[nbytes]>);
	void *_realloc_r(void *<[reent]>, 
                         void *<[aptr]>, size_t <[nbytes]>);
	void *_reallocf_r(void *<[reent]>, 
                         void *<[aptr]>, size_t <[nbytes]>);
	void _free_r(void *<[reent]>, void *<[aptr]>);

	void *_memalign_r(void *<[reent]>,
			  size_t <[align]>, size_t <[nbytes]>);

	size_t _malloc_usable_size_r(void *<[reent]>, void *<[aptr]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	char *malloc(<[nbytes]>)
	size_t <[nbytes]>;

	char *realloc(<[aptr]>, <[nbytes]>)
	char *<[aptr]>;
	size_t <[nbytes]>;

	char *reallocf(<[aptr]>, <[nbytes]>)
	char *<[aptr]>;
	size_t <[nbytes]>;

	void free(<[aptr]>)
	char *<[aptr]>;

	char *memalign(<[align]>, <[nbytes]>)
	size_t <[align]>;
	size_t <[nbytes]>;

	size_t malloc_usable_size(<[aptr]>)
	char *<[aptr]>;

	char *_malloc_r(<[reent]>,<[nbytes]>)
	char *<[reent]>;
	size_t <[nbytes]>;

	char *_realloc_r(<[reent]>, <[aptr]>, <[nbytes]>)
	char *<[reent]>;
	char *<[aptr]>;
	size_t <[nbytes]>;

	char *_reallocf_r(<[reent]>, <[aptr]>, <[nbytes]>)
	char *<[reent]>;
	char *<[aptr]>;
	size_t <[nbytes]>;

	void _free_r(<[reent]>, <[aptr]>)
	char *<[reent]>;
	char *<[aptr]>;

	char *_memalign_r(<[reent]>, <[align]>, <[nbytes]>)
	char *<[reent]>;
	size_t <[align]>;
	size_t <[nbytes]>;

	size_t malloc_usable_size(<[reent]>, <[aptr]>)
	char *<[reent]>;
	char *<[aptr]>;

DESCRIPTION
These functions manage a pool of system memory.

Use <<malloc>> to request allocation of an object with at least
<[nbytes]> bytes of storage available.  If the space is available,
<<malloc>> returns a pointer to a newly allocated block as its result.

If you already have a block of storage allocated by <<malloc>>, but
you no longer need all the space allocated to it, you can make it
smaller by calling <<realloc>> with both the object pointer and the
new desired size as arguments.  <<realloc>> guarantees that the
contents of the smaller object match the beginning of the original object.

Similarly, if you need more space for an object, use <<realloc>> to
request the larger size; again, <<realloc>> guarantees that the
beginning of the new, larger object matches the contents of the
original object.

When you no longer need an object originally allocated by <<malloc>>
or <<realloc>> (or the related function <<calloc>>), return it to the
memory storage pool by calling <<free>> with the address of the object
as the argument.  You can also use <<realloc>> for this purpose by
calling it with <<0>> as the <[nbytes]> argument.

The <<reallocf>> function behaves just like <<realloc>> except if the
function is required to allocate new storage and this fails.  In this
case <<reallocf>> will free the original object passed in whereas
<<realloc>> will not.

The <<memalign>> function returns a block of size <[nbytes]> aligned
to a <[align]> boundary.  The <[align]> argument must be a power of
two.

The <<malloc_usable_size>> function takes a pointer to a block
allocated by <<malloc>>.  It returns the amount of space that is
available in the block.  This may or may not be more than the size
requested from <<malloc>>, due to alignment or minimum size
constraints.

The alternate functions <<_malloc_r>>, <<_realloc_r>>, <<_reallocf_r>>, 
<<_free_r>>, <<_memalign_r>>, and <<_malloc_usable_size_r>> are reentrant
versions.  The extra argument <[reent]> is a pointer to a reentrancy structure.

If you have multiple threads of execution which may call any of these
routines, or if any of these routines may be called reentrantly, then
you must provide implementations of the <<__malloc_lock>> and
<<__malloc_unlock>> functions for your system.  See the documentation
for those functions.

These functions operate by calling the function <<_sbrk_r>> or
<<sbrk>>, which allocates space.  You may need to provide one of these
functions for your system.  <<_sbrk_r>> is called with a positive
value to allocate more space, and with a negative value to release
previously allocated space if it is no longer required.
@xref{Stubs}.

RETURNS
<<malloc>> returns a pointer to the newly allocated space, if
successful; otherwise it returns <<NULL>>.  If your application needs
to generate empty objects, you may use <<malloc(0)>> for this purpose.

<<realloc>> returns a pointer to the new block of memory, or <<NULL>>
if a new block could not be allocated.  <<NULL>> is also the result
when you use `<<realloc(<[aptr]>,0)>>' (which has the same effect as
`<<free(<[aptr]>)>>').  You should always check the result of
<<realloc>>; successful reallocation is not guaranteed even when
you request a smaller object.

<<free>> does not return a result.

<<memalign>> returns a pointer to the newly allocated space.

<<malloc_usable_size>> returns the usable size.

PORTABILITY
<<malloc>>, <<realloc>>, and <<free>> are specified by the ANSI C
standard, but other conforming implementations of <<malloc>> may
behave differently when <[nbytes]> is zero.

<<memalign>> is part of SVR4.

<<malloc_usable_size>> is not portable.

Supporting OS subroutines required: <<sbrk>>.  */


////////////////////////
///  2-calloc-2  ///////
////////////////////////


/*
FUNCTION
<<calloc>>---allocate space for arrays

INDEX
	calloc

INDEX
	_calloc_r

ANSI_SYNOPSIS
	#include <stdlib.h>
	void *calloc(size_t <[n]>, size_t <[s]>);
	void *_calloc_r(void *<[reent]>, size_t <[n]>, size_t <[s]>);
	
TRAD_SYNOPSIS
	#include <stdlib.h>
	char *calloc(<[n]>, <[s]>)
	size_t <[n]>, <[s]>;

	char *_calloc_r(<[reent]>, <[n]>, <[s]>)
	char *<[reent]>;
	size_t <[n]>;
	size_t <[s]>;



DESCRIPTION
Use <<calloc>> to request a block of memory sufficient to hold an
array of <[n]> elements, each of which has size <[s]>.

The memory allocated by <<calloc>> comes out of the same memory pool
used by <<malloc>>, but the memory block is initialized to all zero
bytes.  (To avoid the overhead of initializing the space, use
<<malloc>> instead.)

The alternate function <<_calloc_r>> is reentrant.
The extra argument <[reent]> is a pointer to a reentrancy structure.

RETURNS
If successful, a pointer to the newly allocated space.

If unsuccessful, <<NULL>>.

PORTABILITY
<<calloc>> is ANSI.

Supporting OS subroutines required: <<close>>, <<fstat>>, <<isatty>>,
<<lseek>>, <<read>>, <<sbrk>>, <<write>>.
*/

/////////////////////////////////
/// 3-HEADERS OF REALLOCF()-3 ///
/////////////////////////////////

/*-
 * Copyright (c) 1998, M. Warner Losh <imp@freebsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

////////////////////////
///  4-mallinfos-4  ////
////////////////////////

/*
FUNCTION
<<mallinfo>>, <<malloc_stats>>, <<mallopt>>---malloc support

INDEX
	mallinfo
INDEX
	malloc_stats
INDEX
	mallopt
INDEX
	_mallinfo_r
INDEX
	_malloc_stats_r
INDEX
	_mallopt_r

ANSI_SYNOPSIS
	#include <malloc.h>
	struct mallinfo mallinfo(void);
	void malloc_stats(void);
	int mallopt(int <[parameter]>, <[value]>);

	struct mallinfo _mallinfo_r(void *<[reent]>);
	void _malloc_stats_r(void *<[reent]>);
	int _mallopt_r(void *<[reent]>, int <[parameter]>, <[value]>);

TRAD_SYNOPSIS
	#include <malloc.h>
	struct mallinfo mallinfo();

	void malloc_stats();

	int mallopt(<[parameter]>, <[value]>)
	int <[parameter]>;
	int <[value]>;

	struct mallinfo _mallinfo_r(<[reent]>);
	char *<[reent]>;

	void _malloc_stats_r(<[reent]>);
	char *<[reent]>;

	int _mallopt_r(<[reent]>, <[parameter]>, <[value]>)
	char *<[reent]>;
	int <[parameter]>;
	int <[value]>;

DESCRIPTION
<<mallinfo>> returns a structure describing the current state of
memory allocation.  The structure is defined in malloc.h.  The
following fields are defined: <<arena>> is the total amount of space
in the heap; <<ordblks>> is the number of chunks which are not in use;
<<uordblks>> is the total amount of space allocated by <<malloc>>;
<<fordblks>> is the total amount of space not in use; <<keepcost>> is
the size of the top most memory block.

<<malloc_stats>> print some statistics about memory allocation on
standard error.

<<mallopt>> takes a parameter and a value.  The parameters are defined
in malloc.h, and may be one of the following: <<M_TRIM_THRESHOLD>>
sets the maximum amount of unused space in the top most block before
releasing it back to the system in <<free>> (the space is released by
calling <<_sbrk_r>> with a negative argument); <<M_TOP_PAD>> is the
amount of padding to allocate whenever <<_sbrk_r>> is called to
allocate more space.

The alternate functions <<_mallinfo_r>>, <<_malloc_stats_r>>, and
<<_mallopt_r>> are reentrant versions.  The extra argument <[reent]>
is a pointer to a reentrancy structure.

RETURNS
<<mallinfo>> returns a mallinfo structure.  The structure is defined
in malloc.h.

<<malloc_stats>> does not return a result.

<<mallopt>> returns zero if the parameter could not be set, or
non-zero if it could be set.

PORTABILITY
<<mallinfo>> and <<mallopt>> are provided by SVR4, but <<mallopt>>
takes different parameters on different systems.  <<malloc_stats>> is
not portable.

*/


#include <_ansi.h>
#include <reent.h>
#include <stdlib.h>
#include "malloc.h"

#include <string.h>
#include <sys/cdefs.h>
#include <stdio.h>

#if 0
/* _reallocf_r */

_PTR
_DEFUN (_reallocf_r, (reentptr, ptr, size),
	struct _reent *reentptr _AND
	_PTR ptr _AND
	size_t size)
{
	void *nptr;

	nptr = _realloc_r(reentptr, ptr, size);
	if (!nptr && ptr)
		_free_r(reentptr, ptr);
	return (nptr);
}
#endif

/* _mstats_r */

#if !defined (_ELIX_LEVEL) || _ELIX_LEVEL >= 2

/* mstats is now compatibility code.  It used to be real, for a
   previous version of the malloc routines.  It now just calls
   malloc_stats.  */

void
_DEFUN (_hbw_mstats_r, (ptr, s),
	struct _reent *ptr _AND
	char *s)
{
  _REENT_SMALL_CHECK_INIT(ptr);
  fiprintf (_stderr_r (ptr), "Memory allocation statistics %s\n", s);
  _malloc_stats_r (ptr);
}

#endif /* !_ELIX_LEVEL || _ELIX_LEVEL >= 2 */


#ifndef _REENT_ONLY

/* wrapper for malloc/free */

_PTR
_DEFUN (hbw_malloc, (nbytes),
	size_t nbytes)		/* get a block */
{
  return _malloc_r (_REENT, nbytes);
}

void
_DEFUN (hbw_free, (aptr),
	_PTR aptr)
{
  _free_r (_REENT, aptr);
}

/* wrapper for realloc */

_PTR
_DEFUN (hbw_realloc, (ap, nbytes),
	_PTR ap _AND
	size_t nbytes)
{
  return _realloc_r (_REENT, ap, nbytes);
}

/* wrapper for calloc */

_PTR
_DEFUN (hbw_calloc, (n, size),
	size_t n _AND
	size_t size)
{
  return _calloc_r (_REENT, n, size);
}

/* wrapper for reallocf */
#if 0
_PTR
_DEFUN (reallocf, (ptr, size),
	_PTR ptr _AND
	size_t size)
{
  return _reallocf_r(_REENT, ptr, size);
}
#endif

/* wrapper for memalign */

_PTR
_DEFUN (hbw_memalign, (align, nbytes),
	size_t align _AND
	size_t nbytes)
{
  return _memalign_r (_REENT, align, nbytes);
}

/* wrapper for mallinfo, malloc_stats, mallopt */

struct mallinfo
_DEFUN_VOID (hbw_mallinfo)
{
  return _mallinfo_r (_REENT);
}

#if !defined (_ELIX_LEVEL) || _ELIX_LEVEL >= 2
void
_DEFUN_VOID (hbw_malloc_stats)
{
  _malloc_stats_r (_REENT);
}

int
_DEFUN (hbw_mallopt, (p, v),
	int p _AND
	int v)
{
  return _mallopt_r (_REENT, p, v);
}

#endif /* !_ELIX_LEVEL || _ELIX_LEVEL >= 2 */

/* wrapper for mstats */

#if !defined (_ELIX_LEVEL) || _ELIX_LEVEL >= 2

void
_DEFUN (hbw_mstats, (s),
	char *s)
{
  _hbw_mstats_r (_REENT, s);
}

#endif /* !_ELIX_LEVEL || _ELIX_LEVEL >= 2 */

/* wrapper for malloc_usable_size */

size_t
_DEFUN (hbw_malloc_usable_size, (ptr),
	_PTR ptr)
{
  return _malloc_usable_size_r (_REENT, ptr);
}

/* wrapper for vallocs */

_PTR
_DEFUN (hbw_valloc, (nbytes),
	size_t nbytes)
{
  return _valloc_r (_REENT, nbytes);
}

_PTR
_DEFUN (hbw_pvalloc, (nbytes),
	size_t nbytes)
{
  return _pvalloc_r (_REENT, nbytes);
}

/* wrapper for malloc_trim */

int
_DEFUN (hbw_malloc_trim, (pad),
	size_t pad)
{
  return _malloc_trim_r (_REENT, pad);
}



#endif /* _REENT_ONLY */

#endif /* ! defined (MALLOC_PROVIDED) */


