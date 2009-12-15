#include "RecoTauTag/TauAnalysisTools/interface/TauStringFunction.h"

template<typename Obj>
TauStringFunction<Obj>::TauStringFunction(const std::string &expr)
{
   expr_ = expr;
   try {
      // try to compile the given function
      std::cout << "Building expression: " << expr << std::endl;
      func_ = boost::shared_ptr<StringObjectFunction<Obj> >(new StringObjectFunction<Obj>(expr_));
   } catch (cms::Exception) {
      // if it doesn't compile, leave it empty
      std::cout << "Can't init pointer!" << std::endl;
      func_ =  boost::shared_ptr<StringObjectFunction<Obj> >();
   }
}

template<typename Obj>
double TauStringFunction<Obj>::evaluate(const CandBaseRef &c) const
{
   return evaluate(static_cast<const Obj&>(*c));
}

template<typename Obj>
double TauStringFunction<Obj>::evaluate(const Obj &c) const
{
   if(isValid())
   {
      return (*func_)(c);
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
