// Use of this source code is governed by a BSD-style license.
//
// Author: Huchao Bi (bihuchao at qq dot com)

#pragma once

#include <stdio.h>

namespace jsonlib {

// logging
#define LOG_BASE(severity, message) printf("[ %s ] : %s:%d - %s\n", \
    severity, __FILE__, __LINE__, message);

#define LOG_FATAL(message) do { \
    LOG_BASE("FATAL", message); \
    abort(); \
} while (false);

#define LOG_WARNING(message) LOG_BASE("WARNING", message)

}; // namespace jsonlib
