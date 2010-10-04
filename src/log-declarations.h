/*=======================================================================\
#                                                                        $
#   Copyright (C) 2010 Nokia Corporation.                                $
#                                                                        $
#   Author: Ilya Dogolazky <ilya.dogolazky@nokia.com>                    $
#                                                                        $
#     This file is part of qmlog                                         $
#                                                                        $
#     qmlog is free software; you can redistribute it and/or modify      $
#     it under the terms of the GNU Lesser General Public License        $
#     version 2.1 as published by the Free Software Foundation.          $
#                                                                        $
#     qmlog is distributed in the hope that it will be useful, but       $
#     WITHOUT ANY WARRANTY;  without even the implied warranty  of       $
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               $
#     See the GNU Lesser General Public License  for more details.       $
#                                                                        $
#   You should have received a copy of the GNU  Lesser General Public    $
#   License along with qmlog. If not, see http://www.gnu.org/licenses/   $
\_______________________________________________________________________*/
#ifndef MAEMO_QMLOG_LOG_DECLARATION_H
#define MAEMO_QMLOG_LOG_DECLARATION_H

#include <cstdio>

#include <memory>

/* Verbosity levels, the upper boundary could be set at compile time.
 *
 * 0 INTERNAL --- produced by failing log_assert(...)
 * 1 CRITICAL --- programm can continue, but some stuff is lost/can't be done
 * 2 ERROR --- incorrect input
 * 3 WARNING --- tolerable input, should be corrected
 * 4 INFO --- just some blah blah
 * 5 DEBUG --- verbose info
 *
 */

#define LOG_LEVEL_INTERNAL 0
#define LOG_LEVEL_CRITICAL 1
#define LOG_LEVEL_ERROR    2
#define LOG_LEVEL_WARNING  3
#define LOG_LEVEL_INFO     4
#define LOG_LEVEL_DEBUG    5

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL 5
#endif

#define LOG_BIT_MASK(bit) (1 << bit)

#ifndef LOG_MAX_LOCATION
#define LOG_MAX_LOCATION (LOG_BIT_MASK(LOG_LEVEL_DEBUG)|LOG_BIT_MASK(LOG_LEVEL_INTERNAL))
#endif

#ifndef LOG_ASSERTION
#define LOG_ASSERTION 1
#endif

#if LOG_MAX_LEVEL<LOG_LEVEL_INTERNAL || LOG_MAX_LEVEL>LOG_LEVEL_DEBUG
#error LOG_MAX_LEVEL outside of [0..5]
#endif

#define LOG_OUTPUT_WORD_WRAP       0
#define LOG_OUTPUT_MESSAGE         1
#define LOG_OUTPUT_FILE_LINE       2
#define LOG_OUTPUT_FUNC            3
#define LOG_OUTPUT_PID             4
#define LOG_OUTPUT_NAME            5
#define LOG_OUTPUT_TZ_SYM_LINK     6
#define LOG_OUTPUT_TIME_LOW_BIT    7
#define LOG_OUTPUT_TIME_HIGH_BIT   8
#define LOG_OUTPUT_DATE            9
#define LOG_OUTPUT_TZ_ABBR          10
#define LOG_OUTPUT_MTIMER_LOW_BIT  11
#define LOG_OUTPUT_MTIMER_HIGH_BIT 12


class LoggerSettings
{
public:
  enum
  {
      EMTimerMs = LOG_BIT_MASK(LOG_OUTPUT_MTIMER_HIGH_BIT)
    , EMTimerNs = LOG_BIT_MASK(LOG_OUTPUT_MTIMER_LOW_BIT)
    , EMTimer = EMTimerMs | EMTimerNs //both ms and ns bits means no ms and ns
    , ETzAbbr = LOG_BIT_MASK(LOG_OUTPUT_TZ_ABBR)
    , EDate = LOG_BIT_MASK(LOG_OUTPUT_DATE)
    , ETimeMs = LOG_BIT_MASK(LOG_OUTPUT_TIME_HIGH_BIT)
    , ETimeNs = LOG_BIT_MASK(LOG_OUTPUT_TIME_LOW_BIT)
    , ETime = ETimeMs | ETimeNs //both ms and ns bits means no ms and ns
    , ETzSymLink = LOG_BIT_MASK(LOG_OUTPUT_TZ_SYM_LINK)
    , EDateTimeInfo = EMTimer | ETzAbbr | EDate | ETime | ETzSymLink

