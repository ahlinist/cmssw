////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"

#include <memory>
#include <vector>
#include <sstream>

//#include "Validation/RecoTau/interface/prettyPrint.h" debugging putpose

////////////////////////////////////////////////////////////////////////////////
// class definition
////////////////////////////////////////////////////////////////////////////////
class CollectionFromZLegProducer : public edm::EDProducer
{
public:
  // construction/destruction
  CollectionFromZLegProducer(const edm::ParameterSet& iConfig);
  virtual ~CollectionFromZLegProducer();
  
  void produce(edm::Event& iEvent,const edm::EventSetup& iSetup);
  void endJob();

private:  
  // member data
  edm::InputTag      		  srcTheZ_	                      ;
  std::string        		  OutputCollection_, moduleLabel_ ;
  
};



////////////////////////////////////////////////////////////////////////////////
// construction/destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
CollectionFromZLegProducer::CollectionFromZLegProducer(const edm::ParameterSet& iConfig)
  : srcTheZ_     (iConfig.getParameter<edm::InputTag>("ZCandidateCollection"))
  , moduleLabel_ ( iConfig.getParameter<std::string> ("@module_label")       )
{
  produces< std::vector<reco::CompositeCandidate> >("theTagLeg"  );
  produces< std::vector<reco::CompositeCandidate> >("theProbeLeg");
  produces< std::vector<double>                   >("mass"       );
}


//______________________________________________________________________________
CollectionFromZLegProducer::~CollectionFromZLegProducer()
{
}

////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void CollectionFromZLegProducer::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{  
  // ValueMap tutorial from https://twiki.cern.ch/twiki/bin/view/CMS/NewValueMap

  std::auto_ptr<std::vector<reco::CompositeCandidate> > theTagLeg  (new std::vector<reco::CompositeCandidate>) ;	     
  std::auto_ptr<std::vector<reco::CompositeCandidate> > theProbeLeg(new std::vector<reco::CompositeCandidate>) ;
  std::auto_ptr<std::vector<double> >                   masses     (new std::vector<double>)                 ; // create map passing the handle to the matched collection
  
  edm::Handle< std::vector<reco::CompositeCandidate> > theZHandle;
  iEvent.getByLabel(srcTheZ_,theZHandle);
   
  //std::vector<double> masses ;
  masses->clear()             ;

  // this is specific for our 'tag and probe'
  for (std::vector<reco::CompositeCandidate>::const_iterator Zit  = theZHandle->begin() ; 
                                                             Zit != theZHandle->end()   ; 
                                                             ++Zit                      )
  {
    //std::cout << __LINE__ << " : sono qui" << std::endl ;
	int c = 0;
	//masses.push_back(Zit->mass());
	masses->push_back(Zit->mass());

	for(reco::CompositeCandidate::const_iterator Daug =(*Zit).begin(); 
                                                 Daug!=(*Zit).end()  ; 
                                                 ++Daug              )
	{
	  if (c == 0){
	    reco::CompositeCandidate candT(*Daug) ;
	    theTagLeg->push_back(candT) ;
	  }
	  if (c == 1){
	    reco::CompositeCandidate candP(*Daug) ;
	    theProbeLeg->push_back(candP) ;
	  }
	  c++ ;
	}
  }
   
  iEvent.put(theTagLeg  , "theTagLeg"   ) ;
  iEvent.put(theProbeLeg, "theProbeLeg" ) ;
  iEvent.put(masses     , "mass"        ) ;
}

void CollectionFromZLegProducer::endJob()
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CollectionFromZLegProducer);
