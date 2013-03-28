// standard c++ headers
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>

// C++11 headers
#include <chrono>

// boost headers
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

// TBB headers
#include <tbb/tick_count.h>

// for timespec and clock_gettime
#include <time.h>

// for timeval and gettimeofday
#include <sys/time.h>

// for timeval and getrusage
#include <sys/resource.h>

// for omp_get_wtime
#include <omp.h>

// for times and tms, sysconf, usleep
#include <sys/times.h>
#include <unistd.h>

#if defined(__APPLE__) || defined(__MACH__)
// Darwin system headers
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif // defined(__APPLE__) || defined(__MACH__)

// to check if reading the TSC is allowed
#if defined(__linux__)
#include <linux/version.h>
#include <sys/prctl.h>
#endif // defined(__linux__)

// for rdtscp, rdtscp, lfence, mfence, cpuid
#if defined(__x86_64__) || defined(__i386__)
#include <x86intrin.h>
#include <cpuid.h>
#endif // defined(__x86_64__) || defined(__i386__)

// check available capabilities
#if (defined(_POSIX_TIMERS) && (_POSIX_TIMERS >= 0))

#define HAVE_POSIX_CLOCK_REALTIME

#if (defined(_POSIX_MONOTONIC_CLOCK) && (_POSIX_MONOTONIC_CLOCK >= 0))
#define HAVE_POSIX_CLOCK_MONOTONIC           
#endif // _POSIX_MONOTONIC_CLOCK

#if (defined(_POSIX_CPUTIME) && (_POSIX_CPUTIME >= 0))
#define HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID
#endif // _POSIX_CPUTIME

#if (defined(_POSIX_THREAD_CPUTIME) && (_POSIX_THREAD_CPUTIME >= 0))
#define HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID
#endif // _POSIX_THREAD_CPUTIME

#ifdef __linux__
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 28)
#define HAVE_POSIX_CLOCK_MONOTONIC_RAW
#endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 28)
#endif // __linux__

#endif // _POSIX_TIMERS

#if defined(__APPLE__) || defined(__MACH__)

#define HAVE_MACH_SYSTEM_CLOCK
#define HAVE_MACH_REALTIME_CLOCK
#define HAVE_MACH_CALENDAR_CLOCK
#define HAVE_MACH_ABSOLUTE_TIME

#endif // defined(__APPLE__) || defined(__MACH__)


constexpr unsigned int SIZE = 1000000;


std::string read_clock_source() {
  std::ifstream current_clocksource("/sys/devices/system/clocksource/clocksource0/current_clocksource");
  if (current_clocksource.good()) {
    std::string value;
    current_clocksource >> value;
    return value;
  } else {
    return std::string("default");
  }
}

#if defined(HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID) || defined(HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID)
// check if CLOCK_PROCESS_CPUTIME_ID and CLOCK_THREAD_CPUTIME_ID are properly supported 
bool clock_cputime_supported() {
  // from the clock_gettime(3) man page:
  // If the CPUs in an SMP system have different clock sources then there is no way to maintain a 
  // correlation between the timer registers since each CPU will run at a slightly different frequency.
  // If that is the case then clock_getcpuclockid(0) will return ENOENT to signify this condition.
  // The two clocks will then only be useful if it can be ensured that a process stays on a certain CPU.
  clockid_t clock;
  if (clock_getcpuclockid(0, & clock) == ENOENT)
    return false;
  else
    return true;
}
#endif // HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID || HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID

// check if the RDTSCP instruction is supported
#if defined(__x86_64__) || defined(__i386__)
#ifndef bit_RDTSCP
#define bit_RDTSCP      (1 << 27)
#endif // bit_RDTSCP

bool rdtscp_supported() {
  unsigned int eax, ebx, ecx, edx;
  if (__get_cpuid(0x80000001, & eax, & ebx, & ecx, & edx))
    return (edx & bit_RDTSCP) != 0;
  else
    return false;
}
#else // defined(__x86_64__) || defined(__i386__)
bool rdtscp_supported() {
  return false;
}
#endif // defined(__x86_64__) || defined(__i386__)

