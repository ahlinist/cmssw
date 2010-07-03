#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonSelector.h"

VGammaPhotonSelector::VGammaPhotonSelector( const edm::ParameterSet& conf ) {
  
  std::string verStr(conf.getParameter<std::string>("version"));
  
  version v;
  if( verStr == "Jul022010")
    v = Jul022010;
  else if( verStr == "Jul022010_poter" )
    v = Jul022010_poter;
  else if( verStr == "Jul022010_poterrel" )
    v = Jul022010_poterrel;
  else
    throw cms::Exception("InvalidInput") << "\'version\' must be one of: Jul022010 Jul022010_poter" << std::endl;
    
    init( v,	
	  conf.getParameter<double>("BarrelJurrasicECALIsoConst"),
	  conf.getParameter<double>("BarrelJurrasicECALIsoSlope"),
	  conf.getParameter<double>("BarrelTowerHCALIsoConst"),	
	  conf.getParameter<double>("BarrelTowerHCALIsoSlope"),	
	  conf.getParameter<double>("BarrelMaxHadronicOverEm"),
	  conf.getParameter<double>("BarrelHollowConeTrkIsoConst"),
	  conf.getParameter<double>("BarrelHollowConeTrkIsoSlope"),
	  conf.getParameter<double>("BarrelMaxSigmaIetaIeta"),
	  conf.getParameter<double>("EndcapJurrasicECALIsoConst"),
	  conf.getParameter<double>("EndcapJurrasicECALIsoSlope"),
	  conf.getParameter<double>("EndcapTowerHCALIsoConst"),	
	  conf.getParameter<double>("EndcapTowerHCALIsoSlope"),	
	  conf.getParameter<double>("EndcapMaxHadronicOverEm"),
	  conf.getParameter<double>("EndcapHollowConeTrkIsoConst"),
	  conf.getParameter<double>("EndcapHollowConeTrkIsoSlope"),
	  conf.getParameter<double>("EndcapMaxSigmaIetaIeta"),
	  conf.getParameter<double>("minPt"),
	  conf.getParameter<double>("maxEta")
	);
    

  if( conf.exists("cutToIgnore") )
    setIgnoredCuts( conf.getParameter<std::vector<std::string> >("cutsToIgnore") );
  
  retInternal_ = getBitTemplate();
}

void VGammaPhotonSelector::init(const version& v,			      
				const double& barJurECALIsoConst,
				const double& barJurECALIsoSlope,
				const double& barTowHCALIsoConst,
				const double& barTowHCALIsoSlope,
				const double& barMaxHOverE,
				const double& barHConeTrkIsoConst,
				const double& barHConeTrkIsoSlope,
				const double& barMaxSigIEtaIEta,
				const double& endJurECALIsoConst,
				const double& endJurECALIsoSlope,
				const double& endTowHCALIsoConst,
				const double& endTowHCALIsoSlope,
				const double& endMaxHOverE,
				const double& endHConeTrkIsoConst,
				const double& endHConeTrkIsoSlope,
				const double& endMaxSigIEtaIEta,
				const double& minPt,
				const double& maxEta ) {
  version_ = v;

  barJurECALIsoConst_ = barJurECALIsoConst;
  barJurECALIsoSlope_ = barJurECALIsoSlope;
  barTowHCALIsoConst_ = barTowHCALIsoConst;	
  barTowHCALIsoSlope_ = barTowHCALIsoSlope;	
  barMaxHOverE_ = barMaxHOverE;
  barHConeTrkIsoConst_ = barHConeTrkIsoConst;
  barHConeTrkIsoSlope_ = barHConeTrkIsoSlope;
  barMaxSigIEtaIEta_ = barMaxSigIEtaIEta;
  endJurECALIsoConst_ = endJurECALIsoConst;
  endJurECALIsoSlope_ = endJurECALIsoSlope;
  endTowHCALIsoConst_ = endTowHCALIsoConst;	
  endTowHCALIsoSlope_ = endTowHCALIsoSlope;	
  endMaxHOverE_ = endMaxHOverE;
  endHConeTrkIsoConst_ = endHConeTrkIsoConst;
  endHConeTrkIsoSlope_ = endHConeTrkIsoSlope;
  endMaxSigIEtaIEta_ = endMaxSigIEtaIEta;
  
  push_back("PassJurrasicECALIso");
  push_back("PassTowerHCALIso");
  push_back("PassHadronicOverEm");
  push_back("PassHollowConeTrkIso");
  push_back("PassSigmaIetaIeta");
  push_back("minPt",minPt);
  push_back("maxEta",maxEta);
  
  set("PassJurrasicECALIso");
  set("PassTowerHCALIso");
  set("PassHadronicOverEm");
  set("PassHollowConeTrkIso");
  set("PassSigmaIetaIeta");
  set("minPt");
  set("maxEta");

  switch( version_ ) {
  case Jul022010:
    set("PassSigmaIetaIeta",false);
    break;
  case Jul022010_poter:
    break;
  case Jul022010_poterrel:
    break;
  default:
    break;
  }
  
}

