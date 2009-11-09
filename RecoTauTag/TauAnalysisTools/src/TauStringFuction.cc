#include "RecoTauTag/TauAnalysisTools/interface/TauStringFunction.h"

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

template<typename Obj>
double TauStringFunction<Obj>::compute(const CandBaseRef &c) const
{
   return evaluate(c);
}
