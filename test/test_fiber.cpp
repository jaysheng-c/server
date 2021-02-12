#include "../jaysheng/fiber.h"
#include "../jaysheng/log.h"
#include "../jaysheng/thread.h"

#include <vector>

jaysheng::Logger::ptr g_logger = JAYSHENG_LOG_ROOT();

void run_in_fiber() {
    JAYSHENG_LOG_INFO(g_logger) << "run_in_fiber begin";
    jaysheng::Fiber::YieldToHold();
    JAYSHENG_LOG_INFO(g_logger) << "run_in_fiber end";
    jaysheng::Fiber::YieldToHold();
}

void test_fiber() {
    // JAYSHENG_LOG_INFO(g_logger) << "main begin";
    {
        auto cur = jaysheng::Fiber::GetThis();
        // JAYSHENG_LOG_INFO(g_logger) << "main begin 2";
        jaysheng::Fiber::ptr fiber(new jaysheng::Fiber(run_in_fiber));
        fiber->swapIn();
        // JAYSHENG_LOG_INFO(g_logger) << "main after swapIn";
        // fiber->swapIn();
        // JAYSHENG_LOG_INFO(g_logger) << "main after end";
        // fiber->swapIn();
    }
    // JAYSHENG_LOG_INFO(g_logger) << "main after end 2";
}

static int count = 3;
int main(int argc, char const *argv[]) {
    jaysheng::Thread::SetName("main");
    std::vector<jaysheng::Thread::ptr> thrs;
    for (int i = 0; i < count; ++i) {
        thrs.push_back(jaysheng::Thread::ptr(
            new jaysheng::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for (auto i : thrs) {
        i->join();
    }
    return 0;
}