bool VGammaPhotonSelector::operator()( const pat::Photon & pho, 
				       edm::EventBase const & evt, 
				       pat::strbitset & ret ) {
  switch (version_) {
  case Jul022010:
    return Jul022010Cuts(pho, ret);
    break;
  case Jul022010_poter:
    return Jul022010_poterCuts(pho, ret);
    break;
  case Jul022010_poterrel:
    return Jul022010_poterrelCuts(pho, ret);
  default:
    break;
  }
  return false;
}

bool VGammaPhotonSelector::operator()( const pat::Photon & pho, 
				       pat::strbitset &  ret) {
  switch (version_) {
  case Jul022010:
    return Jul022010Cuts(pho, ret);
    break;
  case Jul022010_poter:
    return Jul022010_poterCuts(pho, ret);
    break;
  case Jul022010_poterrel:
    return Jul022010_poterrelCuts(pho, ret);
  default:
    break;
  }
  return false;
}

bool VGammaPhotonSelector::Jul022010Cuts( const pat::Photon& pho, 
					  pat::strbitset & ret ) {
  ret.set(false);
  
  double pt(pho.et()), eta(fabs(pho.superCluster()->eta()));
  double ecaliso = pho.ecalRecHitSumEtConeDR04();
  double hcaliso = pho.hcalTowerSumEtConeDR04();
  double trkiso = pho.trkSumPtHollowConeDR04();
  double hovere = pho.hadronicOverEm();

  double maxecaliso(-1.0), maxhcaliso(-1.0), maxtrkiso(-1.0), maxhovere(-1.0);

  if(eta < 1.442) {
    maxecaliso = barJurECALIsoConst_ + barJurECALIsoSlope_*pt;
    maxhcaliso = barTowHCALIsoConst_ + barTowHCALIsoSlope_*pt;
    maxtrkiso  = barHConeTrkIsoConst_ + barHConeTrkIsoSlope_*pt;
    maxhovere   = barMaxHOverE_;
  }
  if(eta > 1.650) {
    maxecaliso = endJurECALIsoConst_ + endJurECALIsoSlope_*pt;
    maxhcaliso = endTowHCALIsoConst_ + endTowHCALIsoSlope_*pt;
    maxtrkiso  = endHConeTrkIsoConst_ + endHConeTrkIsoSlope_*pt;
    maxhovere   = endMaxHOverE_;
  }

  if(ignoreCut("PassJurrasicECALIso")  || ecaliso < maxecaliso) passCut(ret,"PassJurrasicECALIso");
  if(ignoreCut("PassTowerHCALIso")     || hcaliso < maxhcaliso) passCut(ret,"PassTowerHCALIso");
  if(ignoreCut("PassHadronicOverEm")   || hovere < maxhovere)   passCut(ret,"PassHadronicOverEm");
  if(ignoreCut("PassHollowConeTrkIso") || trkiso < maxtrkiso)   passCut(ret,"PassHollowConeTrkIso");
  if(pt > cut("minPt", double())       || ignoreCut("minPt"))   passCut(ret,"minPt");
  if(eta < cut("maxEta", double())     || ignoreCut("maxEta"))  passCut(ret,"maxEta");
  
  setIgnored(ret);  
  return (bool)ret;
}

