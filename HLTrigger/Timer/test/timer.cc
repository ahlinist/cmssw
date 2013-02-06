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
#include <x86intrin.h>
#include <cpuid.h>


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

static constexpr unsigned int SIZE = 1000000;


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

// check if the RDTSCP instruction is supported
#ifndef bit_RDTSCP
#define bit_RDTSCP      (1 << 27)
#endif

bool rdtscp_supported() {
  unsigned int eax, ebx, ecx, edx;
  if (__get_cpuid(0x80000001, & eax, & ebx, & ecx, & edx))
    return (edx & bit_RDTSCP) != 0;
  else
    return false;
}

// check if the RDTSC and RDTSCP instructions are allowed
bool tsc_allowed() {
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
}

// calibrate TSC with respect to CLOCK_MONOTONIC_RAW (if available) or CLOCK_MONOTONIC
double calibrate_tsc() {
  const unsigned int sample_size = 16;              // 16 samplings
  const unsigned int sleep_time  = 10000;           // 10 ms
  unsigned long long ticks[sample_size];
  double             times[sample_size];

#if defined(HAVE_POSIX_CLOCK_MONOTONIC) || defined(HAVE_POSIX_CLOCK_MONOTONIC_RAW)
  // use clock_gettime(CLOCK_MONOTONIC), or clock_gettime(CLOCK_MONOTONIC_RAW) if available
  timespec ts;
  for (unsigned int i = 0; i < sample_size; ++i) {
    if (i) usleep(sleep_time);
    ticks[i] = __rdtsc();
#ifdef HAVE_POSIX_CLOCK_MONOTONIC_RAW
    clock_gettime(CLOCK_MONOTONIC_RAW, & ts);
#else
    clock_gettime(CLOCK_MONOTONIC, & ts);
#endif
    times[i] = ts.tv_sec + (double) ts.tv_nsec / 1.e9; 
  }
#elif defined(HAVE_MACH_ABSOLUTE_TIME)
  // on OSX, use mach_absolute_time
  mach_timebase_info_data_t timebase_info;
  mach_timebase_info(& timebase_info);
  for (unsigned int i = 0; i < sample_size; ++i) {
    if (i) usleep(sleep_time);
    ticks[i] = __rdtsc();
    times[i] = (double) mach_absolute_time() * timebase_info.numer / timebase_info.denom * 1.e-9;;
  }
#else
  // fall back on std::chrono::high_resolution_clock
  auto start = std::chrono::high_resolution_clock::now();
  for (unsigned int i = 0; i < sample_size; ++i) {
    if (i) usleep(sleep_time);
    ticks[i] = __rdtsc();
    times[i] = std::chrono::duration_cast<std::chrono::duration<double>>( std::chrono::high_resolution_clock::now() - start ).count();
  }
#endif

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


class TimerInterface {
public:
  // perform the measurements
  virtual void measure(void) = 0;

  // extract from characteristics of the timer from the measurements
  virtual void compute(void) = 0;

  // print a report
  virtual void report(void) = 0;
};


template <typename T>
class TimerBase : public TimerInterface {
public:
  typedef T time_type;

  // default ctor, initialize all members
  TimerBase() :
    values( new time_type[2*SIZE+1] ),
    granularity( 0. ),
    resolution( 0. ),
    resolution_mean( 0. ),
    resolution_sigma( 0. ),
    overhead( 0. ),
    ticks_per_second( 1. ),
    description()
  {
  }

  // return the delta between two time_type, expressed in seconds
  double delta(const time_type & start, const time_type & stop) {
    // generic implementation, see below for template specializations
    return (double) (stop - start) / ticks_per_second;
  }

  // extract from characteristics of the timer from the measurements
  void compute() {
    // per-call overhead
    overhead = delta(values[SIZE], values[2*SIZE]) / (double) SIZE;

    // resolution (median of the increments)
    std::vector<double> steps;
    steps.reserve(SIZE + 1);
    for (unsigned int i = 0; i < SIZE; ++i) {
      double step = delta(values[SIZE + i], values[SIZE + i + 1]);
      if (step > 0)
        steps.push_back(step);
    }
    std::sort( steps.begin(), steps.end() );
    if (not steps.empty()) {
      // measure resolution as the median of the steps
      resolution = steps[steps.size() / 2];

      // measure average and sigma of the steps
      double n    = steps.size();
      double sum  = 0.;
      for (size_t i = 0; i < steps.size(); ++i)
        sum  += steps[i];
      resolution_mean = sum / n;
      if (steps.size() > 1) {
        double sum2 = 0.;
        for (size_t i = 0; i < steps.size(); ++i)
          sum2 += (steps[i] - resolution_mean) * (steps[i] - resolution_mean);
        resolution_sigma = std::sqrt( sum2 ) / n;
        // take into account limited accuracy
        if (resolution_sigma > 0 and resolution_sigma < 1.e-9)
          resolution_sigma = 1.e-9;
      }
    }
  }

  // print a report
  void report() {
    std::cout << std::setprecision(0) << std::fixed;
    std::cout << "Performance of " << description << std::endl;
    std::cout << "\tAverage time per call: " << std::right << std::setw(10) << overhead    * 1e9 << " ns" << std::endl;
    std::cout << "\tReported resolution:   " << std::right << std::setw(10) << granularity * 1e9 << " ns" << std::endl;
    std::cout << "\tMeasured resolution:   " << std::right << std::setw(10) << resolution  * 1e9 << " ns (" << resolution_mean * 1e9 << " +/- " << resolution_sigma * 1e9 << " ns)" << std::endl;
    /*
    std::cout << "\tSteps:" << std::endl;
    for (unsigned int i = 0; i < SIZE; ++i) {
      double step = delta(values[SIZE + i], values[SIZE + i + 1]);
      if (step > 0)
        std::cout << "\t\t" << std::right << std::setw(10) << delta(values[SIZE + i], values[SIZE + i + 1]) * 1e9 << " ns" << std::endl;
    }
    */
    std::cout << std::endl;
  }

protected:
  time_type *   values;
  double        granularity;            // the reported resolution, in seconds
  double        resolution;             // the measured resolution, in seconds (median of the steps)
  double        resolution_mean;        // the measured resolution, in seconds (mean of the steps)
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

// std::chrono::high_resolution_clock::time_point
template <>
double TimerBase<std::chrono::high_resolution_clock::time_point>::delta(const std::chrono::high_resolution_clock::time_point & start, const std::chrono::high_resolution_clock::time_point & stop) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
}


// C++11 high_resolution_clock
class TimerCxx11HighResolutionClock : public TimerBase<std::chrono::high_resolution_clock::time_point> {
public:
  TimerCxx11HighResolutionClock() {
    description = "std::chrono::high_resolution_clock";
    granularity = (double) std::chrono::high_resolution_clock::period::num / (double) std::chrono::high_resolution_clock::period::den;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      values[i] = std::chrono::high_resolution_clock::now();
  }
};


#ifdef HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID
// clock_gettime(CLOCK_THREAD_CPUTIME_ID)
class TimerClockGettimeThread : public TimerBase<timespec> {
public:
  TimerClockGettimeThread() {
    description = "clock_gettime(CLOCK_THREAD_CPUTIME_ID)";

    timespec value;
    clock_getres(CLOCK_THREAD_CPUTIME_ID, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_gettime(CLOCK_THREAD_CPUTIME_ID, values+i);
  }
};
#endif // HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID

#ifdef HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID
// clock_gettime(CLOCK_PROCESS_CPUTIME_ID)
class TimerClockGettimeProcess : public TimerBase<timespec> {
public:
  TimerClockGettimeProcess() {
    description = "clock_gettime(CLOCK_PROCESS_CPUTIME_ID)";

    timespec value;
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, values+i);
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

    timespec value = { 0, 0 };
    clock_getres(CLOCK_REALTIME, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_gettime(CLOCK_REALTIME, values+i);
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

    timespec value = { 0, 0 };
    clock_getres(CLOCK_MONOTONIC, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_gettime(CLOCK_MONOTONIC, values+i);
  }
};
#endif // HAVE_POSIX_CLOCK_MONOTONIC

#ifdef HAVE_POSIX_CLOCK_MONOTONIC_RAW
// clock_gettime(CLOCK_MONOTONIC_RAW)
class TimerClockGettimeMonotonicRaw : public TimerBase<timespec> {
public:
  TimerClockGettimeMonotonicRaw() {
    description = str(boost::format("clock_gettime(CLOCK_MONOTONIC_RAW, ...) using %s clock source") % read_clock_source());

    timespec value = { 0, 0 };
    clock_getres(CLOCK_MONOTONIC_RAW, & value);
    granularity = value.tv_sec + value.tv_nsec / (double) 1e9;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_gettime(CLOCK_MONOTONIC_RAW, values+i);
  }
};
#endif // HAVE_POSIX_CLOCK_MONOTONIC_RAW


#ifdef HAVE_MACH_SYSTEM_CLOCK
// clock_get_time <-- SYSTEM_CLOCK
class TimerClockGetTimeSystemClock : public TimerBase<mach_timespec_t> {
public:
  TimerClockGetTimeSystemClock() {
    description = "clock_get_time() with SYSTEM_CLOCK";

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock_port);
    int value;
    unsigned int size = 1;
    clock_get_attributes( clock_port, CLOCK_GET_TIME_RES, & value, & size);
    granularity = value * 1.e-9;
  }

  ~TimerClockGetTimeSystemClock() {
    mach_port_deallocate(mach_task_self(), clock_port);
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_get_time(clock_port, values+i);
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

    host_get_clock_service(mach_host_self(), REALTIME_CLOCK, &clock_port);
    int value;
    unsigned int size = 1;
    clock_get_attributes( clock_port, CLOCK_GET_TIME_RES, & value, & size);
    granularity = value * 1.e-9;
  }

  ~TimerClockGetTimeRealtimeClock() {
    mach_port_deallocate(mach_task_self(), clock_port);
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_get_time(clock_port, values+i);
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

    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &clock_port);
    int value;
    unsigned int size = 1;
    clock_get_attributes( clock_port, CLOCK_GET_TIME_RES, & value, & size);
    granularity = value * 1.e-9;
  }

  ~TimerClockGetTimeCalendarClock() {
    mach_port_deallocate(mach_task_self(), clock_port);
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      clock_get_time(clock_port, values+i);
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

    mach_timebase_info(& timebase_info);
    ticks_per_second = 1.e9 / timebase_info.numer * timebase_info.denom;
    granularity = 1.e-9 * timebase_info.numer / timebase_info.denom;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      values[i] = mach_absolute_time();
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
    granularity = 1e-6;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      gettimeofday(values+i, NULL);
  }
};


// getrusage(RUSAGE_SELF)
class TimerGetrusageSelf: public TimerBase<timeval> {
public:
  TimerGetrusageSelf() {
    description = "getrusage(RUSAGE_SELF)";
    granularity = 1e-6;
  }

  void measure() {
    rusage usage;
    for (unsigned int i = 0; i <= 2*SIZE; ++i) {
      getrusage(RUSAGE_SELF, &usage);
      values[i].tv_sec  = usage.ru_stime.tv_sec  + usage.ru_utime.tv_sec;
      values[i].tv_usec = usage.ru_stime.tv_usec + usage.ru_utime.tv_usec;
    }
  }
};


// omp_get_wtime()
// note: on Linux, the behaviour of this timer is affected by the choice of the clock source (/sys/devices/system/clocksource/clocksource0/current_clocksource)
class TimerOMPGetWtime : public TimerBase<double> {
public:
  TimerOMPGetWtime() {
    description = str(boost::format("omp_get_wtime() using %s clock source") % read_clock_source());
    granularity = omp_get_wtick();
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      values[i] = omp_get_wtime();
  }
};


// clock()
class TimerClock : public TimerBase<clock_t> {
public:
  TimerClock() {
    description = "clock()";
    ticks_per_second = CLOCKS_PER_SEC;
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i)
      values[i] = clock();
  }
};


