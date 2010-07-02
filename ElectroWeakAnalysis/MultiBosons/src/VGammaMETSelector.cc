#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMETSelector.h"

VGammaMETSelector::VGammaMETSelector( const edm::ParameterSet& conf ) {
  
  std::string verStr(conf.getUntrackedParameter<std::string>("version"));
  
  version v;
  if( verStr == "Jul0262010")
    v = Jul022010;
  else
    throw cms::Exception("InvalidInput") << "\'version\' must be one of: Jul022010" << std::endl;
  
  init( v,
	conf.getParameter<double>("minMET")
       );

  if( conf.exists("cutToIgnore") )
    setIgnoredCuts( conf.getParameter<std::vector<std::string> >("cutsToIgnore") );
  
  retInternal_ = getBitTemplate();
}

void VGammaMETSelector::init( const version& v,
				   double minmet
				  ) {
  version_ = v;

  push_back("minMET", minmet);
  
  
  switch (version_) {
  case Jul022010 :    
    break;
  default:
    throw cms::Exception("ImpossibleOutcome") << "This should have been thrown previously!!!" << std::endl;
  }
}

bool VGammaMETSelector::operator()( const pat::MET & met, 
					 edm::EventBase const & evt, 
					 pat::strbitset & ret ) {
  switch (version_) {
  case Jul022010:
    return Jul022010Cuts(met, ret);
    break;
  default:
    break;
  }
  return false;
}

bool VGammaMETSelector::operator()( const pat::MET & met, 
					 pat::strbitset &  ret) {
  switch (version_) {
  case Jul022010:
    return Jul022010Cuts(met, ret);
    break;
  default:
    break;
  }
  return false;
}

bool VGammaMETSelector::Jul022010Cuts( const pat::MET& theMET, 
					    pat::strbitset & ret ) {
  ret.set(false);
  
  double met(theMET.et());
    
  if(met > cut("minMET", double()) || ignoreCut("minMET")) passCut(ret,"minMET");
    
  return (bool)ret;
}

