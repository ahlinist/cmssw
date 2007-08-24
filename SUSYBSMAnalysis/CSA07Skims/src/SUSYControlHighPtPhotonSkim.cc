/* \class SUSYControlHighPtPhotonSkim
 *
 * High Energy Photon SUSY Skim (control sample)
 * one photon and one electron > xx GeV in barrel + isolation 
 *
 * $Date: 2007/08/24 12:29:28 $
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
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/AssociationVector.h"

#include "SUSYBSMAnalysis/CSA07Skims/interface/SUSYControlHighPtPhotonSkim.h"

using namespace edm;
using namespace std;
using namespace reco;

SUSYControlHighPtPhotonSkim::SUSYControlHighPtPhotonSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  Photonsrc_ = iConfig.getParameter<InputTag>( "Photonsrc" );
  Electronsrc_ = iConfig.getParameter<InputTag>( "Electronsrc" );
  PhotonPtmin_ = 
    iConfig.getUntrackedParameter<double>( "PhotonPtmin", 20. );
  ElectronPtmin_ = 
    iConfig.getUntrackedParameter<double>( "ElectronPtmin", 80. );
  IsIsolated_ = iConfig.getUntrackedParameter<bool>( "IsIsolated", 0);
  IsolationCut_ = iConfig.getUntrackedParameter<double>( "IsolationCut", 9. );
}

/*------------------------------------------------------------------------*/

SUSYControlHighPtPhotonSkim::~SUSYControlHighPtPhotonSkim() 
{}

/*------------------------------------------------------------------------*/

bool SUSYControlHighPtPhotonSkim::filter( edm::Event& iEvent, 
				       const edm::EventSetup& iSetup )
{
  nEvents_++;

  typedef AssociationVector<RefProd<CandidateCollection>, vector<double> > PhotonMapCollection;
  Handle<PhotonMapCollection>  PhotonHandle;

  iEvent.getByLabel( Photonsrc_, PhotonHandle );

  if ( PhotonHandle->empty() ) return false;

  Handle<PixelMatchGsfElectronCollection>  ElectronHandle;

  iEvent.getByLabel( Electronsrc_, ElectronHandle );

  if ( ElectronHandle->empty() ) return false;

  int nPhoton = 0;
  int nElectron = 0;

  for ( PhotonMapCollection::const_iterator it = PhotonHandle->begin(); 
	it != PhotonHandle->end(); it++ ) {

    bool iso = it->second < IsolationCut_;
    if(!IsIsolated_) iso = 1; 

    if (iso && fabs(it->first->eta()) < 1.479 && it->first->pt() > PhotonPtmin_) { 

      int overlap(0);
      for ( PixelMatchGsfElectronCollection::const_iterator itel = ElectronHandle->begin(); 
	    itel != ElectronHandle->end(); itel++ ) {
	
	if( itel->pt() > ElectronPtmin_ ){
	  double dr = sqrt( pow(it->first->eta() - itel->eta(),2) +
			    pow(it->first->phi() - itel->phi(),2) );
	  if ( dr<0.1 ) overlap = 1;
	}
      }
      if (!overlap) nPhoton++;

    }

  }

  for ( PixelMatchGsfElectronCollection::const_iterator itel = ElectronHandle->begin(); 
	itel != ElectronHandle->end(); itel++ ) {

    if (fabs(itel->eta()) < 1.479 && itel->pt() > ElectronPtmin_) 
      nElectron++;
 }

  if (!nPhoton) return false;
  if (!nElectron) return false;

  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void SUSYControlHighPtPhotonSkim::endJob()
{
  edm::LogVerbatim( "SUSYControlHighPtPhotonSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
