////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

#include <memory>
#include <vector>
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////
class METSelector : public edm::EDFilter
{
public:
  // construction/destruction
  METSelector(const edm::ParameterSet& iConfig);
  virtual ~METSelector();
  
  // member functions
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  //virtual void endJob();

private:  
  // member data
  edm::InputTag     src_   ;
  double            minEt_ ;  
  double            maxEt_ ;  
};

METSelector::METSelector(const edm::ParameterSet& iConfig)
  : src_  (iConfig.getParameter<edm::InputTag>("src")  )
  , minEt_(iConfig.getParameter<double>       ("minEt"))
  , maxEt_(iConfig.getParameter<double>       ("maxEt"))
{
}


METSelector::~METSelector(){}

bool METSelector::filter(edm::Event& iEvent,const edm::EventSetup& iSetup)
{     
  edm::Handle<reco::PFMETCollection> metHandle;
  iEvent.getByLabel(src_,metHandle);

  bool passed = false ;
  
  //std::cout << "MET in the event: " << (metHandle->front()).sumEt() << std::endl;
  
  //double metEt = (metHandle -> front()).sumEt() ;
  double metEt = (metHandle -> front()).pt() ;
  
  if ( metEt > minEt_ && metEt < maxEt_ ){
    passed = true;
    //std::cout << std::endl << "********************** " << "selected MET: " << metEt << " **********************" << std::endl;
  }
  
  //std::cout << std::endl << "********************** " << "passed: " << passed << " **********************" << std::endl;
  
  return passed;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(METSelector);
