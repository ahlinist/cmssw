#ifndef TauAnalysis_RecoTools_PATElectronIdSelector_h
#define TauAnalysis_RecoTools_PATElectronIdSelector_h

/** \class PATElectronIdSelector
 *
 *  cut on output of electron ID MVA 
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Electron.h"

#include "EGamma/EGammaAnalysisTools/interface/EGammaMvaEleEstimator.h"

#include <vector>

template <typename T>
class PATElectronIdSelectorImp
{
  public:
    
    typedef std::vector<T> collection;
  
    explicit PATElectronIdSelectorImp(const edm::ParameterSet&);
    ~PATElectronIdSelectorImp();

    typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
    typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }

    void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);
    
    size_t size() const { return selected_.size(); }

  protected:
    std::vector<const T*> selected_;

    edm::InputTag srcVertex_;
    
    EGammaMvaEleEstimator* fMVA_;
   
    enum { kLoose, kTight };
    int cut_;

    int verbosity_;
};

#endif
