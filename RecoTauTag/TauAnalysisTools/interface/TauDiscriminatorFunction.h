#ifndef RecoTauTag_TauAnalysisTools_TauDiscriminatorFunction_h
#define RecoTauTag_TauAnalysisTools_TauDiscriminatorFunction_h

#include "RecoTauTag/TauAnalysisTools/interface/TauFunction.h"
#include "DataFormats/Common/interface/Handle.h"

template<class TauType, class DiscriminatorType>
class TauDiscriminatorFunction : public TauFunction {
   typedef edm::Ref<std::vector<TauType> > tau_ref;
   public:
      TauDiscriminatorFunction(const edm::Handle<DiscriminatorType> *handle):handle_(handle){};
      virtual ~TauDiscriminatorFunction(){};
      bool isValid() const;
   protected:
      double compute(const CandBaseRef &c) const;
   private:
      const edm::Handle<DiscriminatorType> *handle_;
};

// Implementation in RecoTauTag/TauAnalysisTools/src/TauDiscriminatorFunction.cc

#endif
