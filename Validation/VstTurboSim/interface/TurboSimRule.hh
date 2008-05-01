#ifndef __TurboSimRule_h
#define __TurboSimRule_h

#include <iostream>
#include <string>

#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstTurboSim/interface/TurboSimSaveableItem.hh"
#include "Validation/VstTurboSim/interface/TurboSimEvent.hh"
#include "Validation/VstTurboSim/interface/TurboSimRule_Utilities.hh"
#include "Validation/VstTurboSim/interface/TurboSimRule_Modifiers.hh"

//base class for TurboSim rules
class TurboSimRule : public TurboSimSaveableItem {

public:
   
   ~TurboSimRule() {}

   // Virtual Usage Functions

   /// Train rule ( produce a set of parameters ) 
   virtual bool train( TurboSimItem & );

   /// toString
   virtual std::string toString( ) const;

   /// read
   virtual bool read( istream & );

   /// Apply rule to event
   virtual bool apply( TurboSimItem  & ) const; 

   /// Description of rule
   const std::string &description() const {
      return _description;
   }

protected:

   // Constructors
   TurboSimRule( const std::string &desc )
      : _description( desc ) {}
   TurboSimRule( const TurboSimRule &original )
      : _description( original._description ) {}

private:  

   const std::string _description;

};

#endif
