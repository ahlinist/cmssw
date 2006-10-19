#ifndef AcceptJet_H
#define AcceptJet_H

#include "DataFormats/BTauReco/interface/JetTag.h"

class AcceptJet {

 public:
  AcceptJet();
  // event selection
  bool operator() (const reco::JetTag & jetTag) const;

  // set cut parameters
  void setEtaMin            ( double d ) { etaMin            = d ; } 
  void setEtaMax            ( double d ) { etaMax            = d ; } 
  void setPPartonMin        ( double d ) { pPartonMin        = d ; } 
  void setPPartonMax        ( double d ) { pPartonMax        = d ; } 
  void setPtRecJetMin       ( double d ) { ptRecJetMin       = d ; } 
  void setPtRecJetMax       ( double d ) { ptRecJetMax       = d ; } 
  void setPRecJetMin        ( double d ) { pRecJetMin        = d ; } 
  void setPRecJetMax        ( double d ) { pRecJetMax        = d ; } 

  void setPtPartonMin       ( double d ) { ptPartonMin       = d ; } 
  void setPtPartonMax       ( double d ) { ptPartonMax       = d ; } 

 protected:

  // eta range 
  double etaMin ;   // these are meant as |eta| !!
  double etaMax ;

  // parton p
  double pPartonMin ;
  double pPartonMax ;

  // parton pt
  double ptPartonMin ;
  double ptPartonMax ;

  // rec. jet
  double ptRecJetMin ;
  double ptRecJetMax ;
  //
  double pRecJetMin  ;
  double pRecJetMax  ;
  
  
} ;

#endif
