#ifndef RecoTauTag_TauAnalysisTools_TauFunction_h
#define RecoTauTag_TauAnalysisTools_TauFunction_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/RefToBase.h"

class TauFunction {
   public:
      typedef edm::RefToBase<reco::Candidate> CandBaseRef;
      TauFunction(){};
      virtual ~TauFunction(){};
      //double operator()(const reco::Candidate &c) const;
      double operator()(const CandBaseRef &c) const;
      virtual bool isValid() const = 0;
      void setName(const std::string &name) { name_ = name; };
      const std::string& name() const { return name_; };
      // Not all functions should need this functionality, only 
      //   those which access another product.
      virtual void fillFrom(const edm::Event&);
   protected:
      virtual double compute( const CandBaseRef &c) const = 0;
   private:
      std::string name_;
};

void 
TauFunction::fillFrom(const edm::Event&)
{
   // do nothing
}

/*
double 
TauFunction::operator()(const reco::Candidate &c) const
{
   return compute(c);
}
double 
TauFunction::operator()(const CandBaseRef &c) const
{
   return computeOnRef(c);
}
*/

double 
TauFunction::operator()(const CandBaseRef &c) const
{
   return compute(c);
}

// Default case for functions that don't need a ref - convert to normal 
//   candidate.  In cases where a ref is required this can be overloaded
//   to throw an exception.

/*
double
TauFunction::computeOnRef(const CandBaseRef &c) const 
{
   const reco::Candidate& cand = *c;
   return compute(cand);
}
*/

#endif
