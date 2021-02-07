#include "util.h"

namespace jaysheng {

unsigned int GetThreadId() {
    return syscall(SYS_gettid);
}

unsigned int GetFiberId() {
    //#ifdef WIN32
    //		return GetCurrentThreadId();
    //#else
    //		//return pthread_self();
    //		return syscall(SYS_gettid);
    //#endif // WIN32
    return 0;
}

} // namespace jaysheng