////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <memory>
#include <vector>
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////
class TauFromPVSelector : public edm::EDProducer
{
public:
  // construction/destruction
  TauFromPVSelector(const edm::ParameterSet& iConfig);
  virtual ~TauFromPVSelector();
  
  // member functions
  void produce(edm::Event& iEvent,const edm::EventSetup& iSetup);
  void endJob();

private:  
  // member data
  edm::InputTag     srcPart_ ;  
  edm::InputTag     srcPV_   ;
  double            max_dxy_ ;
  double            max_dz_  ;
};



////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
TauFromPVSelector::TauFromPVSelector(const edm::ParameterSet& iConfig)
  : srcPart_(iConfig.getParameter<edm::InputTag>("srcTau"))
  , srcPV_  (iConfig.getParameter<edm::InputTag>("srcVertex"))
  , max_dxy_(iConfig.getParameter<double>("max_dxy"))
  , max_dz_ (iConfig.getParameter<double>("max_dz"))
{
  produces<std::vector<reco::PFTau> >();
}


//______________________________________________________________________________
TauFromPVSelector::~TauFromPVSelector(){}

////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void TauFromPVSelector::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{  
  std::auto_ptr<std::vector<reco::PFTau> > goodTaus(new std::vector<reco::PFTau >);
  
  edm::Handle< std::vector<reco::Vertex> > VertexHandle;
  iEvent.getByLabel(srcPV_,VertexHandle);

  edm::Handle< std::vector<reco::PFTau> > TauHandle;
  iEvent.getByLabel(srcPart_,TauHandle);
  
  if( (VertexHandle->size() == 0) || (TauHandle->size() == 0) ) 
  {
    iEvent.put(goodTaus);
    return ;
  }
  
  
  reco::Vertex PV = VertexHandle->front();   
  std::vector<reco::PFTau>::const_iterator TauIt ;

  for (TauIt = TauHandle->begin(); TauIt != TauHandle->end(); ++TauIt) {

    double dZ = TauIt->leadTrack()->dz();
    std::cout << __LINE__ << "]\t" << "d zeta: " << dZ << std::endl;

    if ( fabs(TauIt->leadTrack()->dxy(PV.position())) < max_dxy_ &&
         fabs(TauIt->leadTrack()->dz(PV.position()))  < max_dz_  ){
      goodTaus -> push_back(*TauIt) ;
    }
  }  
  
  iEvent.put(goodTaus);
  
}

void TauFromPVSelector::endJob()
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauFromPVSelector);
