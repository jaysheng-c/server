#ifndef __JAYSHENG_MACRO_H__
#define __JAYSHENG_MACRO_H__

#include "log.h"
#include "util.h"

#include <cassert>

#if defined __GNUC__ || defined __llvm__

/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#define JAYSHENG_LIKELY(x) __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#define JAYSHENG_UNLIKELY(x) __builtin_expect(!!(x), 0)

#else

#define JAYSHENG_LIKELY(x) (x)
#define JAYSHENG_UNLIKELY(x) (x)

#endif // __GNUC__ || __llvm__

// assert封装
#define JAYSHENG_ASSERT(x)                                                                      \
    if (JAYSHENG_UNLIKELY(!(x))) {                                                              \
        JAYSHENG_LOG_ERROR(JAYSHENG_LOG_ROOT()) << "ASSERTION: " #x                             \
                                                << "\nbacktrace:\n"                             \
                                                << jaysheng::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                              \
    }
#define JAYSHENG_ASSERT2(x, w)                                                                  \
    if (JAYSHENG_UNLIKELY(!(x))) {                                                              \
        JAYSHENG_LOG_ERROR(JAYSHENG_LOG_ROOT()) << "ASSERTION: " #x                             \
                                                << "\n"                                         \
                                                << w                                            \
                                                << "\nbacktrace:\n"                             \
                                                << jaysheng::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                              \
    }

#endif // !__JAYSHENG_MACRO_H__