#ifndef RecoTauTag_TauStringFunction_h
#define RecoTauTag_TauStringFunction_h

#include "RecoTauTag/TauAnalysisTools/interface/TauFunction.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include <boost/shared_ptr.hpp>

template<typename Obj>
class TauStringFunction : public TauFunction {
   public:
      TauStringFunction(const std::string &expr);
      virtual ~TauStringFunction(){};
      bool isValid() const { return func_.get(); };
   protected:
      virtual double compute(const CandBaseRef &c) const;
      // Will be casted automatically to template type
      double evaluate(const CandBaseRef &c) const;
      double evaluate(const Obj &c) const;
   private:
      std::string expr_;
      boost::shared_ptr<StringObjectFunction<Obj> > func_;
};

// Templated implementation in 
//
// RecoTauTag/TauAnalysisTools/src/TauStringFuction.cc     
//

#endif
