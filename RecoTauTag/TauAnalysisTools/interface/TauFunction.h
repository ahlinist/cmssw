#ifndef RecoTauTag_TauAnalysisTools_TauFunction_h
#define RecoTauTag_TauAnalysisTools_TauFunction_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "FWCore/Framework/interface/Event.h"

class TauFunction {
   public:
      typedef edm::RefToBase<reco::Candidate> CandBaseRef;
      TauFunction(){}
      virtual ~TauFunction(){}

      /// Public method to get function result for Candidate c
      double operator()(const CandBaseRef &c) const { return compute(c); }
      
      /// Abstract function that returns whether the function is well defined
      virtual bool isValid() const = 0;

      void setName(const std::string &name) { name_ = name; }
      const std::string& name() const { return name_; }

      // Not all functions should need this functionality, only 
      //   those which access another product.
      virtual void fillFrom(const edm::Event& evt) {}  // do nothing

   protected:
      /// Derived class should implement this function which 
      ///  implements the actual computation on the candidate
      virtual double compute( const CandBaseRef &c) const = 0;
   private:
      std::string name_;
};

#endif
