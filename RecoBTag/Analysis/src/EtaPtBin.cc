#include "RecoBTag/Analysis/interface/EtaPtBin.h"




EtaPtBin::EtaPtBin ( bool etaActive_ , double etaMin_ , double etaMax_ ,
		     bool ptActive_  , double ptMin_  , double ptMax_ )
  : etaActive ( etaActive_ ) , etaMin ( etaMin_ ) , etaMax ( etaMax_ ) ,
    ptActive  (  ptActive_ ) , ptMin  (  ptMin_ ) , ptMax  (  ptMax_ )   {

  descriptionString = buildDescriptionString ( etaActive , etaMin , etaMax ,
					       ptActive  , ptMin  , ptMax  );
}



TString EtaPtBin::buildDescriptionString
		( bool etaActive_ , double etaMin_ , double etaMax_ ,
		  bool ptActive_  , double ptMin_  , double ptMax_)
{
  // create string only from the active parts
  TString descr ( "" );

  if ( etaActive_ ) {
    descr += "__ETA_";
    descr += etaMin_;
    descr += "-";
    descr += etaMax_;
  }

  if ( ptActive_ ) {
    descr += "__PT_";
    descr += ptMin_;
    descr += "-";
    descr += ptMax_;
  }
  if (!(etaActive_||ptActive_)) descr="__GLOBAL";
  // remove blanks which are introduced when adding doubles
  descr.ReplaceAll ( " " , "" );

  return descr;
}

bool EtaPtBin::inBin(const reco::JetTag & jetTag) const
{
  return inBin(jetTag.jet().eta(), jetTag.jet().pt());
}


bool EtaPtBin::inBin (const double & eta , const double & pt ) const {
  bool inEta = true;
  //
  if ( etaActive ) {
    if ( fabs(eta) < etaMin ) inEta = false;
    if ( fabs(eta) > etaMax ) inEta = false;
  }

  bool inPt = true;
  //
  if ( ptActive ) {
    if ( pt < ptMin ) inPt = false;
    if ( pt > ptMax ) inEta = false;
  }

  return ( inEta && inPt );
}
