#ifndef RecoTauTag_TauAnalysisTools_TauDecayModeFunction_h
#define RecoTauTag_TauAnalysisTools_TauDecayModeFunction_h

#include "RecoTauTag/TauAnalysisTools/interface/TauStringFunction.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TauReco/interface/PFTauDecayModeAssociation.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"

class TauDecayModeFunction : public TauStringFunction<reco::PFTauDecayMode> {
   public:
      typedef edm::Ref<std::vector<reco::PFTau > > tau_ref;
      typedef edm::Ref<std::vector<reco::PFTauDecayMode > > decaymode_ref;
      TauDecayModeFunction(const edm::Handle<reco::PFTauDecayModeAssociation> *handle, 
            const std::string &expr):TauStringFunction<reco::PFTauDecayMode>(expr),handle_(handle){};
      virtual ~TauDecayModeFunction(){};
      bool isValid() const;
   protected:
      double compute(const CandBaseRef &c) const;
   private:
      const edm::Handle<reco::PFTauDecayModeAssociation> *handle_;
};

// Implementation in RecoTauTag/TauAnalysisTools/src/TauDecayModeFunction.cc

#endif
