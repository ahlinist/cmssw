//
// 
//

#include "TauAnalysis/RecoTools/plugins/PATLeptonIpSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

template <class T>
PATLeptonIpSelector<T>::PATLeptonIpSelector(const edm::ParameterSet & iConfig) 
{
 
  leptonSrc_ = iConfig.getParameter<edm::InputTag>("leptonSource");
  leptonType_ = iConfig.getParameter<std::string>("leptonType");
  vertexSrc_ = iConfig.getParameter<edm::InputTag>("vertexSource");
  ipCut_ = iConfig.getParameter<double>("IpCut");
}

template <class T>
PATLeptonIpSelector<T>::~PATLeptonIpSelector() 
{
//--- nothing to be done yet...
}

template <class T>
void PATLeptonIpSelector<T>::select(const edm::Handle<collection>& patCollection,
				    edm::Event & iEvent, const edm::EventSetup & iSetup) 
{
  selected_.clear();
  
  typedef std::vector<T> patLeptonCollection;
  
  typedef edm::Handle<patLeptonCollection> patLeptonCollectionHandle;
  std::vector<patLeptonCollectionHandle> patSelLeptonCollections;
  
  patLeptonCollectionHandle patLeptonInputCollection;
  iEvent.getByLabel(leptonSrc_, patLeptonInputCollection);
  
  edm::Handle<reco::VertexCollection> thePVs;
  iEvent.getByLabel(vertexSrc_, thePVs);
  const reco::VertexCollection vertCollection=*(thePVs.product());
  reco::Vertex thePV;
  thePV=*(vertCollection.begin());

  patLeptonCollection collection=*patCollection;
  for ( typename patLeptonCollection::const_iterator patLeptonInput = patLeptonInputCollection->begin(); 
	patLeptonInput != patLeptonInputCollection->end(); ++patLeptonInput ) {

    //--- copy pat::Lepton object
    T patLeptonOutput(*patLeptonInput);
    double ip = -100000.;
    if(leptonType_=="electron")
      ip=patLeptonOutput.gsfTrack()->dxy(thePV.position());
    else if(leptonType_=="muon")
      ip=patLeptonOutput.track()->dxy(thePV.position());
    else
      std::cout<<"==== USE electron or muon TYPE ===="<<std::endl;
    
    if(fabs(ip)<ipCut_)
      selected_.push_back(&patLeptonOutput);
  }
  
}



typedef ObjectSelector<PATLeptonIpSelector<pat::Electron> > PATElectronIpSelector;
typedef ObjectSelector<PATLeptonIpSelector<pat::Muon> > PATMuonIpSelector;



#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_ANOTHER_FWK_MODULE(PATElectronIpSelector);
DEFINE_ANOTHER_FWK_MODULE(PATMuonIpSelector);
                          
