/**
 * @file timers.hpp
 * @author Matthew Amidon
 * @author Marcus Edel
 *
 * Timers for MLPACK.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_CORE_UTILITIES_TIMERS_HPP
#define __MLPACK_CORE_UTILITIES_TIMERS_HPP

#include <map>
#include <string>

#if defined(__unix__) || defined(__unix)
  #include <time.h>       // clock_gettime()
  #include <sys/time.h>   // timeval, gettimeofday()
  #include <unistd.h>     // flags like  _POSIX_VERSION
#elif defined(__MACH__) && defined(__APPLE__)
  #include <mach/mach_time.h>   // mach_timebase_info,
                                // mach_absolute_time()

  // TEMPORARY
  #include <time.h>       // clock_gettime()
  #include <sys/time.h>   // timeval, gettimeofday()
  #include <unistd.h>     // flags like  _POSIX_VERSION
#elif defined(_WIN32)
  #ifndef NOMINMAX
    #define NOMINMAX // Don't define min and max macros.
  #endif
  #include <windows.h>  // GetSystemTimeAsFileTime(),
                        // QueryPerformanceFrequency(),
                        // QueryPerformanceCounter()
  #include <winsock.h>  // timeval on windows
  #undef NOMINMAX

  // uint64_t isn't defined on every windows.
  #if !defined(HAVE_UINT64_T)
    #if SIZEOF_UNSIGNED_LONG == 8
      typedef unsigned long uint64_t;
    #else
      typedef unsigned long long  uint64_t;
    #endif  // SIZEOF_UNSIGNED_LONG
  #endif  // HAVE_UINT64_T

  //gettimeofday has no equivalent will need to write extra code for that.
  #if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
    #define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
  #else
    #define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
  #endif // _MSC_VER, _MSC_EXTENSIONS
#else
  #error "unknown OS"
#endif

namespace mlpack {

/**
 * The timer class provides a way for mlpack methods to be timed.  The three
 * methods contained in this class allow a named timer to be started and
 * stopped, and its value to be obtained.
 */
class Timer
{
 public:
  /**
   * Start the given timer.  If a timer is started, then stopped, then
   * re-started, then re-stopped, the final value of the timer is the length of
   * both runs -- that is, mlpack timers are additive for each time they are
   * run, and do not reset.
   *
   * @note A std::runtime_error exception will be thrown if a timer is started
   * twice.
   *
   * @param name Name of timer to be started.
   */
  static void Start(const std::string& name);

  /**
   * Stop the given timer.
   *
   * @note A std::runtime_error exception will be thrown if a timer is started
   * twice.
   *
   * @param name Name of timer to be stopped.
   */
  static void Stop(const std::string& name);

  /**
   * Get the value of the given timer.
   *
   * @param name Name of timer to return value of.
   */
  static timeval Get(const std::string& name);
};

class Timers
{
 public:
  //! Nothing to do for the constructor.
  Timers() { }

  /**
   * Returns a copy of all the timers used via this interface.
   */
  std::map<std::string, timeval>& GetAllTimers();

  /**
   * Returns a copy of the timer specified.
   *
   * @param timerName The name of the timer in question.
   */
  timeval GetTimer(const std::string& timerName);

  /**
   * Prints the specified timer.  If it took longer than a minute to complete
   * the timer will be displayed in days, hours, and minutes as well.
   *
   * @param timerName The name of the timer in question.
   */
  void PrintTimer(const std::string& timerName);

  /**
   * Initializes a timer, available like a normal value specified on
   * the command line.  Timers are of type timeval.  If a timer is started, then
   * stopped, then re-started, then stopped, the final timer value will be the
   * length of both runs of the timer.
   *
   * @param timerName The name of the timer in question.
   */
  void StartTimer(const std::string& timerName);

  /**
   * Halts the timer, and replaces it's value with
   * the delta time from it's start
   *
   * @param timerName The name of the timer in question.
   */
  void StopTimer(const std::string& timerName);

  /**
   * Returns state of the given timer.
   * 
   * @param timerName The name of the timer in question.
   */
  bool GetState(std::string timerName);
  
 private:
  //! A map of all the timers that are being tracked.
  std::map<std::string, timeval> timers;
  //! A map that contains whether or not each timer is currently running.
  std::map<std::string, bool> timerState;

  void FileTimeToTimeVal(timeval* tv);
  void GetTime(timeval* tv);
};

} // namespace mlpack

#endif // __MLPACK_CORE_UTILITIES_TIMERS_HPP
