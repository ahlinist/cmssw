#include "DetectorDescription/Core/src/Orb.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include <ostream>

void DDI::Orb::stream(std::ostream & os) const
{
  os << " rMax[cm]=" << p_[0]/cm;
}
