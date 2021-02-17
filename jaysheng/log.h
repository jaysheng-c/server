
/************************************
@brief:			日志系统
@file:			log.h
@version:
@company:
@author: 		jaysheng
@email:
@create_time:	
@update_time:	2020-05-29 21:08

2020-05-29 21:08 : 添加宏定义JAYSHENG_LOG_NAME(name), 修改LoggerManager::getLogger(std::string&) 
					使其能够创建一个logger, 修改Logger::log, 修改NameFormatItem, 实现LoggerManager::init()
					

************************************/

#ifndef __JAYSHENG_LOG_H__
#define __JAYSHENG_LOG_H__

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdarg.h>
#include <stdint.h>
#include <string>
#include <vector>

#include <functional>
#include <tuple>

#include "singleton.h"
#include "thread.h"
#include "util.h"

#define JAYSHENG_LOG_LEVEL(logger, level)                                                                                                 \
    if (logger->getLevel() <= level)                                                                                                      \
    jaysheng::LogEventWrap(jaysheng::LogEvent::ptr(new jaysheng::LogEvent(logger, level,                                                  \
                                                                          __FILE__, __LINE__, 0, jaysheng::GetThreadId(),                 \
                                                                          jaysheng::GetFiberId(), time(0), jaysheng::Thread::GetName()))) \
        .getSS()

#define JAYSHENG_LOG_DEBUG(logger) JAYSHENG_LOG_LEVEL(logger, jaysheng::LogLevel::DEBUG)
#define JAYSHENG_LOG_INFO(logger) JAYSHENG_LOG_LEVEL(logger, jaysheng::LogLevel::INFO)
#define JAYSHENG_LOG_WARN(logger) JAYSHENG_LOG_LEVEL(logger, jaysheng::LogLevel::WARN)
#define JAYSHENG_LOG_ERROR(logger) JAYSHENG_LOG_LEVEL(logger, jaysheng::LogLevel::ERROR)
#define JAYSHENG_LOG_FATAL(logger) JAYSHENG_LOG_LEVEL(logger, jaysheng::LogLevel::FATAL)

#define JAYSHENG_LOG_FMT_LEVEL(logger, level, fmt, ...)                                                                                   \
    if (logger->getLevel() <= level)                                                                                                      \
    jaysheng::LogEventWrap(jaysheng::LogEvent::ptr(new jaysheng::LogEvent(logger, level, __FILE__, __LINE__, 0, jaysheng::GetThreadId(),  \
                                                                          jaysheng::GetFiberId(), time(0), jaysheng::Thread::GetName()))) \
        .getEvent()                                                                                                                       \
        ->format(fmt, __VA_ARGS__)

#define JAYSHENG_LOG_FMT_DEBUG(logger, fmt, ...) JAYSHENG_LOG_FMT_LEVEL(logger, jaysheng::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define JAYSHENG_LOG_FMT_INFO(logger, fmt, ...) JAYSHENG_LOG_FMT_LEVEL(logger, jaysheng::LogLevel::INFO, fmt, __VA_ARGS__)
#define JAYSHENG_LOG_FMT_WARN(logger, fmt, ...) JAYSHENG_LOG_FMT_LEVEL(logger, jaysheng::LogLevel::WARN, fmt, __VA_ARGS__)
#define JAYSHENG_LOG_FMT_ERROR(logger, fmt, ...) JAYSHENG_LOG_FMT_LEVEL(logger, jaysheng::LogLevel::ERROR, fmt, __VA_ARGS__)
#define JAYSHENG_LOG_FMT_FATAL(logger, fmt, ...) JAYSHENG_LOG_FMT_LEVEL(logger, jaysheng::LogLevel::FATAL, fmt, __VA_ARGS__)

#define JAYSHENG_LOG_ROOT() jaysheng::LoggerMgr::GetInstance()->getRoot()
#define JAYSHENG_LOG_NAME(name) jaysheng::LoggerMgr::GetInstance()->getLogger(name)

