/*
 * getifaddrs.c
 * Replacement for getifaddrs() function.
 *
 * Copyright (c) 2002, 2003 University of Utah and the Flux Group.
 * All rights reserved.
 *
 * This file is licensed under the terms of the GNU Public License.
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * Contributed by the Flux Research Group, Department of Computer Science,
 * University of Utah, http://www.cs.utah.edu/flux/
 */

#include "config.h"

#if defined(HAVE_GETIFADDRS)

#elif defined(linux)

#if !defined(__set_errno)
#define __set_errno(x) errno = x
#endif

#if !defined(__close)
#define __close(x) close(x)
#endif

#include "ifaddrs_linux.c"

#else

#include <errno.h>
#include <sys/types.h>
#include <ifaddrs.h>

int getifaddrs(struct ifaddrs **ifap)
{
    errno = ENOSYS;
    return -1;
}

void freeifaddrs(struct ifaddrs *ifp)
{
}

#endif
