/** \class HadSUSYdiElecSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel muons 30 GeV, Pt(2muons)>150
 * Z-Candle, dimuon/Z trigger path
 *
 * $Date: 2007/07/12 09:17:22 $
 * $Revision: 1.1 $
 *
 * \author Michael Tytgat, Maria Spiropulu - CERN
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

#include "SUSYBSMAnalysis/CSA07Skims/interface/HadSUSYdiElecSkim.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

using namespace edm;
using namespace std;
using namespace reco;

class PtSorter {
public:
  template <class T> bool operator() ( const T& a, const T& b ) {
    return ( a.pt() > b.pt() );
  }
};

HadSUSYdiElecSkim::HadSUSYdiElecSkim( const edm::ParameterSet& iConfig ) :
  nEvents_(0), nAccepted_(0)
{
  Elecsrc_ = iConfig.getParameter<InputTag>( "Elecsrc" );
  NminElec_ = iConfig.getUntrackedParameter<int>( "NminElec", 2 );
  ElecPtmin_ = iConfig.getUntrackedParameter<double>( "ElecPtmin", 30. );
  PtmindiElec_ = iConfig.getUntrackedParameter<double>( "PtmindiElec", 150. );
}

/*------------------------------------------------------------------------*/

HadSUSYdiElecSkim::~HadSUSYdiElecSkim() 
{}

/*------------------------------------------------------------------------*/

bool HadSUSYdiElecSkim::filter( edm::Event& iEvent, 
				const edm::EventSetup& iSetup )
{
  nEvents_++;

  Handle<PixelMatchGsfElectronCollection> ElecHandle;
//  try {
    iEvent.getByLabel( Elecsrc_, ElecHandle );
//  }
//  catch ( cms::Exception& ex ) {
//    edm::LogError( "HadSUSYdiElecSkim" ) 
//      << "Unable to get Elec collection "
//      << Elecsrc_.label();
//    return false;
//  }
  if ( ElecHandle->empty() ) return false;
  PixelMatchGsfElectronCollection TheElecs = *ElecHandle;
  std::stable_sort( TheElecs.begin(), TheElecs.end(), PtSorter() );
  
  int nElec = 0;
  double Pxdielec = 0., Pydielec = 0.;
  for ( PixelMatchGsfElectronCollection::const_iterator it = TheElecs.begin();
	it != TheElecs.end(); it++ ) {
    if ( (it->pt() > ElecPtmin_) 
	 && (fabs(it->eta()) < 3.0) ) {
      if ( nElec < 2 ) {
	Pxdielec += it->p()*sin(it->theta())*cos(it->phi());
	Pydielec += it->p()*sin(it->theta())*sin(it->phi());
      }
      nElec++;
    }
  }
  
  if ( nElec < NminElec_ ) return false;

  double PtdiElec = sqrt( Pxdielec*Pxdielec + Pydielec*Pydielec );
  if ( PtdiElec < PtmindiElec_ ) return false;
    
  nAccepted_++;

  return true;
}

/*------------------------------------------------------------------------*/

void HadSUSYdiElecSkim::endJob()
{
  edm::LogVerbatim( "HadSUSYdiElecSkim" ) 
    << "Events read " << nEvents_
    << " Events accepted " << nAccepted_
    << "\nEfficiency " << (double)(nAccepted_)/(double)(nEvents_) 
    << endl;
}
