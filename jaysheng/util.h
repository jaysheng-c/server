#ifndef __JAYSHENG_UTIL_H__
#define __JAYSHENG_UTIL_H__

namespace jaysheng {

#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

unsigned int GetThreadId();
unsigned int GetFiberId();
} // namespace jaysheng

#endif // !__JAYSHENG_UTIL_H__
