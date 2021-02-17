#include "../jaysheng/log.h"
#include "../jaysheng/scheduler.h"

static jaysheng::Logger::ptr g_logger = JAYSHENG_LOG_ROOT();

void test_fiber() {
    static int s_count = 5;
    JAYSHENG_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

    sleep(1);
    if (--s_count >= 0) {
        // jaysheng::Scheduler::GetThis()->scheduler(&test_fiber, jaysheng::GetThreadId());
        jaysheng::Scheduler::GetThis()->scheduler(&test_fiber);
    }
}

int main(int argc, const char **argv) {
    JAYSHENG_LOG_INFO(g_logger) << "main";
    jaysheng::Scheduler sc(3, false, "test");
    sleep(2);
    sc.start();
    // sleep(2);
    JAYSHENG_LOG_INFO(g_logger) << "schedule___________________";
    sc.scheduler(&test_fiber);
    sc.stop();
    JAYSHENG_LOG_INFO(g_logger) << "end";
    return 0;
}