
#ifndef __stdhep2partonWrap__
#define __stdhep2partonWrap__

#include <string>
#include "Validation/VstQuaeroUtils/interface/HepevtBlock.hh"

void stdhep2partonWrap(std::string stdhepFileName, std::string txtFileName);
void stdhep2partonWrapSimple(std::string txtFileName, const HepevtEvent&);

#endif
