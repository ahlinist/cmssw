/** \class TeVdiMuonSkim
 *
 * a skim for TeV dimuon analyses
 * requires 2 muon with a selectable Pt cut
 *
 * $Date: 2007/08/08 13:18:38 $
 * $Revision: 1.2 $
 *
 * \author Piotr Traczyk - SINS Warsaw
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

#include "SUSYBSMAnalysis/CSA07Skims/interface/TeVdiMuonSkim.h"
#include "DataFormats/MuonReco/interface/Muon.h"

using namespace edm;
using namespace std;
using namespace reco;

TeVdiMuonSkim::TeVdiMuonSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  Muonsrc_ = iConfig.getParameter<InputTag>( "Muonsrc" );
  MuonPtmin_ = iConfig.getParameter<double>( "MuonPtmin");
}

/*------------------------------------------------------------------------*/

TeVdiMuonSkim::~TeVdiMuonSkim() 
{}

/*------------------------------------------------------------------------*/

bool TeVdiMuonSkim::filter( edm::Event& iEvent, 
				const edm::EventSetup& iSetup )
{
  nEvents_++;

  Handle<MuonCollection> MuonHandle;
  iEvent.getByLabel(Muonsrc_, MuonHandle);

  if (MuonHandle->empty()) return false;
  
  MuonCollection theMuonC = *MuonHandle;
  
  int nMuons=0;
  for ( MuonCollection::const_iterator it=theMuonC.begin(); it!=theMuonC.end(); it++ )
    if (it->pt()>MuonPtmin_) nMuons++;
  
  if (nMuons<2) return false;

  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void TeVdiMuonSkim::endJob()
{
  edm::LogVerbatim("TeVdiMuonSkim") 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
