#ifndef _ElectroWeakAnalysis_MultiBosons_ValueMapTypeConverter_h_
#define _ElectroWeakAnalysis_MultiBosons_ValueMapTypeConverter_h_

// -*- C++ -*-
//
// Package:    MultiBosons
// Class:      ValueMapTypeConverter
//
/**\class  ValueMapTypeConverter ValueMapTypeConverter.h ElectroWeakAnalysis/MultiBosons/interface/ValueMapTypeConverter.h

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
  template <typename CollType,typename InputType,typename OutputType>
  class ValueMapTypeConverter : public edm::EDProducer {
    typedef edm::ValueMap<InputType> InType;
    typedef edm::ValueMap<OutputType> OutType;
  public:
    explicit ValueMapTypeConverter(const edm::ParameterSet&);
    ~ValueMapTypeConverter() {}
  private:
    virtual void produce(edm::Event&, const edm::EventSetup&);

    edm::InputTag valueTypeSrc;
    edm::InputTag valMapSrc;
    std::string prodInstName;    
  }; // ValueMapTypeConverter<T,U>

  template <typename CollType,typename InputType,typename OutputType>
    ValueMapTypeConverter<CollType,InputType,OutputType>::ValueMapTypeConverter
    (const edm::ParameterSet& iConfig): 
      valueTypeSrc(iConfig.getParameter<edm::InputTag>("collectionSrc")),
      valMapSrc(iConfig.getParameter<edm::InputTag>("valMapSrc"))
	{
	  if(valMapSrc.instance() != "")
	    prodInstName = valMapSrc.instance();
	  else     
	    prodInstName = "";
	  
	  produces< OutType >(prodInstName);
	}
    
    template <typename CollType,typename InputType, typename OutputType>
      void ValueMapTypeConverter<CollType,InputType,OutputType>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {    
      // the association map to be filled
      edm::Handle<InType> theMap;
      // the ValueMap product and its filler
      std::auto_ptr<OutType> prod( new OutType() );
      typename OutType::Filler theFiller(*prod);
      std::vector<typename OutType::value_type> values;
      // the data collection serving as the keys for the association map
      edm::Handle<CollType> theCollection;
      std::string pin;
      
      //get collections
      iEvent.getByLabel(valueTypeSrc,theCollection);
      iEvent.getByLabel(valMapSrc,theMap);
      values.reserve(theCollection->size());
      
      for(typename CollType::const_iterator i = theCollection->begin(); 
	  i != theCollection->end();
	  ++i) {
	edm::Ref<CollType> cRef(theCollection,i - theCollection->begin());
	values.push_back((*theMap)[cRef]);      
      }
      theFiller.insert(theCollection, values.begin(), values.end());
      theFiller.fill();
      iEvent.put(prod,prodInstName);
    }
    
} // namespace vgamma

#endif
