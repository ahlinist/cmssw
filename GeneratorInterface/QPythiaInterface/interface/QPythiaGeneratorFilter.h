#ifndef gen_QPythiaGeneratorFilter_h
#define gen_QPythiaGeneratorFilter_h

#include "GeneratorInterface/QPythiaInterface/interface/QPythiaHadronizer.h"
#include "GeneratorInterface/Core/interface/GeneratorFilter.h"
#include "GeneratorInterface/ExternalDecays/interface/ExternalDecayDriver.h"

namespace gen
{
   typedef edm::GeneratorFilter<gen::QPythiaHadronizer, gen::ExternalDecayDriver> QPythiaGeneratorFilter;
}

#endif
