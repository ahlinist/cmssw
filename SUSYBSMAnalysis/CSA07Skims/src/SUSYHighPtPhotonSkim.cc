/* \class SUSYHighPtPhotonSkim
 *
 * High Energy Photon SUSY Skim
 * one(two) photon(s) > xx GeV in barrel + isolation 
 *
 * $Date: 2007/07/12 09:24:45 $
 * $Revision: 1.2 $
 *
 * \author Daniele del Re - Univ. La Sapienza & INFN
 *
 */

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <cstdio>
#include <vector>
#include <memory>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"    

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"

#include "SUSYBSMAnalysis/CSA07Skims/interface/SUSYHighPtPhotonSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

SUSYHighPtPhotonSkim::SUSYHighPtPhotonSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  Photonsrc_ = iConfig.getParameter<InputTag>( "Photonsrc" );
  Tracksrc_ = iConfig.getParameter<InputTag>( "Tracksrc" );
  Photon1Ptmin_ = 
    iConfig.getUntrackedParameter<double>( "Photon1Ptmin", 80. );
  Photon2Ptmin_ = 
    iConfig.getUntrackedParameter<double>( "Photon2Ptmin", 20. );
  IsIsolated_ = iConfig.getUntrackedParameter<bool>( "IsIsolated", 0);
}

/*------------------------------------------------------------------------*/

SUSYHighPtPhotonSkim::~SUSYHighPtPhotonSkim() 
{}

/*------------------------------------------------------------------------*/

bool SUSYHighPtPhotonSkim::filter( edm::Event& iEvent, 
				       const edm::EventSetup& iSetup )
{
  nEvents_++;

  Handle<PhotonCollection> PhotonHandle; 
  Handle<TrackCollection> TrackHandle;

  try {
    iEvent.getByLabel( Photonsrc_, PhotonHandle );
  } 
  catch ( cms::Exception& ex ) {
    edm::LogError( "SUSYHighPtPhotonSkim" ) 
      << "Unable to get Photon collection "
      << Photonsrc_.label();
    return false;
  }

  try {
    iEvent.getByLabel( Tracksrc_, TrackHandle );
  } 
  catch ( cms::Exception& ex ) {
    edm::LogError( "SUSYHighPtPhotonSkim" ) 
      << "Unable to get Track collection "
      << Tracksrc_.label();
    return false;
  }

  if ( PhotonHandle->empty() ) return false;
  if ( TrackHandle->empty() ) return false;

  TrackDetectorAssociator trackAssociator_;
  TrackDetectorAssociator::AssociatorParameters parameters;
  parameters.useEcal = false ;
  parameters.useCalo = false ;
  parameters.useHcal = false ;
  parameters.useHO = false ;
  parameters.useMuon = false ;
  trackAssociator_.useDefaultPropagator();

  int nPhoton1 = 0;
  int nPhoton2 = 0;
  
  for ( PhotonCollection::const_iterator it = PhotonHandle->begin(); 
	it != PhotonHandle->end(); it++ ) {
        
    double dummyPt = 0;    
    int conetracks = 0;

    if (IsIsolated_ && fabs(it->eta()) < 1.479 && it->pt() > Photon2Ptmin_) {
 
      for(TrackCollection::const_iterator it_track = TrackHandle->begin(); it_track != TrackHandle->end(); it_track++ ){ 
	
	const FreeTrajectoryState fts = trackAssociator_.getFreeTrajectoryState( iSetup , *it_track );
	const TrackDetectorAssociator::AssociatorParameters myparameters = parameters;   
	
	TrackDetMatchInfo info = trackAssociator_.associate(iEvent, iSetup,fts,myparameters);      
	
	double deta = info.trkGlobPosAtEcal.eta() - it->eta();
	double dphi = fabs( info.trkGlobPosAtEcal.phi() - it->phi());
	double dr = sqrt(dphi*dphi+deta*deta);
	
	if ( fabs(dr) < 0.3 ){ 
	  conetracks++; dummyPt += it_track->pt(); 
	  //	  cout << dummyPt << "   "  << conetracks << "   " <<  it->pt() << endl;
	} 
	
      } //end loop over tracks                     

    }

    if (fabs(it->eta()) < 1.479 && dummyPt<9.){
      if (it->pt() > Photon1Ptmin_)  nPhoton1++;
      if (it->pt() > Photon2Ptmin_)  nPhoton2++;
    }
  }
  
  if ( !nPhoton1 ) return false;
  if ( nPhoton2<2 ) return false;
  
  cout << nAccepted_ << "    "  <<  nPhoton1 << "    "  <<  nPhoton2 <<  endl;
  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void SUSYHighPtPhotonSkim::endJob()
{
  edm::LogVerbatim( "SUSYHighPtPhotonSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
