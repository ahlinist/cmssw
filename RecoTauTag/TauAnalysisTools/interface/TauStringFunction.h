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
      double evaluate(const CandBaseRef &c) const;
   private:
      std::string expr_;
      boost::shared_ptr<StringObjectFunction<Obj> > func_;
};

template<typename Obj>
TauStringFunction<Obj>::TauStringFunction(const std::string &expr)
{
   expr_ = expr;
   try {
      // try to compile the given function
      func_ = boost::shared_ptr<StringObjectFunction<Obj> >(new StringObjectFunction<Obj>(expr_));
   } catch (cms::Exception) {
      // if it doesn't compile, leave it empty
      func_ =  boost::shared_ptr<StringObjectFunction<Obj> >();
   }
}

template<typename Obj>
double TauStringFunction<Obj>::evaluate(const CandBaseRef &c) const
{
   if(isValid())
   {
      return (*func_)(static_cast<const Obj&>(*c));
   } else
   {
      throw cms::Exception("InvalidStringFunction") << "String function " << name() 
         << " expression " << expr_ << " does not evaluate!";
   }
}

//double TauStringFunction<Obj>::compute(const reco::Candidate &c) const
template<typename Obj>
double TauStringFunction<Obj>::compute(const CandBaseRef &c) const
{
   return evaluate(c);
}

#endif
