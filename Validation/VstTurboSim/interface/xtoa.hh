/*
 *  xtoa.h
 *  TurboSim
 *
 *  Created by David Friend on 7/3/05.
 *
 */

#ifndef __xtoa_h
#define __xtoa_h

#include <strstream>
#include <iostream>
#include <iomanip>
#include <string>

class xtoa : public std::strstream {
 
public:
   
   // Constructor
   xtoa() {}

   // Construct with value
   template <typename Type>
   xtoa( Type x, int precision = -1 ) {
      if ( precision >= 0 ) {
	 this->setf( std::ios::fixed );
         this->precision( precision );
      }
      *this << x; 
   }
   
   // Convert to string
   operator std::string() {
      return this->str();
   }   

   std::string toString() {
      return this->str();
   }

   // assign an object a new ( numerical ) value
   // x2a converter;
   // cout << ( converter = 12 );
   template <typename Type>
   const xtoa &operator=( const Type x ) {
      str( "" );  // hack to clear stringstream buffer
                  // .clear() doesn't work.
      *this << x;
      return *this;
   }
   
   // use an object as a function:
   // x2a converter;
   // cout << converter.toString( 12 );
   template <typename Type>
   std::string toString( const Type x ) {
      return ( *this = x ).str();
   }

   // use an object as a function:
   // x2a converter;
   // cout << converter( 12 );
   template <typename Type>
   std::string operator()( const Type x ) {
      return ( *this = x ).str();
   }

};


#endif