bool VGammaPhotonSelector::Jul022010_poterCuts( const pat::Photon& pho,
						pat::strbitset & ret ) {
  ret.set(false);
  
  double pt(pho.pt()), eta(fabs(pho.eta()));
  double ecaliso = pho.ecalRecHitSumEtConeDR04();
  double hcaliso = pho.hcalTowerSumEtConeDR04();
  double trkiso = pho.trkSumPtHollowConeDR04();
  double hovere = pho.hadronicOverEm();
  double sigmaIetaIeta = pho.sigmaIetaIeta();
  
  double maxecaliso(-1.0), maxhcaliso(-1.0), maxtrkiso(-1.0), 
    maxhovere(-1.0), maxsigmaietaieta(-1.0);

  if(eta < 1.442) {
    maxecaliso = barJurECALIsoConst_ + barJurECALIsoSlope_*pt;
    maxhcaliso = barTowHCALIsoConst_ + barTowHCALIsoSlope_*pt;
    maxtrkiso  = barHConeTrkIsoConst_ + barHConeTrkIsoSlope_*pt;
    maxhovere   = barMaxHOverE_;
    maxsigmaietaieta = barMaxSigIEtaIEta_;
  }
  if(eta > 1.650) {
    maxecaliso = endJurECALIsoConst_ + endJurECALIsoSlope_*pt;
    maxhcaliso = endTowHCALIsoConst_ + endTowHCALIsoSlope_*pt;
    maxtrkiso  = endHConeTrkIsoConst_ + endHConeTrkIsoSlope_*pt;
    maxhovere   = endMaxHOverE_;
    maxsigmaietaieta = endMaxSigIEtaIEta_;
  }

  if(ignoreCut("PassJurrasicECALIso")  || ecaliso < maxecaliso) passCut(ret,"PassJurrasicECALIso");
  if(ignoreCut("PassTowerHCALIso")     || hcaliso < maxhcaliso) passCut(ret,"PassTowerHCALIso");
  if(ignoreCut("PassHadronicOverEm")   || hovere < maxhovere)   passCut(ret,"PassHadronicOverEm");
  if(ignoreCut("PassHollowConeTrkIso") || trkiso < maxtrkiso)   passCut(ret,"PassHollowConeTrkIso");
  if(ignoreCut("PassSigmaIetaIeta")    || sigmaIetaIeta < maxsigmaietaieta)   
    passCut(ret,"PassSigmaIetaIeta");
  if(eta < cut("maxEta",double())      || ignoreCut("maxEta")) passCut(ret,"maxEta");
  if(pt > cut("minPt",double())        || ignoreCut("minPt")) passCut(ret,"minPt");
  
  setIgnored(ret);
  return (bool)ret;
}

bool VGammaPhotonSelector::Jul022010_poterrelCuts( const pat::Photon& pho,
						   pat::strbitset & ret ) {
  ret.set(false);
  
  double pt(pho.et()), eta(fabs(pho.eta()));
  double ecaliso = pho.ecalRecHitSumEtConeDR04();
  double hcaliso = pho.hcalTowerSumEtConeDR04();
  double trkiso = pho.trkSumPtHollowConeDR04();
  double hovere = pho.hadronicOverEm();
  double sigmaIetaIeta = pho.sigmaIetaIeta();
  
  double maxecaliso(-1.0), maxhcaliso(-1.0), maxtrkiso(-1.0), 
    maxhovere(-1.0), maxsigmaietaieta(-1.0);

  if(eta < 1.442) {
    maxecaliso = barJurECALIsoConst_;
    maxhcaliso = barTowHCALIsoConst_;
    maxtrkiso  = barHConeTrkIsoConst_;
    maxhovere  = barMaxHOverE_;
    maxsigmaietaieta = barMaxSigIEtaIEta_;
  }
  if(eta > 1.650) {
    maxecaliso = endJurECALIsoConst_;
    maxhcaliso = endTowHCALIsoConst_;
    maxtrkiso  = endHConeTrkIsoConst_;
    maxhovere   = endMaxHOverE_;
    maxsigmaietaieta = endMaxSigIEtaIEta_;
  }

  if(ignoreCut("PassJurrasicECALIso")  || ecaliso/pt < maxecaliso) passCut(ret,"PassJurrasicECALIso");
  if(ignoreCut("PassTowerHCALIso")     || hcaliso/pt < maxhcaliso) passCut(ret,"PassTowerHCALIso");
  if(ignoreCut("PassHadronicOverEm")   || hovere < maxhovere)      passCut(ret,"PassHadronicOverEm");
  if(ignoreCut("PassHollowConeTrkIso") || trkiso/pt < maxtrkiso)   passCut(ret,"PassHollowConeTrkIso");
  if(ignoreCut("PassSigmaIetaIeta")    || sigmaIetaIeta < maxsigmaietaieta)   
    passCut(ret,"PassSigmaIetaIeta");
  if(eta < cut("maxEta",double()) || ignoreCut("maxEta")) passCut(ret,"maxEta");
  if(pt > cut("minPt",double()) || ignoreCut("minPt")) passCut(ret,"minPt");
  
  setIgnored(ret);  
  return (bool)ret;
}
