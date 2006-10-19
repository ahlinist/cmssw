#ifndef EtaPtBin_H
#define EtaPtBin_H

#include "TString.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

class EtaPtBin {

 public:

  EtaPtBin(bool etaActive_ , double etaMin_ , double etaMax_ ,
	    bool ptActive_  , double ptMin_  , double ptMax_ ) ;

  ~EtaPtBin () {} ;

  TString getDescriptionString () const { return descriptionString ; } 

  // method to build the string from other quantities
  // (static for easy external use)
  static TString buildDescriptionString 
  	( bool etaActive_ , double etaMin_ , double etaMax_ ,
	  bool ptActive_  , double ptMin_  , double ptMax_ ) ;  // pt
  
  
  bool   getEtaActive () const { return etaActive ; }
  double getEtaMin    () const { return etaMin    ; }
  double getEtaMax    () const { return etaMax    ; }

  bool   getPtActive () const { return ptActive ; }
  double getPtMin    () const { return ptMin    ; }
  double getPtMax    () const { return ptMax    ; }


  bool inBin(const double & eta , const double & pt) const;
  bool inBin(const reco::JetTag & jetTag) const;

 private:

  // definition of the bin

  bool   etaActive ; // should cuts be applied?
  double etaMin ;
  double etaMax ;
  
  bool   ptActive ; // should cuts be applied?
  double ptMin ;
  double ptMax ;
  

  // description string as built from bin definition
  TString descriptionString ;
  
  
} ;


#endif
