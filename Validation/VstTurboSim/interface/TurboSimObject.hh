
#ifndef __TurboSimObject_h
#define __TurboSimObject_h

class TurboSimEventData;

#include <string>
#include <vector>
#include <iostream>
#include "Validation/VstTurboSim/interface/TurboSimSaveableItem.hh"
#include "Validation/VstTurboSim/interface/TurboSimParticle.hh"

/**
 * An object is a singlet or clustering of particles 
 *
 *  TurboSimObject.h
 *  TurboSim
 *
 * @author David Friend on 7/3/05.
 *
 */

class TurboSimObject : public TurboSimSaveableItem, 
                       protected std::vector< TurboSimParticle > {
   
public:
   
   // Constructors
   TurboSimObject( );
   TurboSimObject( const TurboSimObject &original );
   TurboSimObject( const TurboSimEventData *ptrEventData,
                   double weight = 1.0  );
   TurboSimObject( const TurboSimParticle &particle,
                   const TurboSimEventData *ptrEventData = NULL,
                   double weight = 1.0 );
   TurboSimObject( const std::vector< TurboSimParticle> &particles,
                   const TurboSimEventData *ptrEventData = NULL,
                   double weight = 1.0 );
   
   // Object accessors
   double weight() const { return _weight; }
   int numParticles() const { return size(); }
   const TurboSimEventData *eventData() const { return _eventData; }
   CLHEP::HepLorentzVector lorentzVector() const;
   double mass() const;
   TurboSimParticle::StringFormat defaultStringFormat() const;
   std::string toString( ) const { return toString( defaultStringFormat() ); }
   std::string toString( int format,
                         int precision = 3 ) const;
   
   // Vector accessors
   const TurboSimParticle &particleAt( int index ) const {
      if ( index >= 0 && index < size() )
         return (*this)[ index ];
      else
         return DefinitionOf::nullParticle; }
   const TurboSimParticle &front() const { 
      return std::vector< TurboSimParticle >::front(); }
   const TurboSimParticle &back() const { 
      return std::vector< TurboSimParticle >::back(); }
   std::vector< TurboSimParticle >::const_iterator begin() const { 
      return std::vector< TurboSimParticle >::begin(); }
   std::vector< TurboSimParticle >::const_iterator end() const { 
      return std::vector< TurboSimParticle >::end(); }
   bool empty() const { 
      return std::vector< TurboSimParticle >::empty(); }
   int size() const { 
      return std::vector< TurboSimParticle >::size(); }

   // Object Modifiers
   bool weight( double weight ) {
      if ( weight > 0 ) { _weight = weight; return true; } return false; }
   const TurboSimEventData *eventData( const TurboSimEventData *ptr ) {
      const TurboSimEventData *tmp = _eventData;
      _eventData = ptr; return tmp; }
   bool read( std::istream &fin ) { return read( fin, 
                                                 defaultStringFormat() ); }
   bool read( std::istream &fin, 
	      int format );
   
   // Vector modifiers
   void clear() { std::vector< TurboSimParticle >::clear(); }
   TurboSimParticle &front() { 
      return std::vector< TurboSimParticle >::front(); }
   TurboSimParticle &back() { 
      return std::vector< TurboSimParticle >::back(); }
   std::vector< TurboSimParticle >::iterator begin() { 
      return std::vector< TurboSimParticle >::begin(); }
   std::vector< TurboSimParticle >::iterator end() { 
      return std::vector< TurboSimParticle >::end(); }
   std::vector< TurboSimParticle >::iterator erase( const std::vector< TurboSimParticle >::iterator &pos ) { 
      return std::vector< TurboSimParticle >::erase( pos ); }
   std::vector< TurboSimParticle >::iterator erase( const std::vector< TurboSimParticle >::iterator &first,
                                                    const std::vector< TurboSimParticle >::iterator &beyond ) {
      return std::vector< TurboSimParticle >::erase( first, beyond ); }
   void pop_back() { std::vector< TurboSimParticle >::pop_back(); }
   void push_back( const TurboSimParticle &particle ) { 
      std::vector< TurboSimParticle >::push_back( particle ); } 
   void insert( std::vector< TurboSimParticle >::iterator pos,
		std::vector< TurboSimParticle >::const_iterator first,
		std::vector< TurboSimParticle >::const_iterator beyond ) {
      std::vector< TurboSimParticle >::insert( pos,
					       first,
					       beyond );
   }
   
   // Operators
   
   // Assignment
   const TurboSimObject &operator=( const TurboSimObject &rhs );
   
   // Comparators
   bool operator==( const TurboSimObject &rhs ) const;
   bool equals( const TurboSimObject &rhs, double = 0.0 ) const;
   bool operator<( const TurboSimObject &rhs ) const;
   bool operator!=( const TurboSimObject &rhs ) const;
   bool operator>( const TurboSimObject &rhs ) const;
   
   // Merging
   TurboSimObject operator+( const TurboSimObject &rhs ) const;
   TurboSimObject operator+( const TurboSimParticle &rhs ) const;
   
private:
      
   const TurboSimEventData *_eventData;
   double _weight;
   
};

#include "Validation/VstTurboSim/interface/TurboSimEventData.hh"

#endif
