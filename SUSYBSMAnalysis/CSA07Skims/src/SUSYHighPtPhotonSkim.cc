/* \class SUSYHighPtPhotonSkim
 *
 * High Energy Photon SUSY Skim
 * one(two) photon(s) > xx GeV in barrel + isolation 
 *
 * $Date: 2007/08/24 12:44:18 $
 * $Revision: 1.4 $
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

#include "SUSYBSMAnalysis/CSA07Skims/interface/SUSYHighPtPhotonSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

SUSYHighPtPhotonSkim::SUSYHighPtPhotonSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  Photonsrc_ = iConfig.getParameter<InputTag>( "Photonsrc" );
  Photon1Ptmin_ = 
    iConfig.getParameter<double>( "Photon1Ptmin");
  Photon2Ptmin_ = 
    iConfig.getParameter<double>( "Photon2Ptmin");
  IsIsolated_ = iConfig.getParameter<bool>( "IsIsolated");
  IsolationCut_ = iConfig.getParameter<double>( "IsolationCut");
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

  for ( PhotonMapCollection::const_iterator it = PhotonHandle->begin(); 
	it != PhotonHandle->end(); it++ ) {

    bool iso = it->second < IsolationCut_;
    if(!IsIsolated_) iso = 1; 
    
    if (iso && fabs(it->first->eta()) < 1.479) {       
      if (it->first->pt() > Photon1Ptmin_)  nPhoton1++;
      if (it->first->pt() > Photon2Ptmin_)  nPhoton2++;
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
  edm::LogVerbatim( "SUSYHighPtPhotonSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
