#ifndef __TurboSimRule_DiscardLowPt_h
#define __TurboSimRule_DiscardLowPt_h

#include "Validation/VstTurboSim/interface/TurboSimRule.hh"

#define DiscardLowPt "Removes particles of low Pt from event"

class TurboSimRule_DiscardLowPt : public TurboSimRule {

public:

   TurboSimRule_DiscardLowPt( double min = 3.0 ) 
      : _minimumPt( min ),
	TurboSimRule( std::string( DiscardLowPt ) ) { 
      if ( _minimumPt < 0.0 )
	 _minimumPt = 0.0;
   }

   bool train( TurboSimEvent &event );
   
   std::string toString() const {
      return xtoa( _minimumPt, 3 ).toString(); }

   bool read( istream &);

   bool apply( TurboSimEvent &event ) const;
   
private:

   double _minimumPt;

};

#endif
