#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaPhotonSelector.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

VGammaPhotonSelector::VGammaPhotonSelector( const edm::ParameterSet& conf ) {

  std::string verStr(conf.getParameter<std::string>("version"));

  if( verStr == "Jul022010")
    version_ = Jul022010;
  else if( verStr == "Jul022010_poter" )
    version_ = Jul022010_poter;
  else if( verStr == "Jul022010_poterrel" )
    version_ = Jul022010_poterrel;
  else if ( verStr == "Fsr2011Apr11" )
    version_ = Fsr2011Apr11;
  else
    throw cms::Exception("InvalidInput")
      << "\'version\' must be one of: Jul022010 Jul022010_poter" << std::endl
      << "Jul022010_poterrel Fsr2011Apr11" << std::endl;

  switch(version_) {
    case Jul022010:
    case Jul022010_poter:
    case Jul022010_poterrel:
      init( conf.getParameter<double>("BarrelJurrasicECALIsoConst"),
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
      break;
    case Fsr2011Apr11:
      init_Fsr2011Apr11(
        // 1. maximum super cluster pseudo-rapidity absolute value
        conf.getParameter<double>("maxAbsEtaSC"),
        // 2. exclude EB EE transition region |eta| in [1.4442, 1.566]
        int( conf.getParameter<bool>("excludeEBEEGap") ? 1 : 0),
        // 3. super cluster seed reco flag != EcalRecHit::kOutOfTime=2
        int( conf.getParameter<bool>("excludeOutOfTimeReco") ? 1 : 0),
        // 4. super cluster seed severity level != EcalSeverityLevelAlgo::kWeird=4
        int( conf.getParameter<bool>("excludeWeirdSeverity") ? 1 : 0),
        // 5. super cluster seed severity level != EcalSeverityLevelAlgo::kBad=5
        int( conf.getParameter<bool>("excludeBadSeverity") ? 1 : 0),
        // 6. maximum ecal isolation = const + slope * pt
        conf.getParameter<double>("ecalIsoConst"),
        conf.getParameter<double>("ecalIsoSlope"),
        // 7. minimum transverse momentum
        conf.getParameter<double>("minPt")
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

void VGammaPhotonSelector::init(/*const version& v,*/
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
//   version_ = v;

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
  case Fsr2011Apr11:
  case N_VERSIONS:
  default:
    throw cms::Exception("IllegalEnumValue")
      << "This should have been thrown previously!!!" << std::endl;
  } // end of switch(version_)
}


void VGammaPhotonSelector::init_Fsr2011Apr11(
      // 1. maximum super cluster pseudo-rapidity absolute value
      const double& maxAbsEtaSC,
      // 2. exclude EB EE transition region |eta| in [1.4442, 1.566]
      const bool& excludeEBEEGap,
      // 3. super cluster seed reco flag != EcalRecHit::kOutOfTime=2
      const bool& excludeOutOfTimeReco,
      // 4. super cluster seed severity level != EcalSeverityLevelAlgo::kWeird=4
      const bool& excludeWeirdSeverity,
      // 5. super cluster seed severity level != EcalSeverityLevelAlgo::kBad=5
      const bool& excludeBadSeverity,
      // 6. maximum ecal isolation < const + slope * pt
      const double& ecalIsoConst,
      const double& ecalIsoSlope,
      // 7. minimum transverse momentum
      const double& minPt
) {
  barJurECALIsoConst_ = endJurECALIsoConst_ = ecalIsoConst;
  barJurECALIsoSlope_ = endJurECALIsoSlope_ = ecalIsoSlope;

  push_back("inclusive");
  push_back("maxAbsEtaSC", maxAbsEtaSC);
  push_back("excludeEBEEGap", excludeEBEEGap);
  push_back("excludeOutOfTimeReco", excludeOutOfTimeReco);
  push_back("excludeWeirdSeverity", excludeWeirdSeverity);
  push_back("excludeBadSeverity", excludeBadSeverity);
  push_back("ecalIsolation");
  push_back("minPt", minPt);

  set("inclusive");
  set("maxAbsEtaSC");
  set("excludeEBEEGap");
  set("excludeOutOfTimeReco");
  set("excludeWeirdSeverity");
  set("excludeBadSeverity");
  set("ecalIsolation");
  set("minPt");
} // end of VGammaPhotonSelector::init_Fsr2011Apr11(...)

bool VGammaPhotonSelector::operator()( const pat::Photon & pho,
				       pat::strbitset &  ret) {
  switch (version_) {
  case Jul022010:          return Jul022010Cuts         (pho, ret);
  case Jul022010_poter:    return Jul022010_poterCuts   (pho, ret);
  case Jul022010_poterrel: return Jul022010_poterrelCuts(pho, ret);
  case Fsr2011Apr11:       return Fsr2011Apr11Cuts      (pho, ret);
  case N_VERSIONS:
  default:
    throw cms::Exception("IllegalEnumValue")
      << "This should have been thrown previously!!!" << std::endl;
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

bool VGammaPhotonSelector::Fsr2011Apr11Cuts( const pat::Photon& photon,
                                             pat::strbitset & ret ) {
  ret.set(false);
  setIgnored(ret);

  // 0. all photons
  passCut(ret, "inclusive");

  // 1. maximum super cluster pseudo-rapidity absolute value
  double scEta = photon.superCluster()->eta();
  if (fabs(scEta) < cut("maxAbsEtaSC", double()) || ignoreCut("maxAbsEtaSC") )
    passCut(ret, "maxAbsEtaSC");
  else return false;

  // 2. exclude EB EE transition region |eta| in [1.4442, 1.566]
  if (fabs(scEta) < 1.4442 || 1.566 < fabs(scEta) ||
      cut("excludeEBEEGap", int()) == 0 ||
      ignoreCut("excludeEBEEGap")
      )
    passCut(ret, "excludeEBEEGap");
  else return false;

  // 3. super cluster seed reco flag != EcalRecHit::kOutOfTime=2
  int seedRecoFlag = photon.userInt("photonUserData:seedRecoFlag");
  if (seedRecoFlag != EcalRecHit::kOutOfTime ||
      cut("excludeOutOfTimeReco", int()) == 0 ||
      ignoreCut("excludeOutOfTimeReco")
      )
    passCut(ret, "excludeOutOfTimeReco");
  else return false;

  // 4. super cluster seed severity level != EcalSeverityLevelAlgo::kWeird=4
  int sLevel = photon.userInt("photonUserData:seedSeverityLevel");
  //if (sLevel != EcalSeverityLevelAlgo::kWeird ||
  //    cut("excludeWeirdSeverity", int() ) == 0 ||
  //    ignoreCut("excludeWeirdSeverity")
  //    )
  //  passCut(ret, "excludeWeirdSeverity");
  //else return false;

  // 5. super cluster seed severity level != EcalSeverityLevelAlgo::kBad=5
  //if (sLevel != EcalSeverityLevelAlgo::kBad ||
  //    cut("excludeBadSeverity", int()) == 0 ||
  //    ignoreCut("excludeBadSeverity") 
  //    )
  //  passCut(ret, "excludeBadSeverity");
  //else return false;

  // 6. maximum ecal isolation = const + slope * pt
  double maxEcalIso = barJurECALIsoConst_ + barJurECALIsoSlope_ * photon.pt();
  if (photon.pt() > cut("ecalIsolation", double()) ||
      ignoreCut("ecalIsolation")
      )
    passCut(ret, "ecalIsolation");
  else return false;

  // 7. minimum transverse momentum
  if (photon.pt() > cut("minPt", double()) || ignoreCut("minPt") )
    passCut(ret, "minPt");
  else return false;

  return  (bool) ret;
} // VGammaPhotonSelector::Fsr2011Apr11Cuts(...)
