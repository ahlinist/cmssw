#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaDiLeptonSelector.h"

VGammaDiLeptonSelector::VGammaDiLeptonSelector( const edm::ParameterSet& conf ) {

  std::string version(conf.getParameter<std::string>("version"));

  if ( version == "Fsr2011Apr11" )
    version_ = Fsr2011Apr11;
  else
    throw cms::Exception("InvalidInput")
      << "Received version `" << version << "'" << std::endl
      << "The only supported \'version\' is Fsr2011Apr11." << std::endl;

  switch(version_) {
    case Fsr2011Apr11:
      init_Fsr2011Apr11(
        // 1. total charge
        conf.getParameter<int>("charge"),
        // 2. minimum invariant mass
        conf.getParameter<double>("minMass"),
        // 3. maximum invariant mass
        conf.getParameter<double>("maxMass")
      );
      break;
    case N_VERSIONS:
    default:
      throw cms::Exception("IllegalEnumValue")
        << "This should have been thrown previously!!!" << std::endl;
  } // end of switch(version_)

  if( conf.exists("cutsToIgnore") )
    setIgnoredCuts( conf.getParameter<std::vector<std::string> >("cutsToIgnore") );

  retInternal_ = getBitTemplate();
}

void VGammaDiLeptonSelector::init_Fsr2011Apr11(
  // 1. total charge
  const int& charge,
  // 2. minimum invariant mass
  const double &minMass,
  // 3. maximum invariant mass
  const double &maxMass
) {
  push_back("inclusive");
  push_back("charge", charge);
  push_back("minMass", minMass);
  push_back("maxMass", maxMass);

  set("inclusive");
  set("charge");
  set("minMass");
  set("maxMass");
} // end of VGammaDiLeptonSelector::init_Fsr2011Apr11(...)

bool VGammaDiLeptonSelector::operator()(
  const reco::CompositeCandidate & dilepton,
  pat::strbitset &  ret
  )
{
  switch (version_) {
  case Fsr2011Apr11: return passes_Fsr2011Apr11(dilepton, ret);
  case N_VERSIONS:
  default:
    throw cms::Exception("IllegalEnumValue")
      << "This should have been thrown previously!!!" << std::endl;
  }
  return false;
}

bool VGammaDiLeptonSelector::passes_Fsr2011Apr11(
  const reco::CompositeCandidate& dilepton,
  pat::strbitset & ret
  )
{
  ret.set(false);
  setIgnored(ret);

  // 0. all dileptons
  passCut(ret, "inclusive");

  // 1. total charge
  if (dilepton.charge() == cut("charge", int()) ||
      ignoreCut("charge")
      )
    passCut(ret, "charge");
  else return false;

  // 2. minimum invariant mass
  if (dilepton.mass() > cut("minMass", double()) ||
      ignoreCut("minMass")
      )
    passCut(ret, "minMass");
  else return false;

  // 3. maximum invariant mass
  if (dilepton.mass() < cut("maxMass", double()) ||
      ignoreCut("maxMass")
      )
    passCut(ret, "maxMass");
  else return false;

  return  (bool) ret;
} // VGammaDiLeptonSelector::passes_Fsr2011Apr11Cuts(...)