    , EName = LOG_BIT_MASK(LOG_OUTPUT_NAME)
    , EPid = LOG_BIT_MASK(LOG_OUTPUT_PID)
    , EProcessInfo = EName | EPid

    , EFunc = LOG_BIT_MASK(LOG_OUTPUT_FUNC)
    , EFileLine = LOG_BIT_MASK(LOG_OUTPUT_FILE_LINE)
    , EMessage = LOG_BIT_MASK(LOG_OUTPUT_MESSAGE)
    , EDebugInfo = EFunc | EFileLine | EMessage

    , EWordWrap = LOG_BIT_MASK(LOG_OUTPUT_WORD_WRAP)
  };

public:
  LoggerSettings(int new_verbosity_level, int new_location_mask, int new_message_format);

  void setVerbosityLevel(int new_verbosity_level);
  void setLocationMask(int new_location_mask);
  void setMessageFormat(int new_message_format);

  bool isLogShown(int aLevel) const;
  bool isLocationShown(int aLevel) const;

  bool isMTimerMs() const;
  bool isMTimerNs() const;
  bool isMTimer() const;
  bool isTzAbbr() const;
  bool isDate() const;
  bool isTimeMs() const;
  bool isTimeNs() const;
  bool isTime() const;
  bool isTzSymLink() const;
  bool isDateTimeInfo() const;

  bool isName() const;
  bool isPid() const;
  bool isProcessInfo() const;

  bool isFunc() const;
  bool isFileLine() const;
  bool isMessage() const;
  bool isDebugInfo() const;

  bool isWordWrap() const;

private:
  int verbosity_level ;
  int location_mask ;
  int message_format ;
};

class LoggerDev
{
public:
  enum
  {
      DefaultLevel = LOG_MAX_LEVEL
    , DefaultLocation = LOG_MAX_LOCATION
    , DefaultFormat =   LoggerSettings::EMTimerMs | LoggerSettings::ETzAbbr /*TODO remove --->>>*/| LoggerSettings::EMTimer
                      | LoggerSettings::EDate | LoggerSettings::ETimeMs /*TODO remove --->>>*/| LoggerSettings::ETime
                      | LoggerSettings::ETzSymLink | LoggerSettings::EProcessInfo 
                      | LoggerSettings::EDebugInfo | LoggerSettings::EWordWrap
  };
public:
  virtual ~LoggerDev();

  void logGeneric(int aLevel, bool aShowLevel, const char *aFmt, va_list anArgs);
  void setSettings(const LoggerSettings& aSettings);

protected:
  LoggerDev(int aVerbosityLevel, int aLocationMask, int aMessageFormat);
  virtual void vlogGeneric( int aLevel, bool aShowLevel, const char *aDateTimeInfo,
                            const char* aProcessInfo, const char *aMessage) = 0;
  const LoggerSettings& settings() const;

private:
  int snprintfappend(char * str, int buf_len, int current_len, const char * fmt, ...) __attribute__((format(printf, 5, 6))) ;
  int vsnprintfappend(char * str, int buf_len, int current_len, const char * fmt, va_list arg);

private:
  LoggerSettings iSettings;
};

class FileLoggerDev : public LoggerDev
{
public:
  FileLoggerDev(const char *aFileName);
  ~FileLoggerDev();

protected:
  FileLoggerDev(FILE *aFp, bool aTakeOwnership, int aVerbosityLevel, int aLocationMask, int aMessageFormat);

  virtual void vlogGeneric( int aLevel, bool aShowLevel, const char *aDateTimeInfo,
                            const char* aProcessInfo, const char *aMessage);

private:
  FILE *iFp;
  bool iIsFpOwner;
};

struct log_t
{
  static log_t& logger();
  static void log_init(const char *name, const char *path, bool sys, bool std) ;

