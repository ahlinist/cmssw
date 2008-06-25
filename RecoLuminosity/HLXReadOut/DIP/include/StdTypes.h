#ifndef DIP_STDTYPES_H_INCLUDED
#define DIP_STDTYPES_H_INCLUDED

#include <string>
#include <iostream>

#ifdef WIN32
typedef __int64		longlong;		// for millisecond timestamps
#else
#include <sys/types.h> 
typedef __int64_t	longlong;
#endif


typedef bool DipBool;
typedef unsigned char DipByte;
typedef short DipShort;
typedef long DipInt;
typedef longlong DipLong;
typedef float DipFloat;
typedef double DipDouble; 


#endif //DIP_STDTYPES_H_INCLUDED

