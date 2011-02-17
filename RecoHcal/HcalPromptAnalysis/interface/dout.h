#if !defined(DoutH)
#define DoutH

#if !defined(_DEBUG_QUIET)
#define dout std::cout << __FILE__ << "(" << __LINE__ << ") "
#else
#define dout if (0) std::cout
#endif

#endif
