//
// 
//

#ifndef TauAnalysis_RecoTools_PATLeptonIpSelector_h
#define TauAnalysis_RecoTools_PATLeptonIpSelector_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <vector>
#include <string>

template <class T>
class PATLeptonIpSelector
{

  public:
    
    typedef std::vector<T> collection;
  
    explicit PATLeptonIpSelector(const edm::ParameterSet & iConfig);
    ~PATLeptonIpSelector();

    typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
    typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }


    void select(const edm::Handle<collection>&,edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    size_t size() const { return selected_.size(); }

  private:
    std::vector<const T*> selected_;
    edm::InputTag leptonSrc_;
    std::string leptonType_;
    edm::InputTag vertexSrc_;
    double ipCut_;

};

#endif
