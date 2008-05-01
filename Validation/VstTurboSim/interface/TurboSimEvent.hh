#ifndef __TurboSimEvent_h
#define __TurboSimEvent_h

#include <vector>
#include <string>
#include <iostream>

#include <cassert>
#include <algorithm>
#include "Validation/VstMathUtils/interface/Math.hh"

#include "Validation/VstTurboSim/interface/TurboSimParticle.hh"
#include "Validation/VstTurboSim/interface/TurboSimParticleType.hh"
#include "Validation/VstTurboSim/interface/TurboSimPartonObject.hh"
#include "Validation/VstTurboSim/interface/TurboSimRecoObject.hh"
#include "Validation/VstTurboSim/interface/TurboSimEventData.hh"
#include "Validation/VstTurboSim/interface/TurboSimSaveableItem.hh"

#include "Validation/VstTurboSim/interface/TurboSimEvent_ParticleIterator.hh"

/**
 * Templated storage to group parton and reco items.
 *
 * Class serves two purposes. (1) It contains all data about an
 * event including <code>EventData</code>. (2) It contains parton
 * and reco objects. One might use it to contain the parton and 
 * reco particles read in from a file. It might also contain
 * <code>PartonObject</code>s and <code>RecoObject</code>s whose
 * association form the basis for the morphisms table. Templated
 * types must be derived from <code>TurboSimSavableItem</code>s.
 *
 * @author David Friend, friend at mit dot edu
 * @version 2005.07.07
 */

