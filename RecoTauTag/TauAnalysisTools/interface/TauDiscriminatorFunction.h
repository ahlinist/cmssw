#ifndef RecoTauTag_TauAnalysisTools_TauDiscriminatorFunction_h
#define RecoTauTag_TauAnalysisTools_TauDiscriminatorFunction_h

#include "RecoTauTag/TauAnalysisTools/interface/TauFunction.h"
#include "DataFormats/Common/interface/Handle.h"

class TauDiscriminatorFunction : public TauFunction {
   public:
      TauDiscriminatorFunction(){};
      virtual ~TauDiscriminatorFunction(){};
   protected:
      double compute(const CandBaseRef &c) const;
      virtual double discriminate(const CandBaseRef &c) const = 0;
};

double 
TauDiscriminatorFunction::compute(const CandBaseRef &c) const
{
   return discriminate(c);
}

template<class TauType, class DiscriminatorType>
class TauDiscriminatorFunctionImpl : public TauDiscriminatorFunction {
   typedef edm::Ref<std::vector<TauType> > tau_ref;
   public:
      TauDiscriminatorFunctionImpl(const edm::Handle<DiscriminatorType> *handle):handle_(handle){};
      virtual ~TauDiscriminatorFunctionImpl(){};
      bool isValid() const;
   protected:
      double discriminate(const CandBaseRef &c) const;
   private:
      const edm::Handle<DiscriminatorType> *handle_;
};

template<class TauType, class DiscriminatorType>
bool
TauDiscriminatorFunctionImpl<TauType, DiscriminatorType>::isValid() const
{
   if( handle_ && handle_->isValid())
   {
      return true;
   }
   else 
      return false;
}

template<class TauType, class DiscriminatorType>
double
TauDiscriminatorFunctionImpl<TauType, DiscriminatorType>::discriminate(const CandBaseRef &c) const
{
   tau_ref specificRef = c.castTo<tau_ref>();
   //return (*handle_)[specificRef];
   double result = (**handle_)[specificRef];
   return result;
}


#endif