  void message(int level, const char *fmt, ...) __attribute__((format(printf,3,4))) ;
  void message(int level) ;
  void message(int level, int line, const char *file, const char *func, const char *fmt, ...) __attribute__((format(printf,6,7))) ;
  void message(int level, int line, const char *file, const char *func) ;
  void log_failed_assertion(const char *assertion, int line, const char *file, const char *func, const char *fmt, ...) __attribute__((format(printf,6,7))) ;
  void log_failed_assertion(const char *assertion, int line, const char *file, const char *func) ;
  int level(int new_level=-1) ;
  log_t(bool global, int new_level=-1, int location_mask=-1) ;
  void addLoggerDev(LoggerDev* aLoggerDev);
private:
    ~log_t() ;
public: //TODO for debug
  static bool use_syslog, use_stderr ;
  static FILE *fp ;
  static const char *prg_name ;
  static const char *level_name(int level) ; //TODO move to FileLoggerDev?
private:
  static int syslog_level_id(int level) ;
  void log_generic(int level, bool show_level, const char *fmt, ...) __attribute__((format(printf,4,5))) ;
  void vlog_generic(int level, bool show_level, const char *fmt, va_list args) ;
  void log_location(int level, bool message_follows, int line, const char *file, const char *func) ;

  static log_t * iLogger;

  int verbosity_level ;
  int location_mask ;
  log_t *prev ;

  std::auto_ptr<LoggerDev> iDev; //TODO it shall be array
} ;


#define LOG_LEVEL(args) std::auto_ptr<log_t> current_log_ptr(new log_t(true,args))

#define LOG_LOCAL(args) std::auto_ptr<log_t> current_log(new log_t(false,args))

#define LOG_LOCATION __LINE__,__FILE__,__PRETTY_FUNCTION__

#if LOG_MAX_LEVEL >= LOG_LEVEL_CRITICAL
# if (LOG_MAX_LOCATION) & (1<<LOG_LEVEL_CRITICAL)
#  define log_critical(...) log_t::logger().message(LOG_LEVEL_CRITICAL, LOG_LOCATION, ## __VA_ARGS__)
# else
#  define log_critical(...) log_t::logger().message(LOG_LEVEL_CRITICAL, ## __VA_ARGS__)
# endif
#else
# define log_critical(...) (void)(0)
#endif

#if LOG_MAX_LEVEL >= LOG_LEVEL_ERROR
# if (LOG_MAX_LOCATION) & (1<<LOG_LEVEL_ERROR)
#  define log_error(...) log_t::logger().message(LOG_LEVEL_ERROR, LOG_LOCATION, ## __VA_ARGS__)
# else
#  define log_error(...) log_t::logger().message(LOG_LEVEL_ERROR, ## __VA_ARGS__)
# endif
#else
# define log_error(...) (void)(0)
#endif

#if LOG_MAX_LEVEL >= LOG_LEVEL_WARNING
# if (LOG_MAX_LOCATION) & (1<<LOG_LEVEL_WARNING)
#  define log_warning(...) log_t::logger().message(LOG_LEVEL_WARNING, LOG_LOCATION, ## __VA_ARGS__)
# else
#  define log_warning(...) log_t::logger().message(LOG_LEVEL_WARNING, ## __VA_ARGS__)
# endif
#else
# define log_warning(...) (void)(0)
#endif

#if LOG_MAX_LEVEL >= LOG_LEVEL_INFO
# if (LOG_MAX_LOCATION) & (1<<LOG_LEVEL_INFO)
#  define log_info(...) log_t::logger().message(LOG_LEVEL_INFO, LOG_LOCATION, ## __VA_ARGS__)
# else
#  define log_info(...) log_t::logger().message(LOG_LEVEL_INFO, ## __VA_ARGS__)
# endif
#else
# define log_info(...) (void)(0)
#endif

#if LOG_MAX_LEVEL >= LOG_LEVEL_DEBUG
# if (LOG_MAX_LOCATION) & (1<<LOG_LEVEL_DEBUG)
#  define log_debug(...) log_t::logger().message(LOG_LEVEL_DEBUG, LOG_LOCATION, ## __VA_ARGS__)
# else
#  define log_debug(...) log_t::logger().message(LOG_LEVEL_DEBUG, ## __VA_ARGS__)
# endif
#else
# define log_debug(...) (void)(0)
#endif

#if LOG_ASSERTION
# define log_assert(x, ...) do { if(!(x)) log_t::logger().log_failed_assertion(#x, LOG_LOCATION, ## __VA_ARGS__) ; } while(0)
#else
# define log_assert(...) (void)(0)
#endif

#endif
