#include <iostream>
using namespace std;

#include "RecoBTag/MCTools/interface/MCParticleInfo.h"

MCParticleInfo::MCParticleInfo( int LC ) {
  reset();
  setCode( LC );
}

MCParticleInfo::~MCParticleInfo() {
}

void MCParticleInfo::reset () {
  isInitialised = false;
  // particle info
  lundCode_                = 0;
  lundCodeOfHeaviestQuark_ = 0;
  // for hadrons
  isHadron_        = false;
  isBottomHadron_  = false;
  isCharmHadron_   = false;
  isStrangeHadron_ = false;
  hasStrangeness_  = false;
  isBaryon_        = false;
  // for partons
  isParton_  = false;
  isD_       = false;
  isU_       = false;
  isS_       = false;
  isC_       = false;
  isB_       = false;
  isGluon_   = false;
  isQuark_   = false;
  // for light charged leptons
  isLepton_  = false;
}

void MCParticleInfo::setCode( int LC ) {
  lundCode_ = LC;
  // decode this code and set data members accordingly
  // decompose in digits
  int kfa = abs(lundCode_);
  int kf2 = kfa;
  int n4  = kf2 / 10000;
  kf2 = kf2 - n4*10000; 
  int n3  = kf2 / 1000;
  kf2 = kf2 - n3*1000 ;
  int n2  = kf2 / 100;
  kf2 = kf2 - n2*100;
  int n1  = kf2 / 10;
  kf2 = kf2 - n1*10; 

  int isign;
  if ( lundCode_ >= 0 ) {
    isign = 1;
  }
  else {
    isign = -1;
  }

  //hadron?
  isHadron_ = false;
  if ( n3 != 0 || n2 != 0 ) isHadron_ = true;
  
  //baryon?
  isBaryon_ = false;
  if ( n3 != 0 ) isBaryon_ = true;

// imax:      if baryon: n3
//            meson : n2 * (-1)**max(...)
// def.: imax > 0 for quark; < 0 for antiquark      
// strange: if baryon: n3 or n2 or n1
//          meson :    n2 or n1
  
  int iqmax; // flavour of heaviest quark
  hasStrangeness_ = false;
      
  if ( isBaryon_ ) {
    iqmax = n3;
    iqmax = iqmax * isign;
    if ( n3==3 || n2==3 || n1==3 ) hasStrangeness_ = true;
  }
  else {
    iqmax = n2;
    // adjust sign
    iqmax = iqmax * isign;
    for ( int ii=1; ii<=n2; ii++ ) iqmax = iqmax * -1; 
    if (n2==3 || n1==3) hasStrangeness_ = true;
  }

  // hadron type
  isBottomHadron_  = false;
  isCharmHadron_   = false;
  isStrangeHadron_ = false;
  if ( abs(iqmax) == 5 ) isBottomHadron_  = true;
  if ( abs(iqmax) == 4 ) isCharmHadron_   = true;
  if ( abs(iqmax) == 3 ) isStrangeHadron_ = true;

  
  // partons
  if ( abs ( lundCode_ ) ==  1 ) isD_     = true ; 
  if ( abs ( lundCode_ ) ==  2 ) isU_     = true ; 
  if ( abs ( lundCode_ ) ==  3 ) isS_     = true ; 
  if ( abs ( lundCode_ ) ==  4 ) isC_     = true ; 
  if ( abs ( lundCode_ ) ==  5 ) isB_     = true ; 
  if ( abs ( lundCode_ ) == 21 ) isGluon_ = true ; 
  isParton_ = isD_ || isU_ || isS_ || isC_ || isB_ || isGluon_ ;
  isQuark_  = isD_ || isU_ || isS_ || isC_ || isB_ ;

  lundCodeOfHeaviestQuark_ = iqmax ;
  
  // light charged leptons
  if (( abs ( lundCode_ ) == 11 ) ||
      ( abs ( lundCode_ ) == 13 )) isLepton_ = true;
  
  // now it's initialised properly
  isInitialised = true;
}

void MCParticleInfo::print() const {
  // print all particle Info
  cout << "--> MCParticleInfo:" << endl;
  cout << "--> LundCode               :" << lundCode_                << endl;
  cout << "--> LundCodeOfHeaviestQuark:" << lundCodeOfHeaviestQuark_ << endl;
  cout << "--> BottomHadron           :" << isBottomHadron_          << endl;
  cout << "--> CharmHadron            :" << isCharmHadron_           << endl;
  cout << "--> StrangeHadron          :" << isStrangeHadron_         << endl;
  cout << "--> strange                :" << hasStrangeness_          << endl;
  cout << "--> baryon                 :" << isBaryon_                << endl;
  cout << "--> Parton                 :" << isParton_                << endl;
  cout << "--> D                      :" << isD_                     << endl;
  cout << "--> U                      :" << isU_                     << endl;
  cout << "--> S                      :" << isS_                     << endl;
  cout << "--> C                      :" << isC_                     << endl;
  cout << "--> B                      :" << isB_                     << endl;
  cout << "--> Gluon                  :" << isGluon_                 << endl;
  cout << "--> Quark                  :" << isQuark_                 << endl;
  cout << "--> Lepton                 :" << isLepton_                << endl;
}

