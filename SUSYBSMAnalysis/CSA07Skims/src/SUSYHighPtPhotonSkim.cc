/* \class SUSYHighPtPhotonSkim
 *
 * High Energy Photon SUSY Skim
 * one(two) photon(s) > xx GeV in barrel + isolation 
 *
 * $Date: 2007/07/30 16:33:13 $
 * $Revision: 1.1 $
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
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/AssociationVector.h"
// #include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
// #include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"

#include "SUSYBSMAnalysis/CSA07Skims/interface/SUSYHighPtPhotonSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

SUSYHighPtPhotonSkim::SUSYHighPtPhotonSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  Photonsrc_ = iConfig.getParameter<InputTag>( "Photonsrc" );
  Photon1Ptmin_ = 
    iConfig.getUntrackedParameter<double>( "Photon1Ptmin", 80. );
  Photon2Ptmin_ = 
    iConfig.getUntrackedParameter<double>( "Photon2Ptmin", 20. );
  IsIsolated_ = iConfig.getUntrackedParameter<bool>( "IsIsolated", 0);
  IsolationCut_ = iConfig.getUntrackedParameter<double>( "IsolationCut", 9. );
}

/*------------------------------------------------------------------------*/

SUSYHighPtPhotonSkim::~SUSYHighPtPhotonSkim() 
{}

/*------------------------------------------------------------------------*/

bool SUSYHighPtPhotonSkim::filter( edm::Event& iEvent, 
				       const edm::EventSetup& iSetup )
{
  nEvents_++;

  typedef AssociationVector<RefProd<CandidateCollection>, vector<double> > PhotonMapCollection;
  Handle<PhotonMapCollection>  PhotonHandle;

  iEvent.getByLabel( Photonsrc_, PhotonHandle );

  if ( PhotonHandle->empty() ) return false;

  int nPhoton1 = 0;
  int nPhoton2 = 0;
  //  int counter = 0;

  for ( PhotonMapCollection::const_iterator it = PhotonHandle->begin(); 
	it != PhotonHandle->end(); it++ ) {

    bool iso = it->second < IsolationCut_;
    if(!IsIsolated_) iso = 1; 

    //    counter++;
    //    cout << counter << "  " << it->first->pt() << "  " << it->second << endl;
    
    if (iso && fabs(it->first->eta()) < 1.479 && it->first->pt() > Photon2Ptmin_) {
       
      if (fabs(it->first->eta()) < 1.479){
	if (it->first->pt() > Photon1Ptmin_)  nPhoton1++;
	if (it->first->pt() > Photon2Ptmin_)  nPhoton2++;
      }
    }
  }

  if ( !nPhoton1 ) return false;
  if ( nPhoton2<2 ) return false;
  
  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void SUSYHighPtPhotonSkim::endJob()
{
//   edm::LogVerbatim( "SUSYHighPtPhotonSkim" ) 
  cout
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
