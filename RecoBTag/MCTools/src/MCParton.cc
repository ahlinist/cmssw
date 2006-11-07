#include "RecoBTag/MCTools/interface/MCParton.h"

using namespace HepMC;
using namespace std;

MCParton::MCParton() {
  reset() ;
}


MCParton::MCParton(const HepMC::GenParticle* particle, const HepMC::GenEvent* event,
		const HepMC::GenParticle* lastParton) : 
		MCBaseParticle(particle,event),
		lastParton_(lastParton)
{
  reset();
  setParticleProperties( );
}

void MCParton::reset ( ) {
  // reset (additional) data members
  flavour_             = 0  ;
  flavourAbs_          = 0  ;
  statusPythia_        = -1 ;
  motherLundCode_      = 0 ;
  motherStatusPythia_  = -1 ;
  fromPrimaryProcess_  = false ;
  fromRadiation_       = false ;
  fromGluonSplitting_  = false ;
  isFinalParton_       = false ;
  //  radiatesGluon_       = false ;
  splitsToD_           = false ; 
  splitsToU_           = false ; 
  splitsToS_           = false ; 
  splitsToC_           = false ; 
  splitsToB_           = false ;
  //  splitsToGluon_       = false ;

  daughterLines_.clear();
  summedDaughters_.SetPx (0.0) ;
  summedDaughters_.SetPy (0.0) ;
  summedDaughters_.SetPz (0.0) ;
  summedDaughters_.SetE  (0.0) ;

  isInitialParton_       = false ;
  initialPartonHasCloseHF_ = false ;
}


void MCParton::setParticleProperties( ) {
  //
  // only works properly if full history available!!
  //

  flavour_      = particleInfo().lundCode () ;
  flavourAbs_   = abs ( flavour_ ) ;
  statusPythia_ = hepParticle->status() ;

  mother_ = hepParticle->mother()  ;

  bool hasMother = false ;

  MCParticleInfo motherParticleInfo ;

  if ( hepParticle->mother() != 0 ) {
    motherParticleInfo.setCode ( mother_->pdg_id()) ;
    motherLundCode_ =  motherParticleInfo.lundCode() ;
    motherStatusPythia_ = mother_->status() ;
    hasMother = true ;
  }

// ThS: Seems to be useless!!!
//   const GenParticle *mother2 = hepParticle->secondMother()  ;
//   if ( indexMother2 > 0 ) {
//     MCParticleInfo Mother2 ( rhe->getParticle(indexMother2)->pid() ) ; 
//   }

  
  // for status==3 partons: sum over status 2 daughters
  HepMC::GenEvent::particle_const_iterator p;
  if ( statusPythia_==3 ) {
    for (p = hepEvent->particles_begin(); (*p) != lastParton_; ++p) {
      int statusP = (**p).status() ;
      if ( statusP==2 && (**p).mother() == hepParticle ) {
//       cout << "found daughter at "<<(**p).barcode()<<endl;
	daughterLines_.push_back (*p) ;
	summedDaughters_ += math::XYZTLorentzVector ( (**p).momentum() );
      } // ThS: compare with the daughters from the parton itself!
    }    
  }
// cout << "But: "<<hepParticle->beginDaughters()->barcode()<<" - "
// <<hepParticle->endDaughters()->barcode()<<" - "
// <<hepParticle->listChildren ().size()<<" - "
// <<daughterLines_.size()<<endl;
  // set it as initial parton if there have been daughters and the mother is not the proton

  if ( daughterLines_.size() >= 1 && motherLundCode_ != 2212 ) isInitialParton_ = true ;

//   if ( isInitialParton_ ) {
//     cout << "initial parton found " << endl ;
//     cout << "summed daughters : " << summedDaughters_ << endl ;
// 
// //     for (std::vector< GenParticle * >::iterator	pp = hepParticle->listChildren ().begin(); pp != hepParticle->listChildren ().end(); ++pp) {
// //    cout << "But: "<<(**pp).barcode()<<endl;
// //    cout << "But: "<<(**pp).mother()->barcode()<<" - "<<(**pp).barcode()<<endl;
// //   }
//   }

  // to allow to clean up later on:
  // check if there are other heavy flavour partons within a cone of certain size
  // which do not origin from the initial parton
  if (isInitialParton_) {
    for (p = hepEvent->particles_begin(); (*p) != lastParton_; ++p) {
      int statusP = (**p).status() ;
      MCParticleInfo lundCodeP((**p).pdg_id()) ;
      int flavourP = abs ( lundCodeP.lundCode() ) ;
      double deltaRP = (**p).momentum().deltaR(hepParticle->momentum()) ;
      if ( statusP==2 &&
	   (**p).mother() != hepParticle &&
	   (flavourP==4 || flavourP==5)    &&
	   deltaRP<0.8 ) {
	initialPartonHasCloseHF_ = true ;
      }
    }    
  }
  
  
  
  // is it primary after rad. (the real primary ones are the ones with Status 3!!)
  // yes, if mother itself is a parton and has 'documentation status' = 3
  if ( hasMother && motherParticleInfo.isParton() && motherStatusPythia_ == 3 )
	fromPrimaryProcess_ = true ;
  // but: if it has a status==3 daughter, don't set it!
  for (p = hepEvent->particles_begin(); (*p) != lastParton_; ++p) {
    if ( ((**p).status() == 3)  &&  ((**p).mother() == hepParticle) ) 
	fromPrimaryProcess_ = false ; 
  }
  

  
  // is it from gluon splitting?
  // also check if mother has status == 3
  //CW if ( hasMother && motherParticleInfo.isGluon() && motherStatusPythia_==3 )  fromGluonSplitting = true ; 

  //CW new
  // is a quark, has status==2, mother has status==3 and different flavour
  if ( statusPythia_==2 &&
       flavourAbs_>=0 && flavourAbs_<=5 &&
       motherStatusPythia_==3 &&
       flavour_ != motherLundCode_ ) fromGluonSplitting_ = true ;      
  
  
  // is it a 'final' parton ?
  // no daughters -> loop over partons and check if mother
  // it has to be status==2 !!
  isFinalParton_ = true ;
  if ( statusPythia_ != 2 ) isFinalParton_ = false ;
  for (p = hepEvent->particles_begin(); (*p) != lastParton_; ++p) {
    if ( (**p).mother() == hepParticle ) isFinalParton_ = false ; 
  }

  
  // is it a quark that radiates off a gluon?
  // (not yet there, needed for anything?)

  
  // is it a gluon or light quark that splits? (either direct or subsequent splitting)
  // a slitting gluon needs to have a quark and antiquark of same flavour amongst its daughters;
  // again, we have to go via all partons and check if they have the current parton as mother
  if ( particleInfo().isGluon() ||  particleInfo().isD() || particleInfo().isU() || particleInfo().isS() ) {
    int nD (0) , nDBar (0) ;
    int nU (0) , nUBar (0) ;
    int nS (0) , nSBar (0) ;
    int nC (0) , nCBar (0) ;
    int nB (0) , nBBar (0) ;
    for (p = hepEvent->particles_begin(); (*p) != lastParton_; ++p) {
      if ( (**p).mother() == hepParticle ) {
        int pid = (**p).pdg_id();
	if ( pid == 1 ) nD++ ;
	if ( pid == 2 ) nU++ ;
	if ( pid == 3 ) nS++ ;
	if ( pid == 4 ) nC++ ;
	if ( pid == 5 ) nB++ ;
	// bar
	if ( pid == -1 ) nDBar++ ;
	if ( pid == -2 ) nUBar++ ;
	if ( pid == -3 ) nSBar++ ;
	if ( pid == -4 ) nCBar++ ;
	if ( pid == -5 ) nBBar++ ;
      }
    }
    if ( nD>0 && nDBar>0 ) splitsToD_ = true ; 
    if ( nU>0 && nUBar>0 ) splitsToU_ = true ; 
    if ( nS>0 && nSBar>0 ) splitsToS_ = true ; 
    if ( nC>0 && nCBar>0 ) splitsToC_ = true ; 
    if ( nB>0 && nBBar>0 ) splitsToB_ = true ; 
  }
//  print();   
}



