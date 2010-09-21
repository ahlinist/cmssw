#ifndef _ElectroWeakAnalysis_MultiBosons_AsscToValueToValueMapConverter_h_
#define _ElectroWeakAnalysis_MultiBosons_AsscToValueToValueMapConverter_h_

// -*- C++ -*-
//
// Package:    MultiBosons
// Class:      AsscToValueToValueMapConverter
//
/**\class  AsscToValueToValueMapConverter AsscToValueToValueMapConverter.h ElectroWeakAnalysis/MultiBosons/interface/AsscToValueToValueMapConverter.h

Description: Converts edm::AssociationMap<edm::OneToValue<T,U> > to edm::ValueMap<U>

\author Lindsey Gray (U.W. Madison)

Implementation:

*/

// system include files
#include <memory>
#include <string>
#include <vector>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/Common/interface/AssociationMap.h"

namespace vgamma {
  //
  // class decleration
  //
  // CollType is some kind of edm data collection that has a ::value_type
  template <typename CollType,typename ValueType>
  class AsscToValueToValueMapConverter : public edm::EDProducer {
    typedef edm::AssociationMap<edm::OneToValue<CollType,ValueType> > AsscType;
  public:
    explicit AsscToValueToValueMapConverter(const edm::ParameterSet&);
    ~AsscToValueToValueMapConverter() {}
  private:
    virtual void produce(edm::Event&, const edm::EventSetup&);

    edm::InputTag valueTypeSrc;
    edm::InputTag asscMapSrc;
    std::string prodInstName;
    bool complain;
  }; // AsscToValueToValueMapConverter<T,U>

  template <typename CollType,typename ValueType>
  AsscToValueToValueMapConverter<CollType,ValueType>::AsscToValueToValueMapConverter
    (const edm::ParameterSet& iConfig): valueTypeSrc(iConfig.getParameter<edm::InputTag>("collectionSrc")),
      asscMapSrc(iConfig.getParameter<edm::InputTag>("associationSrc"))
  {
    if(iConfig.existsAs<std::string>("label"))
      prodInstName = iConfig.getParameter<std::string>("label");
    else
      prodInstName = asscMapSrc.label()+std::string(":")+asscMapSrc.instance();

    //complain instead of throwing exception
    if(iConfig.existsAs<bool>("complain"))
      complain = iConfig.getParameter<bool>("complain");
    else
      complain = false;	

    produces<edm::ValueMap<ValueType> >(prodInstName);
  }

  template <typename CollType,typename ValueType>
  void AsscToValueToValueMapConverter<CollType,ValueType>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {    
    // the association map to be filled
    edm::Handle<AsscType> theMap;
    // the ValueMap product and its filler
    std::auto_ptr<edm::ValueMap<ValueType> > prod( new edm::ValueMap<ValueType>() );
    typename edm::ValueMap<ValueType>::Filler theFiller(*prod);
    std::vector<ValueType> values;
    // the data collection serving as the keys for the association map
    edm::Handle<CollType> theCollection;
    std::string pin;

    //get collections
    iEvent.getByLabel(valueTypeSrc,theCollection);
    iEvent.getByLabel(asscMapSrc,theMap);
    values.reserve(theCollection->size());

    for(typename CollType::const_iterator i = theCollection->begin(); 
	i != theCollection->end();
	++i) {
      typename AsscType::const_iterator itr = 
	theMap->find(edm::Ref<CollType>(theCollection,i - theCollection->begin()));
      if(itr != theMap->end())
	values.push_back(itr->val);
      else {
	std::stringstream theerror;

	theerror << "Isomorphic AssociationMap is not isomorphic." << std::endl
		 << "Photon: " << i - theCollection->begin() << " does not have a mapping!" << std::endl;
	
	if(complain)
	  std::cout << theerror.str();
	else
	  throw edm::Exception( edm::errors::ProductNotFound, 
				theerror.str());
	
      }
    }
    theFiller.insert(theCollection, values.begin(), values.end());
    theFiller.fill();
    iEvent.put(prod,prodInstName);
  }

} // namespace vgamma

#endif
