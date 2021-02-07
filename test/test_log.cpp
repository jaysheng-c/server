#include "jaysheng/log.h"
#include "jaysheng/util.h"
#include <iostream>

int main(int argc, char **argv) {
    jaysheng::Logger::ptr logger(new jaysheng::Logger);
    logger->addAppender(jaysheng::LogAppender::ptr(new jaysheng::StdoutLogAppender));

    jaysheng::FileLogAppender::ptr file_appender(new jaysheng::FileLogAppender("./log.txt"));
    jaysheng::LogFormatter::ptr fmt(new jaysheng::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(jaysheng::LogLevel::ERROR);

    logger->addAppender(file_appender);

    //jaysheng::LogEvent::ptr event(new jaysheng::LogEvent(__FILE__, __LINE__, 0, jaysheng::GetThreadId(), jaysheng::GetFiberId(), time(0)));
    //event->getSS() << "hello jaysheng log";
    //logger->log(jaysheng::LogLevel::DEBUG, event);
    std::cout << "hello jaysheng log" << std::endl;

    JAYSHENG_LOG_INFO(logger) << "test macro";
    JAYSHENG_LOG_ERROR(logger) << "test macro error";

    JAYSHENG_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = jaysheng::LoggerMgr::GetInstance()->getLogger("xx");
    JAYSHENG_LOG_INFO(l) << "xxx";
    return 0;
}