// times()
class TimerTimes : public TimerBase<clock_t> {
public:
  TimerTimes() {
    description = "times()";
    ticks_per_second = sysconf(_SC_CLK_TCK);
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
  }

  void measure() {
    tms value;
    for (unsigned int i = 0; i <= 2*SIZE; ++i) {
      times(& value);
      values[i] = value.tms_stime + value.tms_utime;
    }
  }
};


// rdtsc()
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
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i) {
      values[i] = __rdtsc();
    }
  }

};



// lfence, rdtsc()
class TimerFenceRDTSC : public TimerBase<unsigned long long> {
public:
  TimerFenceRDTSC() {
    if (not tsc_allowed())
      throw std::runtime_error("RDTSC is disabled for the current proccess, calling it would result in a SIGSEGV (see 'PR_SET_TSC' under 'man prctl')");
    
    // ticks_per_second and granularity
    ticks_per_second  = calibrate_tsc();
    granularity = 1. / ticks_per_second;
    if (granularity < 1.e-9)
      granularity = 1.e-9;
    
    description = str(boost::format("lfence(); rdtsc() [estimated at %#5.4g GHz]") % (ticks_per_second / 1.e9));
  }

  void measure() {
    for (unsigned int i = 0; i <= 2*SIZE; ++i) {
      _mm_lfence();
      values[i] = __rdtsc();
    }
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
  }

