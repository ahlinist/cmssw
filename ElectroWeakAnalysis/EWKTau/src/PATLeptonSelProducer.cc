//
// $Id: PATLeptonSelProducer.cc,v 1.18 2008/10/13 13:49:08 cbern Exp $
//

#include "ElectroWeakAnalysis/EWKTau/interface/PATLeptonSelProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

template <class T>
PATLeptonSelProducer<T>::PATLeptonSelProducer(const edm::ParameterSet & iConfig) 
{
  //std::cout << "<PATLeptonSelProducer::PATLeptonSelProducer>:" << std::endl;

  leptonSrc_ = iConfig.getParameter<edm::InputTag>("leptonSource");

  edm::ParameterSet selFlags = iConfig.getParameter<edm::ParameterSet>("selFlags");
  std::vector<std::string> selFlagNames = selFlags.getParameterNamesForType<edm::ParameterSet>();
  for ( std::vector<std::string>::const_iterator it = selFlagNames.begin(); it != selFlagNames.end(); ++it ) {
    edm::ParameterSet selFlag = selFlags.getParameter<edm::ParameterSet>(*it);
    edm::InputTag inputTag = selFlag.getParameter<edm::InputTag>("src");
    std::string selLabel = (*it);

    //std::cout << " inputTag = " << inputTag.label() << ", selLabel = " << selLabel << std::endl;
    selFlags_.push_back(selFlagEntry(inputTag, selLabel));    
  }

//--- produces collection of template type T
  produces<std::vector<T> >();
}

template <class T>
PATLeptonSelProducer<T>::~PATLeptonSelProducer() 
{
//--- nothing to be done yet...
}

template <class T>
void PATLeptonSelProducer<T>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) 
{
  //std::cout << "<PATLeptonSelProducer<T>::produce>:" << std::endl;

  typedef std::vector<T> patLeptonCollection;
  patLeptonCollection* patLeptonOutputCollection = new patLeptonCollection();

  typedef edm::Handle<patLeptonCollection> patLeptonCollectionHandle;
  std::vector<patLeptonCollectionHandle> patSelLeptonCollections;
  for ( std::vector<selFlagEntry>::const_iterator it = selFlags_.begin(); it != selFlags_.end(); ++it ) {
    patLeptonCollectionHandle patSelLeptonCollection;
    iEvent.getByLabel(it->first, patSelLeptonCollection);
    patSelLeptonCollections.push_back(patSelLeptonCollection);
  }

  unsigned numSelFlags = selFlags_.size();

  patLeptonCollectionHandle patLeptonInputCollection;
  iEvent.getByLabel(leptonSrc_, patLeptonInputCollection);
  //std::cout << " patLeptonInputCollection.size = " << patLeptonInputCollection->size() << std::endl;

  for ( typename patLeptonCollection::const_iterator patLeptonInput = patLeptonInputCollection->begin(); patLeptonInput != patLeptonInputCollection->end(); ++patLeptonInput ) {

    T patLeptonOutput(*patLeptonInput);

    for ( unsigned iSelFlag = 0; iSelFlag < numSelFlags; ++iSelFlag ) {
      const std::string& selLabel = selFlags_[iSelFlag].second;
      
      patLeptonCollectionHandle patSelLeptonCollection = patSelLeptonCollections[iSelFlag];

      bool isSelected = false;
      for ( typename patLeptonCollection::const_iterator patSelLepton = patSelLeptonCollection->begin(); patSelLepton != patSelLeptonCollection->end(); ++patSelLepton ) {
	if ( patSelLepton->originalObject() == patLeptonInput->originalObject() ) isSelected = true;
      }

      float userFloat = ( isSelected ) ? 1.0 : 0.0; 
      //std::cout << "userFloat = " << userFloat << std::endl;

      patLeptonOutput.addUserFloat(selLabel, userFloat);
    }
   
    patLeptonOutputCollection->push_back(patLeptonOutput);
  }

  //std::cout << " patLeptonOutputCollection.size = " << patLeptonOutputCollection->size() << std::endl;

  std::auto_ptr<patLeptonCollection> ptr(patLeptonOutputCollection);
  iEvent.put(ptr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronSelProducer);
DEFINE_FWK_MODULE(PATMuonSelProducer);
DEFINE_FWK_MODULE(PATTauSelProducer);