// check if the RDTSC and RDTSCP instructions are allowed
bool tsc_allowed() {
#if defined(__x86_64__) || defined(__i386__)
#if defined(__linux__)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 26)
    int tsc_val;
    prctl(PR_GET_TSC, &tsc_val);
    if (tsc_val != PR_TSC_ENABLE)
      prctl(PR_SET_TSC, PR_TSC_ENABLE);
    prctl(PR_GET_TSC, &tsc_val);
    if (tsc_val != PR_TSC_ENABLE)
      return false;
#endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 26)
#endif // defined(__linux__)
    return true;
#else // defined(__x86_64__) || defined(__i386__)
    return false;
#endif // defined(__x86_64__) || defined(__i386__)
}

// calibrate TSC with respect to std::chrono::high_resolution_clock
#if defined(__x86_64__) || defined(__i386__)
double calibrate_tsc() {
  constexpr unsigned int sample_size = 20;          // 20 samplings
  constexpr unsigned int sleep_time  = 10000;       // 10 ms
  unsigned long long ticks[sample_size];
  double             times[sample_size];

  auto reference = std::chrono::high_resolution_clock::now();
  for (unsigned int i = 0; i < sample_size; ++i) {
    usleep(sleep_time);
    ticks[i] = __rdtsc();
    times[i] = std::chrono::duration_cast<std::chrono::duration<double>>( std::chrono::high_resolution_clock::now() - reference ).count();
  }

  double mean_x = 0, mean_y = 0;
  for (unsigned int i = 0; i < sample_size; ++i) {
    mean_x += (double) times[i];
    mean_y += (double) ticks[i];
  }
  mean_x /= (double) sample_size;
  mean_y /= (double) sample_size;

  double sigma_xy = 0, sigma_xx = 0;
  for (unsigned int i = 0; i < sample_size; ++i) {
    sigma_xx += (double) (times[i] - mean_x) * (double) (times[i] - mean_x);
    sigma_xy += (double) (times[i] - mean_x) * (double) (ticks[i] - mean_y);
  }

  // ticks per second
  return sigma_xy / sigma_xx;
}
#endif // defined(__x86_64__) || defined(__i386__)

class TimerInterface {
public:
  // perform the measurements
  virtual void measure() = 0;

  // extract from characteristics of the timer from the measurements
  virtual void compute() = 0;

  // print a report
  virtual void report() = 0;
};


template <typename T>
class TimerBase : public TimerInterface {
public:
  typedef T time_type;

  enum clock_type {
    UNDEFINED = 0,
    CT_REALTIME_CLOCK,
    CT_CPU_TIME_CLOCK
  };

  // default ctor, initialize all members
  TimerBase() :
    values(),
    start_(),
    stop_(),
    type(UNDEFINED),
    granularity( 0. ),
    resolution_min( 0. ),
    resolution_median( 0. ),
    resolution_avg( 0. ),
    resolution_sigma( 0. ),
    overhead( 0. ),
    ticks_per_second( 1. ),
    description()
  {
  }

  // return the delta between two time_type, expressed in seconds
  double delta(const time_type & t1, const time_type & t2) {
    // generic implementation, see below for template specializations
    return (double) (t2 - t1) / ticks_per_second;
  }

  // extract from characteristics of the timer from the measurements
  void compute() {
    // per-call overhead
    overhead = std::chrono::duration_cast<std::chrono::duration<double>>(stop_ - start_).count() / SIZE;

    // resolution (median of the increments)
    std::vector<double> steps;
    steps.reserve(SIZE);
    for (unsigned int i = 0; i < SIZE-1; ++i) {
      double step = delta(values[i], values[i + 1]);
      if (step > 0)
        steps.push_back(step);
    }
    std::sort( steps.begin(), steps.end() );
    if (not steps.empty()) {
      // measure resolution as the median of the steps
      resolution_median = steps[steps.size() / 2];

      // measure resolution as the first non-zero step
      resolution_min = steps.front();

      // measure average and sigma of the steps
      double n    = steps.size();
      double sum  = 0.;
      for (size_t i = 0; i < steps.size(); ++i)
        sum  += steps[i];
      resolution_avg = sum / n;
      if (steps.size() > 1) {
        double sum2 = 0.;
        for (size_t i = 0; i < steps.size(); ++i)
          sum2 += (steps[i] - resolution_avg) * (steps[i] - resolution_avg);
        resolution_sigma = std::sqrt( sum2 ) / n;
        // take into account limited accuracy
        if (resolution_sigma >= 0 and resolution_sigma < 1.e-9)
          resolution_sigma = 1.e-9;
      }
    }
  }

