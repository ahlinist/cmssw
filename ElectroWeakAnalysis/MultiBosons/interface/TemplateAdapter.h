#ifndef ElectroWeakAnalysis_MultiBosons_TemplateAdapter_h
#define ElectroWeakAnalysis_MultiBosons_TemplateAdapter_h
/** \class TemplateAdapter TemplateAdapter.h ElectroWeakAnalysis/MultiBosons/plugins/TemplateAdapter.h

  \author Jan Veverka, Caltech
  \author Lindsey Gray

 Description: Given EWK VBTF Zmumu dimuons produces a collection of references to the (muon) daughters

 Implementation:
  Use a buffer (std::vector) with std::find and std::sort to prepare the
  output and then copy them int the output product (edm::RefVector)
*/
//
// Original Author:  Jan Veverka,32 3-A13,+41227677936,
//         Created:  Sat Apr 24 10:51:03 CEST 2010
// $Id: TemplateAdapter.h,v 1.2 2010/04/25 09:58:07 lgray Exp $
//
//


// system include files
#include <memory>
#include <algorithm>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/EDMException.h"


namespace vgamma {
  template<typename T>
  struct GreaterByPtRef {
    bool operator()( const T & t1, const T & t2 ) const {
      return t1->pt() > t2->pt();
    }
  };
}

//
// class declaration
//
template<typename InputCollection,typename OutputCollection>
class TemplateAdapter : public edm::EDProducer {
private:
  typedef typename OutputCollection::value_type TRef;
  typedef typename TRef::value_type ProductType;
  typedef std::vector<TRef> TRefCollection;
  
public:
  explicit TemplateAdapter(const edm::ParameterSet&);
  ~TemplateAdapter(){}

private:
  virtual void beginJob(){}
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob(){}

  // ----------member data ---------------------------
  edm::InputTag src_;
};

// Constructor
template<typename InputCollection, typename OutputCollection>
  TemplateAdapter<InputCollection,OutputCollection>::TemplateAdapter(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>("src") )
{
  //register product
  produces<OutputCollection>();
}


template<typename InputCollection, typename OutputCollection>
  void
  TemplateAdapter<InputCollection,OutputCollection>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // retreive the source collection
  edm::Handle<InputCollection> src;
  iEvent.getByLabel(src_, src);

  // create the buffer collection
  std::auto_ptr<TRefCollection> buffer(new TRefCollection);
  buffer->reserve( 2 * src->size() );

  // loop over the source collection
  for (typename InputCollection::const_iterator mother = src->begin();
       mother != src->end(); ++mother)
    {
      // loop over the daughters
      for (reco::Candidate::const_iterator daughter = mother->begin();
	   daughter != mother->end(); ++daughter)
	{      
	  if (!daughter->hasMasterClone() && !daughter->hasMasterClonePtr())
	    edm::Exception::throwThis(edm::errors::InvalidReference,"Daughter does not have Master Clone!");
	  
	  try {
	    
	    TRef tRef;
	    std::string TRefType(typeid(TRef).name()); 
	    
	    // some code in case we want to use PtrVector and original persistent references are edm::Refs & not edm::Ptrs
	    if(TRefType.find("edm3Ptr") != std::string::npos) { // this is a really really bad type checking hack
	      try{
		edm::Ref<std::vector<ProductType> >  tempRef = 
		  daughter->masterClone().castTo<edm::Ref<std::vector<ProductType> > >(); // try cast to Ref
		tRef = edm::refToPtr<std::vector<ProductType> >(tempRef); // turn ref into Ptr
	      } catch( edm::Exception& e) {
		tRef = daughter->masterClone().castTo<TRef>(); // if that throws, try cast to Ptr
	      }
	    } else {
	      tRef = daughter->masterClone().castTo<TRef>(); /// just cast to a Ref
	    } // end bad typecheck hack
	    
	    if (std::find(buffer->begin(), buffer->end(), tRef) == buffer->end() )
	      {
		// This tRef is not in the buffer yet, fill it.
		buffer->push_back(tRef);
	      } // Check that tRef is not in buffer.
	  } catch ( edm::Exception& e) {} // If this exception happens for all daughters of a candidate, an exception is thrown later
	} // loop over daughters
    } // loop over source collection

  if(!buffer->size() && src->size()) // Throw exception if no daughters found and there are input candidates
	edm::Exception::throwThis(edm::errors::ProductNotFound,
				  "No daughters of specified type found. Please provide proper input!");

  // Sort buffer by pt.
  vgamma::GreaterByPtRef<TRef> ptComparator;
  std::sort(buffer->begin(), buffer->end(), ptComparator);

  // create the output product
  std::auto_ptr<OutputCollection> output(new OutputCollection);
  
  // copy the buffer in the output
  for (typename TRefCollection::const_iterator item = buffer->begin();
       item != buffer->end(); ++item)
  {
    output->push_back(*item);
  }
  
  // save the output
  iEvent.put(output);
}

#endif
