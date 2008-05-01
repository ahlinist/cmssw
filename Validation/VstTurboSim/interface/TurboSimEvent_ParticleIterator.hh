#ifndef __TurboSimEvent_ParticleIterator_
#define __TurboSimEvent_ParticleIterator_

#include<ostream>
#include<vector>
#include "Validation/VstTurboSim/interface/TurboSimParticle.hh"
#include "Validation/VstTurboSim/interface/TurboSimObject.hh"
#include "Validation/VstTurboSim/interface/TurboSimRecoObject.hh"
#include "Validation/VstTurboSim/interface/TurboSimPartonObject.hh"


/// Base class
/// protected constructor

template< typename objType = TurboSimObject>
class particle_iterator {

public:

  /* particle_iterator( const particle_iterator< objType > &original ) 
      : _objs( original._objs ),
	_p( original._p ),
	_o( original._o ) {} */

   bool operator==( const particle_iterator< objType > &rhs ) const {
      if ( _o == rhs._o  )
	 if ( end() )
	    return true;
	 else
	    return ( _p == rhs._p );
      else
	 return false;
   }

   bool operator!=( const particle_iterator< objType > &rhs ) const {
      return ( not ( *this == rhs ) );
   }

   bool end() const {
      return ( _o == _objs.end() );
   }

   const particle_iterator< objType > 
   &operator=( const particle_iterator< objType > &original ) {
      if ( _objs != original._objs ) {
	std::cerr << "Cannot assign particle_iterator of one event "
	      << "to a particle_iterator of a different event.";
	 exit( EXIT_FAILURE );
      }
      _p = original._p;
      _o = original._o;
   }

   particle_iterator< objType > &operator++() {
      if ( end() )
	 return *this;
      if ( ++_p == _o->end() ) {
	 do {
	    ++_o;
	 } while ( ( _o->begin() == _o->end() ) &&
		   not end() );
	 if ( end() ) 
	    return *this;
	 _p = _o->begin();
      }
      return *this;
   }

   particle_iterator< objType > operator++( int  ) {
      particle_iterator< objType > t = *this;
      ++(*this);
      return t;
   }

   const TurboSimParticle &operator*() const {
      if ( not end() ) {
	 return *_p;
      }
      else {
	std::cerr << "Attempt to dereference particle_iterator at end "
	      << "of particle sequence.";
	 exit( EXIT_FAILURE );
      } 
   }

   TurboSimParticle &operator*() {
      if ( not end() ) {
	 return *_p;
      }
      else {
	std::cerr << "Attempt to dereference particle_iterator at end "
	      << "of particle sequence.";
	 exit( EXIT_FAILURE );
      } 
   }

   TurboSimParticle *operator->() {
      return &(*_p); 
   }

   TurboSimParticle const *operator->() const {
      return &(*_p); 
   }

protected:

  enum IteratorPosition { BEGIN = 0, END = 1 };

  particle_iterator( std::vector< objType > &objs,
		      IteratorPosition startPos = BEGIN ) 
      : _objs( objs ) 
   {
      if ( startPos == BEGIN ) {
	 if ( _objs.begin() == _objs.end() ) {
	    _o = _objs.end();
	    return;
	 }
	 _o = _objs.begin();
	 while ( ( _o->begin() == _o->end() ) &&
		 not end() )
	    _o++;
	 if ( end() ) {
	    return;
	 }
	 // we know that *_o is not empty
	 _p = _o->begin();
      }
      else if ( startPos == END ) {
	 _o = _objs.end();
      } 
   }

  std::vector< TurboSimParticle >::iterator particle() {
      return _p;
   }

  typename std::vector< objType >::iterator object() {
      return _o;
   }

  std::vector< objType > &objects() {
      return _objs;
   }

private:

  std::vector< objType > &_objs;
  std::vector< TurboSimParticle >::iterator _p;
  typename std::vector< objType >::iterator _o;


};

#endif