  // print a report
  void report() {
    std::cout << std::setprecision(0) << std::fixed;
    std::cout << "Performance of " << description;
    if (type == CT_REALTIME_CLOCK)
      std::cout << " (real-time)";
    else if (type == CT_CPU_TIME_CLOCK)
      std::cout << " (cpu time)";
    std::cout << std::endl;
    std::cout << "\tAverage time per call: " << std::right << std::setw(10) << overhead    * 1e9 << " ns" << std::endl;
    if (granularity)
      std::cout << "\tReported resolution:   " << std::right << std::setw(10) << granularity * 1e9 << " ns" << std::endl;
    else
      std::cout << "\tReported resolution:   " << std::right << std::setw(10) << "n/a" << std::endl;
    std::cout << "\tMeasured resolution:   " << std::right << std::setw(10) << resolution_min  * 1e9 << " ns (median: " << resolution_median * 1e9 << " ns) (average: " << resolution_avg * 1e9 << " +/- " << resolution_sigma * 1e9 << " ns)" << std::endl;
    std::cout << std::endl;
  }

  inline
  void start() {
    start_ = std::chrono::high_resolution_clock::now();
  }

  inline
  void stop() {
    stop_ = std::chrono::high_resolution_clock::now();
  }

protected:
  time_type                                         values[SIZE];
  std::chrono::high_resolution_clock::time_point    start_;
  std::chrono::high_resolution_clock::time_point    stop_;
  clock_type    type;
  double        granularity;            // the reported resolution, in seconds
  double        resolution_min;         // the measured resolution, in seconds (smallest of the steps)
  double        resolution_median;      // the measured resolution, in seconds (median of the steps)
  double        resolution_avg;         // the measured resolution, in seconds (average of the steps)
  double        resolution_sigma;       // the measured resolution, in seconds (sigma of the mean)
  double        overhead;               // the measured per-call overhead, in seconds
  double        ticks_per_second;       // optinally used by the delta() method
  std::string   description;
};


// timespec has a ns resolution
template <>
double TimerBase<timespec>::delta(const timespec & start, const timespec & stop) {
  if (stop.tv_nsec > start.tv_nsec)
    return (double) (stop.tv_sec - start.tv_sec) + (double) (stop.tv_nsec - start.tv_nsec) / (double) 1e9;
  else
    return (double) (stop.tv_sec - start.tv_sec) - (double) (start.tv_nsec - stop.tv_nsec) / (double) 1e9;
}

#if defined(__APPLE__) || defined (__MACH__)
// mach_timespec_t has a ns resolution
template <>
double TimerBase<mach_timespec_t>::delta(const mach_timespec_t & start, const mach_timespec_t & stop) {
  if (stop.tv_nsec > start.tv_nsec)
    return (double) (stop.tv_sec - start.tv_sec) + (double) (stop.tv_nsec - start.tv_nsec) / (double) 1e9;
  else
    return (double) (stop.tv_sec - start.tv_sec) - (double) (start.tv_nsec - stop.tv_nsec) / (double) 1e9;
}

// used by mach_absolute_time()
template<>
double TimerBase<uint64_t>::delta(const uint64_t & start, const uint64_t & stop) {
  return (double) (stop - start) / (double) ticks_per_second;
}
#endif // defined(__APPLE__) || defined (__MACH__)

