#ifndef SiStripDigiValidation_Container_H
#define SiStripDigiValidation_Container_H

#include "SimDataFormats/StripDigiValidation/interface/StripDigiValidInfo.h"
#include <vector>

namespace edm{
   typedef std::vector<StripDigiValidInfo>  StripDigiValidInfoContainer;
} //edm

#endif
