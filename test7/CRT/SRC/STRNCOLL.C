/***
*strncoll.c - Collate locale strings
*
*       Copyright (c) 1994-1998, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       Compares at most n characters of two strings.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <errno.h>
#include <awint.h>
#endif  /* _WIN32 */

/***
*int _strncoll() - Collate locale strings
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       Compares at most n characters of two strings.
*
*Entry:
*       const char *s1 = pointer to the first string
*       const char *s2 = pointer to the second string
*       size_t count - maximum number of characters to compare
*
*Exit:
*       Less than 0    = first string less than second string
*       0              = strings are equal
*       Greater than 0 = first string greater than second string
*
*Exceptions:
*       _NLSCMPERROR    = error
*       errno = EINVAL
*
*******************************************************************************/

int __cdecl _strncoll (
        const char *_string1,
        const char *_string2,
        size_t count
        )
{
#if defined (_WIN32)

        int ret;
#if defined (_MT)
        int local_lock_flag;
#endif  /* defined (_MT) */

        if (!count)
            return 0;

        if (__lc_handle[LC_COLLATE] == _CLOCALEHANDLE) {
                return strncmp(_string1, _string2, count);
        }

        _lock_locale( local_lock_flag )

#if defined (_MT)
        if (__lc_handle[LC_COLLATE] == _CLOCALEHANDLE) {
                _unlock_locale( local_lock_flag )
                return strncmp(_string1, _string2, count);
        }
#endif  /* defined (_MT) */

        if ( 0 == (ret = __crtCompareStringA( __lc_handle[LC_COLLATE],
                                              SORT_STRINGSORT,
                                              _string1,
                                              count,
                                              _string2,
                                              count,
                                              __lc_collate_cp )) )
            goto error_cleanup;

        _unlock_locale( local_lock_flag )
        return (ret - 2);

error_cleanup:
        _unlock_locale( local_lock_flag )
        errno = EINVAL;
        return _NLSCMPERROR;

#else  /* defined (_WIN32) */

        return strncmp(_string1, _string2, count);

#endif  /* defined (_WIN32) */
}
