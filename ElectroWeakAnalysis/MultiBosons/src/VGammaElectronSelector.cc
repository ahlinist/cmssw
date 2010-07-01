#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaElectronSelector.h"

VGammaElectronSelector::VGammaElectronSelector( const edm::ParameterSet& conf ) {
  
  std::string verStr(conf.getUntrackedParameter<std::string>("version"));
  
  version v;
  if( verStr == "Jul0162010")
    v = Jul012010;
  else
    throw cms::Exception("InvalidInput") << "\'version\' must be one of: Jul012010" << std::endl;
  
  init( v,
	conf.getParameter<std::string>("ElectronID"),
	conf.getParameter<int>("ElectronIDMinRank"),
	conf.getParameter<double>("minPt"),
	conf.getParameter<double>("maxEta")
       );

  if( conf.exists("cutToIgnore") )
    setIgnoredCuts( conf.getParameter<std::vector<std::string> >("cutsToIgnore") );
  
  retInternal_ = getBitTemplate();
}

void VGammaElectronSelector::init( const version& v,
				   std::string eid,
				   int eidlevel,
				   double minpt,
				   double maxeta
				  ) {
  version_ = v;
  eid_ = eid;
  push_back("ElectronID",eidlevel);
  push_back("minPt", minpt);
  push_back("maxEta", maxeta);
  
  set("ElectronID");  
  set("minPt");
  set("maxEta");
  
  switch (version_) {
  case Jul012010 :    
    break;
  default:
    throw cms::Exception("ImpossibleOutcome") << "This should have been thrown previously!!!" << std::endl;
  }
}

bool VGammaElectronSelector::operator()( const pat::Electron & ele, 
					 edm::EventBase const & evt, 
					 pat::strbitset & ret ) {
  switch (version_) {
  case Jul012010:
    return Jul012010Cuts(ele, ret);
    break;
  default:
    break;
  }
  return false;
}

bool VGammaElectronSelector::operator()( const pat::Electron & ele, 
					 pat::strbitset &  ret) {
  switch (version_) {
  case Jul012010:
    return Jul012010Cuts(ele, ret);
    break;
  default:
    break;
  }
  return false;
}

bool VGammaElectronSelector::Jul012010Cuts( const pat::Electron& ele, 
					    pat::strbitset & ret ) {
  ret.set(false);
  
  double pt(ele.pt()), eta(fabs(ele.eta()));
  bool passID(false);

  if(ele.isElectronIDAvailable(eid_))
    if(ele.electronID(eid_) >= cut("ElectronID",int())) passID = true;
    
  if(passID || ignoreCut("ElectronID")) passCut(ret,"ElectronID");  
  if(pt > cut("minPt", double()) || ignoreCut("minPt")) passCut(ret,"minPt");
  if(eta < cut("maxEta", double()) || ignoreCut("maxEta")) passCut(ret,"maxEta");
    
  return (bool)ret;
}

