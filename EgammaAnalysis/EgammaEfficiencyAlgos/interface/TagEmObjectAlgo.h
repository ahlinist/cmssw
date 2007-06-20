#ifndef EgammaEfficiencyAlgos_TagEmObjectAlgo_h
#define EgammaEfficiencyAlgos_TagEmObjectAlgo_h

#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"

class TagEmObjectAlgo : public EmObjectAlgo
{
   public:
      virtual ~TagEmObjectAlgo(){};
      explicit TagEmObjectAlgo(){};

      virtual void initialise(const edm::ParameterSet&);
      virtual void run(const edm::Event&, EgEff::EmObjectCollection&);

   protected:
      std::string _inputProducer;
      std::string _electronIDAssocProducer;
};

#endif

