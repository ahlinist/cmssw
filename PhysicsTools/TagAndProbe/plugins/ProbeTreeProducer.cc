// -*- C++ -*-
//
// Package:    ProbeTreeProducer
// Class:      ProbeTreeProducer
// 
/**\class ProbeTreeProducer ProbeTreeProducer.cc 

 Description: TTree producer based on input probe parameters

 Implementation:
     <Notes on implementation>
*/

#include <memory>
#include <ctype.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/TagAndProbe/interface/BaseTreeFiller.h"
#include <set>
#include "FWCore/ParameterSet/interface/Registry.h"

class ProbeTreeProducer : public edm::EDFilter {
  public:
    explicit ProbeTreeProducer(const edm::ParameterSet&);
    ~ProbeTreeProducer();

  private:
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    /// InputTag to the collection of all probes
    edm::InputTag probesTag_;

    /// The selector object
    StringCutObjectSelector<reco::Candidate, true> cut_;

    /// Specifies whether this module should filter
    bool filter_;

    /// Name of the reco::Candidate function used for sorting
    std::string sortDescendingBy_;

    /// The StringObjectFunction itself
    StringObjectFunction<reco::Candidate, true> sortFunction_;

    /// The number of first probes used to fill the tree
    int32_t maxProbes_;

    /// The object that actually computes variables and fills the tree for the probe
    std::auto_ptr<tnp::BaseTreeFiller> probeFiller_;
};

ProbeTreeProducer::ProbeTreeProducer(const edm::ParameterSet& iConfig) :
  probesTag_(iConfig.getParameter<edm::InputTag>("src")),
  cut_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : ""),
  filter_(iConfig.existsAs<bool>("filter") ? iConfig.getParameter<bool>("filter") : false),
  sortDescendingBy_(iConfig.existsAs<std::string>("sortDescendingBy") ? iConfig.getParameter<std::string>("sortDescendingBy") : ""),
  sortFunction_(sortDescendingBy_.size()>0 ? sortDescendingBy_ : "pt"), //need to pass a valid default
  maxProbes_(iConfig.existsAs<int32_t>("maxProbes") ? iConfig.getParameter<int32_t>("maxProbes") : -1),
  probeFiller_(new tnp::BaseTreeFiller("probe_tree", iConfig))
{
}

ProbeTreeProducer::~ProbeTreeProducer(){
}

bool ProbeTreeProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
  bool result = !filter_;
  edm::Handle<reco::CandidateView> probes;
  iEvent.getByLabel(probesTag_, probes);
  if(!probes.isValid()) return result;
  probeFiller_->init(iEvent);
  // select probes and calculate the sorting value
  typedef std::pair<reco::CandidateBaseRef, double> Pair;
  std::vector<Pair> selectedProbes;
  for (size_t i = 0; i < probes->size(); ++i){
    const reco::CandidateBaseRef &probe = probes->refAt(i);
    if(cut_(*probe)){
      selectedProbes.push_back(Pair(probe, sortFunction_(*probe)));
    }
  }
  // sort only if a function was provided
  if(sortDescendingBy_.size()>0) sort(selectedProbes.begin(), selectedProbes.end(), boost::bind(&Pair::second, _1) > boost::bind(&Pair::second, _2));
  // fill the first maxProbes_ into the tree
  for (size_t i = 0; i < (maxProbes_<0 ? selectedProbes.size() : maxProbes_); ++i){
    probeFiller_->fill(selectedProbes[i].first);
    result = true;
  }
  return result;
}

void ProbeTreeProducer::endJob(){
    // ask to write the current PSet info into the TTree header
    probeFiller_->writeProvenance(edm::getProcessParameterSet());
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProbeTreeProducer);