class TurboSimEvent : public TurboSimEventData,
		      public TurboSimSaveableItem {

 public:

   //-------------------
   // Particle Iterators
   //-------------------

   /// Parton Particle Iterator
   class parton_particle_iterator 
      : public particle_iterator< TurboSimPartonObject > {
      friend class TurboSimEvent;
     parton_particle_iterator( std::vector< TurboSimPartonObject> &objs,
				particle_iterator<TurboSimPartonObject>::
				IteratorPosition startPos =
				particle_iterator<TurboSimPartonObject>::BEGIN )
	 : particle_iterator< TurboSimPartonObject >( objs, 
						      startPos ) {}
   };

   /// Reco Particle Iterator
   class reco_particle_iterator 
      : public particle_iterator< TurboSimRecoObject > {
      friend class TurboSimEvent;
     reco_particle_iterator( std::vector< TurboSimRecoObject> &objs,
			      particle_iterator<TurboSimRecoObject>::
			      IteratorPosition startPos =
			      particle_iterator<TurboSimRecoObject>::BEGIN )
	 : particle_iterator< TurboSimRecoObject >( objs, 
						    startPos ) {}
   };
   

   parton_particle_iterator partonParticlesBegin() {
      return parton_particle_iterator( partonObjs(), 
				       particle_iterator<TurboSimPartonObject>::
				       BEGIN );
   }
   parton_particle_iterator partonParticlesEnd() {
      return parton_particle_iterator( partonObjs(), 
				       particle_iterator<TurboSimPartonObject>::
				       END );
   }

   reco_particle_iterator recoParticlesBegin() {
      return reco_particle_iterator( recoObjs(), 
				     particle_iterator<TurboSimRecoObject>::
				     BEGIN );
   }
   reco_particle_iterator recoParticlesEnd() {
      return reco_particle_iterator( recoObjs(), 
				     particle_iterator<TurboSimRecoObject>::
				     END );
   }
   
   static reco_particle_iterator 
   eraseParticle( reco_particle_iterator &pos ) {
      if ( pos.end() )
	 return pos;
      pos.object()->erase( pos.particle() );
      if ( pos.object()->empty() )
	 pos.objects().erase( pos.object() );
      return pos;
   }

   static parton_particle_iterator 
   eraseParticle( parton_particle_iterator &pos ) {
      if ( pos.end() )
	 return pos;
      pos.object()->erase( pos.particle() );
      if ( pos.object()->empty() ) 
	 pos.objects().erase( pos.object() );
      return pos;
   }

   // ------------
   // Constructors
   // ------------
   
   /**
    * Creates a new <code>TurboSimEvent</code> object with uninitialized
    * values.
    */
   TurboSimEvent();
   
   /** 
    * Creates a new <code>TurboSimEvent</code> object from given values.
    *
    * @param eventInfo Event descriptor (a <code>TurboSimEventData</code> object)
    * @param weight    Statistical weight of the event (a probability)
    */
   TurboSimEvent( const TurboSimEventData &eventInfo,
		  double weight = 1.0 ); 
   
   /**
    * Inlined accessor that returns statistical weight of event.
    *
    * @return statistical weight of event.
    */ 
   double weight() const { return _weight; }
   
   /** 
    * Accessor that generates a standard string representation of the event
    * data fields. Format varies depending on collider:
    * <pre>
    *   4f 10.86 1 e+e- 205
    *   ^  ^     ^ ^    ^
    *   1  2     3 4    5  
    * <pre>
    * <ol> <li><pre>4f</pre> is the intermediate state of event</li>
    *      <li><pre>10.86</pre> is the run number and event number</li>
    *      <li><pre>1</pre> is ???
    *      <li><pre>e+e-</pre> are the colliding particles</li>
    *      <li><pre>205</pre> is ???</li></ol>
    *
    * @return A string representation of the event descriptor data.
    */
   std::string toString() const;

   /**
    */
   std::string finalState() const;

   /**
    */
   std::string baseFinalState() const;

   /**
    * Accessor for parton objects. Do it cheaply by returning 
    * a <code>const</code> reference to the parton objects vector.
    *
    * @returns Reference to <code>vector</code> of parton objects
    */
  const std::vector< TurboSimPartonObject > &partonObjs() const {
      return _partonObjs; }

   /** 
    * Accessor for a single parton object.
    *
    * @param index Index of parton object
    * @returns Reference to parton object. Calling with an invalid index returns 
               a reference to NULL.
    */
   const TurboSimPartonObject &partonObj( int index ) const {
      if ( index >= 0 && index < _partonObjs.size() )
	 return _partonObjs[ index ];
      else {
	 std::cerr << "Invalid index reference for event partonObjs: " << index;
	 exit( EXIT_FAILURE );
      }
   }

   /**
    * Accessor for reco objects. Do it cheaply by returning 
    * a <code>const</code> reference to the reco objects vector.
    *
    * @retur Reference to <code>vector</code> of reconstructed items
    * @todo Create more accessors for recoItems.
    */
  const std::vector< TurboSimRecoObject > &recoObjs() const {
      return _recoObjs; }

   /**
    * Accessor for a single reco object.
    *
    * @param index Index of reco object.
    * @return Reference to the object. NULL if invalid index.
    */
   const TurboSimRecoObject &recoObj( int index ) const {
      if ( index >= 0 && index < _recoObjs.size() )
	 return _recoObjs[ index ];
      else {
	 std::cerr << "Invalid index reference for event recoObjs: " << index;
	 exit( EXIT_FAILURE );
      } 
   }

   /**
    * Comparator checks equality between two events.
    * 
    * @param rhs Right hand side <code>TurboSimEvent</code>
    * @returns <code>true</code> if events are equal. <code>false</code> otherwise.
    */
   bool operator==( const TurboSimEvent &rhs ) const;

   // MODIFIERS

   /**
    * Modifier for parton items. Do it cheaply by returning 
    * a non-<code>const</code> reference to the parton items vector.
    *
    * @returns Reference to <code>vector</code> of parton items
    * @todo Create more modifiers for partonItems.
    */
  std::vector< TurboSimPartonObject > &partonObjs() {
      return _partonObjs; }
   
  std::vector< TurboSimPartonObject >::iterator
  removePartonObj( std::vector< TurboSimPartonObject >::iterator &obj ) {
      return _partonObjs.erase( obj );
   }
   
  std::vector< TurboSimPartonObject >::iterator 
  mergePartonObjs( std::vector< TurboSimPartonObject >::iterator &dest,
		   std::vector< TurboSimPartonObject >::iterator &data ) {
      if ( dest == data )
	 return dest + 1;
      dest->insert( dest->end(),
		    data->begin(),
		    data->end() );
      removePartonObj( data );
      return dest + 1;
   }
   
   /**
    * Modifier for reco items. Do it cheaply by returning 
    * a non-<code>const</code> reference to the reco items vector.
    *
    * @returns Reference to <code>vector</code> of reconstructed items
    */
  std::vector< TurboSimRecoObject > &recoObjs() {
      return _recoObjs; }

  std::vector< TurboSimRecoObject >::iterator
  removeRecoObj( std::vector< TurboSimRecoObject >::iterator &obj ) {
      return _recoObjs.erase( obj ); 
   }

  std::vector< TurboSimRecoObject >::iterator 
  mergeRecoObjs( std::vector< TurboSimRecoObject >::iterator &dest,
		 std::vector< TurboSimRecoObject >::iterator &data ) {
      if ( dest == data )
	 return dest + 1;
      dest->insert( dest->end(),
		    data->begin(),
		    data->end() );
      removeRecoObj( data );
      return dest + 1;
   }

   // other modifiers
   void setWeight( double wt ) {
      _weight = wt; }

   /** 
    * Modifier that reads event data in from an open input stream.
    * 
    * @param source Input stream.
    * @return <code>true</code> if read was successful.
    */ 
   bool read( std::istream &source ) {
      return read( source, 
		   TurboSimParticle::DEFAULT,
		   EVENT_DATA ); 
   }

   /**
    * Describes what data should be extracted from the read
    * 
    * <code>EVENT_DATA</code> tells read to extract event data from the stream.
    *
    * <code>PARTON_ITEMS</code> tells read to extract many parton items from
    * the stream. Read will extract parton items until the extraction fails.
    * Extraction failure is ususally caused by the object delimiter, as set
    * in <code>TurboSimParticleType</code> with the statement
    * <pre>  #define DELIMITER_VALUE ';'  //< must be a char </pre>
    *
    * <code>RECO_ITEMS</code> tells read to extract many reco items from the
    * stream. Read will extract reco items using the <code>recoItemType</code>'s
    * <code>.read( . . . )</code> function until the extraction fails, i.e.
    * <code>recoItemType.read( . . . )</code> returns false. Extraction failure
    * is usually caused by the object delimiter, as set in
    * <code>TurboSimParticleType</code> with the statement
    * <pre>  #define DELIMITER_VALUE ';'  //< must be a char </pre>
    *
    * <code>PARTON_ITEM</code> tells read to extract only one partonItem from
    * the stream. This should not be used with <code>PARTON_ITEMS</code>.
    * If used with the <code>PARTON_ITEMS</code> option, read will default
    * to <code>PARTON_ITEMS</code>.
    * 
    * <code>RECO_ITEM</code> tells read to extract only one recoItem from
    * the stream. This should not be used with <code>RECO_ITEMS</code>.
    * If used with the <code>RECO_ITEMS</code> option, read will default to
    * <code>PARTON_ITEMS</code>.
    *
    * If both parton item(s) and reco item(s) are specified to be read from the
    * stream, read will expect a -> delimiter between the two types.
    */
   enum { EVENT_DATA = 1,    
	  PARTON_OBJS = 2,
	  PARTON_OBJ = 4,
	  RECO_OBJS = 8,
	  RECO_OBJ = 16 };



   bool read( std::istream&,
	      int,
	      int );


private:

  double _weight;
  std::vector< TurboSimPartonObject > _partonObjs;
  std::vector< TurboSimRecoObject > _recoObjs;

};

#endif
