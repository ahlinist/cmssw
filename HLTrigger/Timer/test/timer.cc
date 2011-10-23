#include <iostream>
#include <iomanip>
// for timespec and clock_gettime
#include <time.h>
// for timeval and gettimeofday
#include <sys/time.h>
// for timeval and getrusage
#include <sys/resource.h>

#define SIZE 1000000

timespec values_ns1[2*SIZE+1];
timespec values_ns2[2*SIZE+1];
timespec values_ns3[2*SIZE+1];
timeval  values_us1[2*SIZE+1];
timeval  values_us2[2*SIZE+1];

void measure_clock_gettime_thread(timespec values[]) {
  for (unsigned int i = 0; i <= 2*SIZE; ++i)
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, values+i);
}

void measure_clock_gettime_process(timespec values[]) {
  for (unsigned int i = 0; i <= 2*SIZE; ++i)
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, values+i);
}

void measure_clock_gettime_realtime(timespec values[]) {
  for (unsigned int i = 0; i <= 2*SIZE; ++i)
    clock_gettime(CLOCK_REALTIME, values+i);
}

void measure_gettimeofday(timeval values[]) {
  for (unsigned int i = 0; i <= 2*SIZE; ++i)
    gettimeofday(values+i, NULL);
}

void measure_getrusage(timeval values[]) {
  rusage usage;
  for (unsigned int i = 0; i <= 2*SIZE; ++i) {
    getrusage(RUSAGE_SELF, &usage);
    values[i].tv_sec  = usage.ru_stime.tv_sec  + usage.ru_utime.tv_sec;
    values[i].tv_usec = usage.ru_stime.tv_usec + usage.ru_utime.tv_usec;
  }
}

// timespec has a ns resolution
double delta(const timespec & start, const timespec & stop) {
  if (stop.tv_nsec > start.tv_nsec)
    return (double) (stop.tv_sec - start.tv_sec) + (double) (stop.tv_nsec - start.tv_nsec) / (double) 1e9;
  else
    return (double) (stop.tv_sec - start.tv_sec) - (double) (start.tv_nsec - stop.tv_nsec) / (double) 1e9;
}

// timeval has a us resolution
double delta(const timeval & start, const timeval & stop) {
  if (stop.tv_usec > start.tv_usec)
    return (double) (stop.tv_sec - start.tv_sec) + (double) (stop.tv_usec - start.tv_usec) / (double) 1e6;
  else
    return (double) (stop.tv_sec - start.tv_sec) - (double) (start.tv_usec - stop.tv_usec) / (double) 1e6;
}

template <typename T>
void report(const T values[]) {
  // set up
  std::cout << std::setprecision(0) << std::fixed;

  // average
  std::cout << "Average time per call: " << delta( values[SIZE], values[2*SIZE] ) / (double) SIZE * 1e9 << " ns" << std::endl;

  // resolution (average of increments)
  unsigned int steps = 0;
  for (unsigned int i = 0; i < SIZE; ++i) {
    double step = delta( values[SIZE + i], values[SIZE + i + 1] );
    if (step > 0) {
      //std::cout << std::setw(8) << std::right << i << std::setw(16) << std::right << step * 1e9 << " ns" << std::endl;
      ++steps;
    }
  }
  double resolution = (steps) ? delta( values[SIZE], values[2*SIZE] ) / (double) steps * 1e9: 0.;
  std::cout << "Resolution:            " << resolution << " ns" << std::endl;
}

int main(void) {
  measure_clock_gettime_thread(values_ns1);
  measure_clock_gettime_process(values_ns2);
  measure_clock_gettime_realtime(values_ns3);
  measure_getrusage(values_us1);
  measure_gettimeofday(values_us2);

  std::cout << "Test results for \"clock_gettime(CLOCK_THREAD_CPUTIME_ID, & value)\"" << std::endl;
  report(values_ns1);
  std::cout << std::endl;

  std::cout << "Test results for \"clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & value)\"" << std::endl;
  report(values_ns2);
  std::cout << std::endl;

  std::cout << "Test results for \"clock_gettime(CLOCK_REALTIME, & value)\"" << std::endl;
  report(values_ns3);
  std::cout << std::endl;

  std::cout << "Test results for \"getrusage(RUSAGE_SELF, & value)\"" << std::endl;
  report(values_us1);
  std::cout << std::endl;

  std::cout << "Test results for \"gettimeofday(& value, NULL)\"" << std::endl;
  report(values_us2);
  std::cout << std::endl;

  return 0;
}
