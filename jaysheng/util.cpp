#include "util.h"
#include "log.h"

#include <cxxabi.h>
#include <execinfo.h>

namespace jaysheng {

static jaysheng::Logger::ptr g_logger = JAYSHENG_LOG_NAME("system");

static std::string demangle(const char *str) {
    size_t size = 0;
    int status = 0;
    std::string rt;
    rt.resize(256);
    if (1 == sscanf(str, "%*[^(]%*[^_]%255[^)+]", &rt[0])) {
        char *v = abi::__cxa_demangle(&rt[0], nullptr, &size, &status);
        if (v) {
            std::string result(v);
            free(v);
            return result;
        }
    }
    if (1 == sscanf(str, "%255s", &rt[0])) {
        return rt;
    }
    return str;
}

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

void Backtrace(std::vector<std::string> &bt, int size, int skip) {
    void **array = (void **)malloc((sizeof(void *) * size));
    size_t s = ::backtrace(array, size);

    char **strings = backtrace_symbols(array, s);
    if (strings == NULL) {
        JAYSHENG_LOG_ERROR(g_logger) << "backtrace_symbols error.";
        return;
    }
    for (size_t i = skip; i < s; ++i) {
        bt.push_back(demangle(strings[i]));
    }

    free(strings);
    free(array);
}

std::string BacktraceToString(int size, int skip, const std::string &prefix) {
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for (size_t i = 0; i < bt.size(); ++i) {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}

} // namespace jaysheng