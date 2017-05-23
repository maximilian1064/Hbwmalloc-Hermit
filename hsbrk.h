#include <_ansi.h>
#include <reent.h>
//#include <_syslist.h>
#include <errno.h>
#include <hermit/syscall.h>

void*
_DEFUN (_hbmem_sbrk_r, (ptr, incr),
	struct _reent *ptr _AND
	ptrdiff_t incr)
{
	ssize_t ret = sys_hbmem_sbrk(incr);
	if (ret <= 0) {
		ptr->_errno = ENOMEM;
		ret = 0;
	}

	return (void*) ret;
}

void*
_DEFUN (hbmem_sbrk, (incr),
	ptrdiff_t incr)
{
	return _hbmem_sbrk_r(_REENT, incr);
}
