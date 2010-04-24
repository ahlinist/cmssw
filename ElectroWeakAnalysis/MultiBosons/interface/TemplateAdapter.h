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
// $Id: TemplateAdapter.h,v 1.1 Sat Apr 24 10:51:03 CEST 2010 veverka Exp $
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


#endif
