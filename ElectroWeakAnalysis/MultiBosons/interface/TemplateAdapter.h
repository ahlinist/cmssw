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
// $Id: TemplateAdapter.h,v 1.1 2010/04/24 16:31:51 veverka Exp $
//
//


// system include files
#include <memory>
#include <algorithm>

// user include files
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

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
template<typename TCollection>
class TemplateAdapter : public edm::EDProducer {
private:
  typedef edm::Ref<TCollection> TRef;
  typedef std::vector<TRef> TRefCollection;
  typedef edm::RefVector<TCollection> TRefVector;

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
template<typename TCollection>
TemplateAdapter<TCollection>::TemplateAdapter(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>("src") )
{
  //register product
  produces<TRefVector>();
}


template<typename TCollection>
void
TemplateAdapter<TCollection>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // retreive the source collection
  edm::Handle<reco::CandidateView> src;
  iEvent.getByLabel(src_, src);

  // create the buffer collection
  std::auto_ptr<TRefCollection> buffer(new TRefCollection);
  buffer->reserve( 2 * src->size() );

  // loop over the source collection
  for (reco::CandidateView::const_iterator mother = src->begin();
      mother != src->end(); ++mother)
  {
    // loop over the daughters
    for (reco::Candidate::const_iterator daughter = mother->begin();
        daughter != mother->end(); ++daughter)
    {
      // if (!daughter->hasMasterClone() ) continue; // better to throw than do something undefined
      TRef tRef = daughter->masterClone().castTo<TRef>();
      // Check that tRef is not in buffer.
      if (std::find(buffer->begin(), buffer->end(), tRef) == buffer->end() )
      {
        // This tRef is not in the buffer yet, fill it.
        buffer->push_back(tRef);
      } // Check that tRef is not in buffer.
    } // loop over daughters
  } // loop over source collection

  // Sort buffer by pt.
  vgamma::GreaterByPtRef<TRef> ptComparator;
  std::sort(buffer->begin(), buffer->end(), ptComparator);

  // create the output product
  std::auto_ptr<TRefVector> output(new TRefVector);
  
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
