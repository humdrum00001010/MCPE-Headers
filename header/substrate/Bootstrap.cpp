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

#include "CydiaSubstrate.h"

#include <unistd.h>

MSInitialize {
    if (getenv("MSExitZero") != NULL) {
        // skeels / planetbeing / rpetrich <- reporting SMS crash
        if (dlopen("/System/Library/PrivateFrameworks/Search.framework/AppIndexer", RTLD_LAZY | RTLD_NOLOAD) != NULL)
            _exit(EXIT_FAILURE);

        _exit(EXIT_SUCCESS);
    }

    // Skype
    if (dlopen("/System/Library/Frameworks/Security.framework/Security", RTLD_LAZY | RTLD_NOLOAD) == NULL)
        return;

    // Maps
    dlopen("/System/Library/Frameworks/CoreFoundation.framework/CoreFoundation", RTLD_LAZY | RTLD_GLOBAL);

    dlopen("/Library/Frameworks/CydiaSubstrate.framework/Libraries/SubstrateLoader.dylib", RTLD_LAZY | RTLD_GLOBAL);
}