  void measure() {
    unsigned int id = 0;
    for (unsigned int i = 0; i <= 2*SIZE; ++i) {
      values[i] = __rdtscp(& id);
    }
  }

};



int main(void) {
  std::vector<TimerInterface *> timers;

  timers.push_back(new TimerCxx11HighResolutionClock());

#ifdef HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID
  timers.push_back(new TimerClockGettimeThread());
#endif // HAVE_POSIX_CLOCK_THREAD_CPUTIME_ID

#ifdef HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID
  timers.push_back(new TimerClockGettimeProcess());
#endif // HAVE_POSIX_CLOCK_PROCESS_CPUTIME_ID

#ifdef HAVE_POSIX_CLOCK_REALTIME
  timers.push_back(new TimerClockGettimeRealtime());
#endif // HAVE_POSIX_CLOCK_REALTIME

#ifdef HAVE_POSIX_CLOCK_MONOTINIC
  timers.push_back(new TimerClockGettimeMonotonic());
#endif // HAVE_POSIX_CLOCK_MONOTINIC

#ifdef HAVE_POSIX_CLOCK_MONOTONIC_RAW
  timers.push_back(new TimerClockGettimeMonotonicRaw());
#endif // HAVE_POSIX_CLOCK_MONOTONIC_RAW

#ifdef HAVE_MACH_SYSTEM_CLOCK
  timers.push_back(new TimerClockGetTimeSystemClock());
#endif // HAVE_MACH_SYSTEM_CLOCK

#ifdef HAVE_MACH_REALTIME_CLOCK
  timers.push_back(new TimerClockGetTimeRealtimeClock());
#endif // HAVE_MACH_REALTIME_CLOCK

#ifdef HAVE_MACH_CALENDAR_CLOCK
  timers.push_back(new TimerClockGetTimeCalendarClock());
#endif // HAVE_MACH_CALENDAR_CLOCK

#ifdef HAVE_MACH_ABSOLUTE_TIME
  timers.push_back(new TimerMachAbsoluteTime());
#endif // HAVE_MACH_ABSOLUTE_TIME

  timers.push_back(new TimerGettimeofday());
  timers.push_back(new TimerGetrusageSelf());
  timers.push_back(new TimerOMPGetWtime());
  timers.push_back(new TimerClock());
  timers.push_back(new TimerTimes());

  if (tsc_allowed()) {
    timers.push_back(new TimerRDTSC());
    timers.push_back(new TimerFenceRDTSC());
    if (rdtscp_supported())
      timers.push_back(new TimerRDTSCP());
    else
      std::cout << "RDTSCP instruction not supported by this processor" << std::endl << std::endl;
  } else {
      std::cout << "access to the TSC by non-privileged processes has been disabled" << std::endl << std::endl;
  }

  std::cout << "For each timer the resolution reported is the MEDIAN (MEAN +/- its STDDEV) of the increments measured during the test." << std::endl << std::endl; 

  for (TimerInterface * timer: timers) {
    timer->measure();
    timer->compute();
    timer->report();
  }

  return 0;
}