namespace jaysheng {

class Logger;
class LoggerManager;

// 日志级别
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char *ToString(LogLevel::Level level);
    static LogLevel::Level FromString(const std::string &str);
};

// 日志事件
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;

    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
             const char *file, int line, unsigned int elapse,
             unsigned int thread_id, unsigned int fiber_id,
             unsigned int time, const std::string &thread_name);
    //~LogEvent();
    const char *getFile() const { return m_file; }
    int getLine() const { return m_line; }
    unsigned int getElapse() const { return m_elapse; }
    unsigned int getThreadId() const { return m_threadId; }
    std::string getThreadName() const { return m_threadName; }
    unsigned int getFiberId() const { return m_fiberId; }
    unsigned int getTime() const { return m_time; }
    std::string getContent() const { return m_ss.str(); }
    std::shared_ptr<Logger> getLogger() const { return m_logger; }
    LogLevel::Level getLevel() const { return m_level; }

    std::stringstream &getSS() { return m_ss; }
    void format(const char *fmt, ...);
    void foramt1(const char *fmt, va_list al);

private:
    const char *m_file = nullptr; // 文件名
    int m_line = 0;               // 行号
    unsigned int m_elapse = 0;    // 程序启动开始到现在的毫秒数
    unsigned int m_threadId = 0;  // 线程id
    unsigned int m_fiberId = 0;   // 协程id
    unsigned int m_time;          // 时间记录
    std::string m_threadName;
    std::stringstream m_ss;

    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class LogEventWrap {
public:
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap(); // 析构时调用Logger中的log打印内容
    std::stringstream &getSS();
    LogEvent::ptr getEvent() const { return m_event; }

private:
    LogEvent::ptr m_event;
};

// 日志格式器
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string &pattern);
    //~LogFormatter();
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr evnet);

public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem() {}
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };
    void init();

    bool isError() const { return m_error; }
    const std::string getPattern() const { return m_pattern; }

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error = false;
};

// 日志输出器
class LogAppender {
    friend class Logger;

public:
    typedef std::shared_ptr<LogAppender> ptr;
    //LogAppender();
    virtual ~LogAppender() {}

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    virtual std::string toYamlString() = 0;

    void setFormatter(LogFormatter::ptr val);
    LogFormatter::ptr getFormatter() const { return m_formatter; }
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    bool m_hasFormatter = false;
    LogFormatter::ptr m_formatter;
};

// 日志器
class Logger : public std::enable_shared_from_this<Logger> {
    friend class LoggerManager;

public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string &name = "root");
    //~Logger();

    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppenders();
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

    std::string &getName() { return m_name; }

    void setFormatter(LogFormatter::ptr val);
    void setFormatter(const std::string &val);
    LogFormatter::ptr getFormatter();

    std::string toYamlString();

private:
    std::string m_name; // 日志名称
    LogLevel::Level m_level;
    std::list<LogAppender::ptr> m_appenders; // Appender集合
    LogFormatter::ptr m_formatter;
    Logger::ptr m_root;
};

// 输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYamlString() override;

private:
};

// 输出到文件的Appender
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string &filename);
    ~FileLogAppender();
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYamlString() override;
    // 重新打开文件，打开成功返回true
    bool reopen();

private:
    std::string m_filename;
    std::ofstream m_filestream;
    uint64_t m_lastTime = 0;
};

class LoggerManager {
public:
    LoggerManager();
    Logger::ptr getLogger(const std::string &name);

    void init();
    Logger::ptr getRoot() const { return m_root; }

    std::string toYamlString();

private:
    std::map<std::string, Logger::ptr> m_loggers;
    Logger::ptr m_root;
};

typedef jaysheng::Singleton<LoggerManager> LoggerMgr;

} // namespace jaysheng

#endif // !__JAYSHENG_LOG_H__