void MCParton::print() const {
  // print all Info for heavy hadron
  cout << "--> MCParton:" << endl;
  cout << "--> LundCode, flavour, statusPythia: ";
  cout.width(8); cout << particleInfo().lundCode();
  cout.width(8); cout << flavour_;
  cout.width(8); cout << statusPythia_        << endl;

  cout << "--> Mother Lund Code, status :       ";
  cout.width(8); cout << motherLundCode_;
  cout.width(16); cout << motherStatusPythia_ << endl;

  if ( fromPrimaryProcess_) cout << "--> fromPrimaryProcess ";
  if ( fromRadiation_	) cout << "--> fromRadiation	  ";
  if ( fromGluonSplitting_) cout << "--> fromGluonSplitting ";
  if ( isFinalParton_	) cout << "--> isFinalParton	  ";
  if ( isInitialParton_	) cout << "--> isInitialParton    ";
  if ( fromPrimaryProcess_ || fromRadiation_ || fromGluonSplitting_ || isFinalParton_ ||
	isInitialParton_ ) cout << endl;
    //  cout << "--> radiates gluon         :" << radiatesGluon       << endl;

  if (splitsToD_ || splitsToU_ || splitsToS_ || splitsToC_ || splitsToB_)
	cout <<"--> splits to : ";
  if (splitsToD_) cout << " D";
  if (splitsToU_) cout << " U";
  if (splitsToS_) cout << " S";
  if (splitsToC_) cout << " C";
  if (splitsToB_) cout << " B";
  if (splitsToD_ || splitsToU_ || splitsToS_ || splitsToC_ || splitsToB_)
    cout << endl;

//   cout << "--> splitsToGluon          :" << splitsToGluon       << endl;
//   cout << "--> mass                   :" << mass                << endl;
//   cout << "--> eta                    :" << eta                 << endl;
//   cout << "--> phi                    :" << phi                 << endl;
//   cout << "--> pabs                   :" << pabs                << endl;
//   cout << "--> e                      :" << e                   << endl;
  cout << "--> Momentum, eta, phi        :" << fourVector().px()<< " , "  
    << fourVector().py() << " , " << fourVector().pz() << " , "  
    << fourVector().eta()<< " , " << fourVector().phi() <<endl;
  cout << "--> Summed daugther momentum  :" << summedDaughters_.px()<< " , "
    << summedDaughters_.py() << " , " << summedDaughters_.pz() << " , "
    << summedDaughters_.eta()<< " , " << summedDaughters_.phi() <<endl;
  cout << endl;

}
  
