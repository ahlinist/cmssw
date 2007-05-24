#ifndef EgammaEfficiencyAlgos_McEmObjectAlgo_h
#define EgammaEfficiencyAlgos_McEmObjectAlgo_h

#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"
#include <string>

class McEmObjectAlgo : public EmObjectAlgo
{
   public:
      virtual ~McEmObjectAlgo(){};
      explicit McEmObjectAlgo(){};

      virtual void initialise(const edm::ParameterSet&);
      virtual void run(const edm::Event&, EgEff::EmObjectCollection&);

   protected:
      std::string inputProducer_;
      bool fromMother_;
      int motherPdgId_;
};

#endif

