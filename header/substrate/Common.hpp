/* Cydia Substrate - Powerful Code Insertion Platform
 * Copyright (C) 2008-2011  Jay Freeman (saurik)
*/

/* GNU Lesser General Public License, Version 3 {{{ */
/*
 * Substrate is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Substrate is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Substrate.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */

#ifndef SUBSTRATE_COMMON_HPP
#define SUBSTRATE_COMMON_HPP

#include <errno.h>
#include "Log.hpp"

#define _syscall(expr) ({ \
    __typeof__(expr) _value; \
    for(;;) if ((long) (_value = (expr)) != -1 || errno != EINTR) \
        break; \
    _value; \
})

#define _trace() do { \
    MSLog(MSLogLevelError, "%s(%d): _trace()", __FILE__, __LINE__); \
} while (false)

#endif//SUBSTRATE_COMMON_HPP