// timeval has a us resolution
template <>
double TimerBase<timeval>::delta(const timeval & start, const timeval & stop) {
  if (stop.tv_usec > start.tv_usec)
    return (double) (stop.tv_sec - start.tv_sec) + (double) (stop.tv_usec - start.tv_usec) / (double) 1e6;
  else
    return (double) (stop.tv_sec - start.tv_sec) - (double) (start.tv_usec - stop.tv_usec) / (double) 1e6;
}

// clock_t can have different resolutions !
template <>
double TimerBase<clock_t>::delta(const clock_t & start, const clock_t & stop) {
  return (double) (stop-start) / (double) ticks_per_second;
}

/*
// std::chrono::system_clock::time_point
template <>
double TimerBase<std::chrono::system_clock::time_point>::delta(const std::chrono::system_clock::time_point & start, const std::chrono::system_clock::time_point & stop) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
}

// std::chrono::steady_clock::time_point
template <>
double TimerBase<std::chrono::steady_clock::time_point>::delta(const std::chrono::steady_clock::time_point & start, const std::chrono::steady_clock::time_point & stop) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
}
*/

// std::chrono::high_resolution_clock::time_point
template <>
double TimerBase<std::chrono::high_resolution_clock::time_point>::delta(const std::chrono::high_resolution_clock::time_point & start, const std::chrono::high_resolution_clock::time_point & stop) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
}

// tbb::tick_count
template <>
double TimerBase<tbb::tick_count>::delta(const tbb::tick_count & start, const tbb::tick_count & stop) {
  return (stop - start).seconds();
}






// C++11 steady_clock
class TimerCxx11SteadyClock : public TimerBase<std::chrono::steady_clock::time_point> {
public:
  TimerCxx11SteadyClock() {
    description = "std::chrono::steady_clock";
    type        = CT_REALTIME_CLOCK;
    granularity = (double) std::chrono::steady_clock::period::num / (double) std::chrono::steady_clock::period::den;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = std::chrono::steady_clock::now();
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// C++11 system_clock
class TimerCxx11SystemClock : public TimerBase<std::chrono::system_clock::time_point> {
public:
  TimerCxx11SystemClock() {
    description = "std::chrono::system_clock";
    type        = CT_REALTIME_CLOCK;
    granularity = (double) std::chrono::system_clock::period::num / (double) std::chrono::system_clock::period::den;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = std::chrono::system_clock::now();
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// C++11 high_resolution_clock
class TimerCxx11HighResolutionClock : public TimerBase<std::chrono::high_resolution_clock::time_point> {
public:
  TimerCxx11HighResolutionClock() {
    description = "std::chrono::high_resolution_clock";
    type        = CT_REALTIME_CLOCK;
    granularity = (double) std::chrono::high_resolution_clock::period::num / (double) std::chrono::high_resolution_clock::period::den;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = std::chrono::high_resolution_clock::now();
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// TBB timer
class TimerTBB : public TimerBase<tbb::tick_count> {
public:
  TimerTBB() {
    description = "tbb::tick_count";
    type        = CT_REALTIME_CLOCK;
    granularity = 0.;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = tbb::tick_count::now();
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// boost cpu_timer, wall_clock
class TimerBoostCpuTimerWallClock : public TimerBase<double> {
public:
  TimerBoostCpuTimerWallClock() {
    description = "boost::cpu_timer";
    type        = CT_REALTIME_CLOCK;
    granularity = 0.;   // n/a
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = timer.elapsed().wall * 1.e-9;
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

private:
  boost::timer::cpu_timer timer;
};


// boost cpu_timer, user+system
class TimerBoostCpuTimerCpuTime : public TimerBase<double> {
public:
  TimerBoostCpuTimerCpuTime() {
    description = "boost::cpu_timer";
    type        = CT_CPU_TIME_CLOCK;
    granularity = 0.;   // n/a
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i) {
      auto const & elapsed = timer.elapsed();
      values[i] = (elapsed.user + elapsed.system) * 1.e-9;
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

private:
  boost::timer::cpu_timer timer;
};


#ifdef HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID
// clock_gettime(CLOCK_THREAD_CPUTIME_ID)
class TimerClockGettimeThread : public TimerBase<timespec> {
public:
  TimerClockGettimeThread() {
    description = "clock_gettime(CLOCK_THREAD_CPUTIME_ID)";
    type        = CT_CPU_TIME_CLOCK;

    timespec value;
    clock_getres(CLOCK_THREAD_CPUTIME_ID, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_gettime(CLOCK_THREAD_CPUTIME_ID, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};
#endif // HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID

#ifdef HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID
// clock_gettime(CLOCK_PROCESS_CPUTIME_ID)
class TimerClockGettimeProcess : public TimerBase<timespec> {
public:
  TimerClockGettimeProcess() {
    description = "clock_gettime(CLOCK_PROCESS_CPUTIME_ID)";
    type        = CT_CPU_TIME_CLOCK;

    timespec value;
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};
#endif // HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID

#ifdef HAVE_POSIX_CLOCK_REALTIME
// clock_gettime(CLOCK_REALTIME)
// note: on Linux, the behaviour of this timer is affected by the choice of the clock source (/sys/devices/system/clocksource/clocksource0/current_clocksource)
class TimerClockGettimeRealtime : public TimerBase<timespec> {
public:
  TimerClockGettimeRealtime() {
    description = str(boost::format("clock_gettime(CLOCK_REALTIME, ...) using %s clock source") % read_clock_source());
    type        = CT_REALTIME_CLOCK;

    timespec value = { 0, 0 };
    clock_getres(CLOCK_REALTIME, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_gettime(CLOCK_REALTIME, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};
#endif // HAVE_POSIX_CLOCK_REALTIME

#ifdef HAVE_POSIX_CLOCK_MONOTONIC
// clock_gettime(CLOCK_MONOTONIC)
// note: on Linux, the behaviour of this timer is affected by the choice of the clock source (/sys/devices/system/clocksource/clocksource0/current_clocksource)
class TimerClockGettimeMonotonic : public TimerBase<timespec> {
public:
  TimerClockGettimeMonotonic() {
    description = str(boost::format("clock_gettime(CLOCK_MONOTONIC, ...) using %s clock source") % read_clock_source());
    type        = CT_REALTIME_CLOCK;

    timespec value = { 0, 0 };
    clock_getres(CLOCK_MONOTONIC, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_gettime(CLOCK_MONOTONIC, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};
#endif // HAVE_POSIX_CLOCK_MONOTONIC

#ifdef HAVE_POSIX_CLOCK_MONOTONIC_RAW
// clock_gettime(CLOCK_MONOTONIC_RAW)
class TimerClockGettimeMonotonicRaw : public TimerBase<timespec> {
public:
  TimerClockGettimeMonotonicRaw() {
    description = str(boost::format("clock_gettime(CLOCK_MONOTONIC_RAW, ...) using %s clock source") % read_clock_source());
    type        = CT_REALTIME_CLOCK;

    timespec value = { 0, 0 };
    clock_getres(CLOCK_MONOTONIC_RAW, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_gettime(CLOCK_MONOTONIC_RAW, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};
#endif // HAVE_POSIX_CLOCK_MONOTONIC_RAW


#ifdef HAVE_MACH_SYSTEM_CLOCK
// clock_get_time <-- SYSTEM_CLOCK
class TimerClockGetTimeSystemClock : public TimerBase<mach_timespec_t> {
public:
  TimerClockGetTimeSystemClock() {
    description = "clock_get_time() with SYSTEM_CLOCK";
    type        = CT_REALTIME_CLOCK;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock_port);
    int value;
    unsigned int size = 1;
    clock_get_attributes( clock_port, CLOCK_GET_TIME_RES, & value, & size);
    granularity = value * 1.e-9;
  }

  ~TimerClockGetTimeSystemClock() {
    mach_port_deallocate(mach_task_self(), clock_port);
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_get_time(clock_port, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

private:
  clock_serv_t clock_port;
};
#endif // HAVE_MACH_SYSTEM_CLOCK

#ifdef HAVE_MACH_REALTIME_CLOCK
// clock_get_time <-- REALTIME_CLOCK
class TimerClockGetTimeRealtimeClock : public TimerBase<mach_timespec_t> {
public:
  TimerClockGetTimeRealtimeClock() {
    description = "clock_get_time() with REALTIME_CLOCK";
    type        = CT_REALTIME_CLOCK;

    host_get_clock_service(mach_host_self(), REALTIME_CLOCK, &clock_port);
    int value;
    unsigned int size = 1;
    clock_get_attributes( clock_port, CLOCK_GET_TIME_RES, & value, & size);
    granularity = value * 1.e-9;
  }

  ~TimerClockGetTimeRealtimeClock() {
    mach_port_deallocate(mach_task_self(), clock_port);
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_get_time(clock_port, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

private:
  clock_serv_t clock_port;
};
#endif // HAVE_MACH_REALTIME_CLOCK

#ifdef HAVE_MACH_CALENDAR_CLOCK
// clock_get_time <-- CALENDAR_CLOCK
class TimerClockGetTimeCalendarClock : public TimerBase<mach_timespec_t> {
public:
  TimerClockGetTimeCalendarClock() {
    description = "clock_get_time() with CALENDAR_CLOCK";
    type        = CT_REALTIME_CLOCK;

    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &clock_port);
    int value;
    unsigned int size = 1;
    clock_get_attributes( clock_port, CLOCK_GET_TIME_RES, & value, & size);
    granularity = value * 1.e-9;
  }

  ~TimerClockGetTimeCalendarClock() {
    mach_port_deallocate(mach_task_self(), clock_port);
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      clock_get_time(clock_port, values+i);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

private:
  clock_serv_t clock_port;
};
#endif // HAVE_MACH_CALENDAR_CLOCK

#ifdef HAVE_MACH_ABSOLUTE_TIME
// this function returns a Mach absolute time value for the current wall clock time in units of uint64_t
class TimerMachAbsoluteTime : public TimerBase<uint64_t> {
public:
  TimerMachAbsoluteTime() {
    description = "mach_absolute_time()";
    type        = CT_REALTIME_CLOCK;

    mach_timebase_info(& timebase_info);
    ticks_per_second = 1.e9 / timebase_info.numer * timebase_info.denom;
    granularity = 1.e-9 * timebase_info.numer / timebase_info.denom;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = mach_absolute_time();
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

private:
  mach_timebase_info_data_t timebase_info;
};
#endif // HAVE_MACH_ABSOLUTE_TIME

// gettimeofday()
// note: on Linux, the behaviour of this timer is affected by the choice of the clock source (/sys/devices/system/clocksource/clocksource0/current_clocksource)
class TimerGettimeofday : public TimerBase<timeval> {
public:
  TimerGettimeofday() {
    description = str(boost::format("gettimeofday() using %s clock source") % read_clock_source());
    type        = CT_REALTIME_CLOCK;
    granularity = 1e-6;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      gettimeofday(values+i, NULL);
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// getrusage(RUSAGE_SELF)
class TimerGetrusageSelf: public TimerBase<timeval> {
public:
  TimerGetrusageSelf() {
    description = "getrusage(RUSAGE_SELF)";
    type        = CT_CPU_TIME_CLOCK;
    granularity = 1e-6;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i) {
      rusage usage;
      getrusage(RUSAGE_SELF, & usage);
      values[i].tv_sec  = usage.ru_stime.tv_sec  + usage.ru_utime.tv_sec;
      values[i].tv_usec = usage.ru_stime.tv_usec + usage.ru_utime.tv_usec;
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// omp_get_wtime()
// note: on Linux, the behaviour of this timer is affected by the choice of the clock source (/sys/devices/system/clocksource/clocksource0/current_clocksource)
class TimerOMPGetWtime : public TimerBase<double> {
public:
  TimerOMPGetWtime() {
    description = str(boost::format("omp_get_wtime() using %s clock source") % read_clock_source());
    type        = CT_REALTIME_CLOCK;
    granularity = omp_get_wtick();
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = omp_get_wtime();
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// clock()
class TimerClock : public TimerBase<clock_t> {
public:
  TimerClock() {
    description = "clock()";
    type        = CT_CPU_TIME_CLOCK;
    ticks_per_second = CLOCKS_PER_SEC;
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i)
      values[i] = clock();
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// times() - cpu time
class TimerTimesCpuTime : public TimerBase<clock_t> {
public:
  TimerTimesCpuTime() {
    description = "times()";
    type        = CT_CPU_TIME_CLOCK;
    ticks_per_second = sysconf(_SC_CLK_TCK);
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i) {
      tms value;
      times(& value);
      values[i] = value.tms_stime + value.tms_utime;
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// times() - wall-clock time
class TimerTimesWallClock : public TimerBase<clock_t> {
public:
  TimerTimesWallClock() {
    description = "times()";
    type        = CT_REALTIME_CLOCK;
    ticks_per_second = sysconf(_SC_CLK_TCK);
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i) {
#ifdef __linux__
      values[i] = times(nullptr);
#else
      tms value;
      values[i] = times(& value);
#endif
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }
};


// rdtsc()
#if defined(__x86_64__) || defined(__i386__)
class TimerRDTSC : public TimerBase<unsigned long long> {
public:
  TimerRDTSC() {
    if (not tsc_allowed())
      throw std::runtime_error("RDTSC is disabled for the current proccess, calling it would result in a SIGSEGV (see 'PR_SET_TSC' under 'man prctl')");
    
    // ticks_per_second and granularity
    ticks_per_second  = calibrate_tsc();
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;

    description = str(boost::format("rdtsc() [estimated at %#5.4g GHz]") % (ticks_per_second / 1.e9));
    type        = CT_REALTIME_CLOCK;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i) {
      values[i] = __rdtsc();
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

};


// lfence, rdtsc()
class TimerLfenceRDTSC : public TimerBase<unsigned long long> {
public:
  TimerLfenceRDTSC() {
    if (not tsc_allowed())
      throw std::runtime_error("RDTSC is disabled for the current proccess, calling it would result in a SIGSEGV (see 'PR_SET_TSC' under 'man prctl')");
    
    // ticks_per_second and granularity
    ticks_per_second  = calibrate_tsc();
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
    
    description = str(boost::format("lfence(); rdtsc() [estimated at %#5.4g GHz]") % (ticks_per_second / 1.e9));
    type        = CT_REALTIME_CLOCK;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i) {
      _mm_lfence();
      values[i] = __rdtsc();
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

};


// mfence, rdtsc()
class TimerMfenceRDTSC : public TimerBase<unsigned long long> {
public:
  TimerMfenceRDTSC() {
    if (not tsc_allowed())
      throw std::runtime_error("RDTSC is disabled for the current proccess, calling it would result in a SIGSEGV (see 'PR_SET_TSC' under 'man prctl')");
    
    // ticks_per_second and granularity
    ticks_per_second  = calibrate_tsc();
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
    
    description = str(boost::format("mfence(); rdtsc() [estimated at %#5.4g GHz]") % (ticks_per_second / 1.e9));
    type        = CT_REALTIME_CLOCK;
  }

  void sample() {
    for (unsigned int i = 0; i < SIZE; ++i) {
      _mm_mfence();
      values[i] = __rdtsc();
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

};


// rdtscp()
class TimerRDTSCP : public TimerBase<unsigned long long> {
public:
  TimerRDTSCP() {
    if (not rdtscp_supported())
      throw std::runtime_error("RDTSCP instruction is not supported by this processor");
    if (not tsc_allowed())
      throw std::runtime_error("RDTSCP is disabled for the current proccess, calling it would result in a SIGSEGV (see 'PR_SET_TSC' under 'man prctl')");
    
    // ticks_per_second and granularity
    ticks_per_second  = calibrate_tsc();
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
    
    description = str(boost::format("rdtscp() [estimated at %#5.4g GHz]") % (ticks_per_second / 1.e9));
    type        = CT_REALTIME_CLOCK;
  }

  void sample() {
    unsigned int id = 0;
    for (unsigned int i = 0; i < SIZE; ++i) {
      values[i] = __rdtscp(& id);
    }
  }

  void measure() {
    sample();
    start();
    sample();
    stop();
  }

};
#endif // defined(__x86_64__) || defined(__i386__)

int main(void) {
  std::vector<TimerInterface *> timers;

  // C++11 timers
  timers.push_back(new TimerCxx11SystemClock());
  timers.push_back(new TimerCxx11SteadyClock());
  timers.push_back(new TimerCxx11HighResolutionClock());

  // boost timers
  timers.push_back(new TimerBoostCpuTimerWallClock());
  timers.push_back(new TimerBoostCpuTimerCpuTime());

  // OpenMP timer
  timers.push_back(new TimerOMPGetWtime());

  // TBB timer
  timers.push_back(new TimerTBB());

  // POSIX timers
#ifdef HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID
  if (clock_cputime_supported())
    timers.push_back(new TimerClockGettimeThread());
#endif // HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID

#ifdef HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID
  if (clock_cputime_supported())
    timers.push_back(new TimerClockGettimeProcess());
#endif // HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID

#ifdef HAVE_POSIX_CLOCK_REALTIME
  timers.push_back(new TimerClockGettimeRealtime());
#endif // HAVE_POSIX_CLOCK_REALTIME

#ifdef HAVE_POSIX_CLOCK_MONOTONIC
  timers.push_back(new TimerClockGettimeMonotonic());
#endif // HAVE_POSIX_CLOCK_MONOTONIC

#ifdef HAVE_POSIX_CLOCK_MONOTONIC_RAW
  timers.push_back(new TimerClockGettimeMonotonicRaw());
#endif // HAVE_POSIX_CLOCK_MONOTONIC_RAW

#ifdef HAVE_MACH_SYSTEM_CLOCK
  timers.push_back(new TimerClockGetTimeSystemClock());
#endif // HAVE_MACH_SYSTEM_CLOCK

  // OSX timers
#ifdef HAVE_MACH_REALTIME_CLOCK
  timers.push_back(new TimerClockGetTimeRealtimeClock());
#endif // HAVE_MACH_REALTIME_CLOCK

#ifdef HAVE_MACH_CALENDAR_CLOCK
  timers.push_back(new TimerClockGetTimeCalendarClock());
#endif // HAVE_MACH_CALENDAR_CLOCK

#ifdef HAVE_MACH_ABSOLUTE_TIME
  timers.push_back(new TimerMachAbsoluteTime());
#endif // HAVE_MACH_ABSOLUTE_TIME

  // Unix timers
  timers.push_back(new TimerGettimeofday());
  timers.push_back(new TimerGetrusageSelf());
  timers.push_back(new TimerClock());
  timers.push_back(new TimerTimesCpuTime());
  timers.push_back(new TimerTimesWallClock());

#if defined(__x86_64__) || defined(__i386__)
  if (tsc_allowed()) {
    timers.push_back(new TimerRDTSC());
    timers.push_back(new TimerLfenceRDTSC());
    timers.push_back(new TimerMfenceRDTSC());
    if (rdtscp_supported())
      timers.push_back(new TimerRDTSCP());
    else
      std::cout << "RDTSCP instruction not supported by this processor" << std::endl << std::endl;
  } else {
      std::cout << "access to the TSC by non-privileged processes has been disabled" << std::endl << std::endl;
  }
#endif // defined(__x86_64__) || defined(__i386__)

  std::cout << "For each timer the resolution reported is the MINIMUM (MEDIAN) (AVERAGE +/- its STDDEV) of the increments measured during the test." << std::endl << std::endl; 

  for (TimerInterface * timer: timers) {
    timer->measure();
    timer->compute();
    timer->report();
  }

  return 0;
}
