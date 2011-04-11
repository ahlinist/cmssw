#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <math.h>
VGammaMuonSelector::VGammaMuonSelector( const edm::ParameterSet& conf ) {

  std::string verStr(conf.getParameter<std::string>("version"));

  if( verStr == "Jun252010")
    version_ = Jun252010;
  else if( verStr == "Jun262010_jpsi" )
    version_ = Jun262010_jpsi;
  else if( verStr == "Sep062010" )
    version_ = Sep062010;
  else if ( verStr == "FsrApr082011" )
    version_ = FsrApr082011;
  else
    throw cms::Exception("InvalidInput")
            << "\'version\' must be one of: "
            << "Jun252010 Jun262010_jpsi Sep062010 FsrApr082011"
            << std::endl;

  switch (version_) {
    case Jun252010:
    case Jun262010_jpsi:
    case Sep062010:
      init( conf.getParameter<std::vector<std::string> >("muonTypeRequirement"),
            conf.getParameter<std::string>("TrackerMuonIdType"),
            conf.getParameter<unsigned>("minPixHits"),
            conf.getParameter<unsigned>("minTkHits"),
            conf.getParameter<double>("maxD0"),
            conf.getParameter<double>("maxDZ"),
            conf.getParameter<double>("maxChi2"),
            conf.getParameter<unsigned>("minChambers"),
            conf.getParameter<unsigned>("minStations"),
            conf.getParameter<double>("minP"),
            conf.getParameter<double>("minPt"),
            conf.getParameter<double>("maxEta"),
            conf.getParameter<double>("maxIso")
          );
      break;
    case FsrApr082011:
      FsrApr082011Init(
        // 1. muon is reconstructed as a "global muon" (out-in fit)
        int(conf.getParameter<bool>("isGlobalMuon") ? 1 : 0),
        // 2. maximum global muon fit normalized chi2
        conf.getParameter<double>("maxNormChi2"),
        // 3. minimum number of valid muon hits matched to the global fit
        conf.getParameter<unsigned>("minMuonHits"),
        // 4. muon is reconsturcted as a "tracker muon" (in-out fit)
        int(conf.getParameter<bool>("isTrackerMuon") ? 1 : 0),
        // 5. minimum number of muon stations with matched segments
        //    (global track: out-in fit)
        conf.getParameter<unsigned>("minMatches"),
        // 6. minimum number of tracker (pixels + strips) hits
        conf.getParameter<unsigned>("minTkHits"),
        // 7. minimum number of pixel hits
        conf.getParameter<unsigned>("minPixelHits"),
        // 8. maximum absolute value of the inner track transverse impact
        //    parameter w.r.t the beam spot
        conf.getParameter<double>("maxAbsTrackDxyBS"),
        // 9. sum of pT of tracks with pT >1.5 within a cone of DR < 0.3 around
        //    the muon direction, vetoing a cone of 0.015 around that direction
        conf.getParameter<double>("maxTrackIso"),
        // 10. minimum transverse momentum
        conf.getParameter<double>("minPt"),
        // 11. maximum pseudo-rapidity absolute value
        conf.getParameter<double>("maxAbsEta")
      );
      break;
    case N_VERSIONS: default:
      throw cms::Exception("ImpossibleOutcome")
        << "This should have been thrown previously!!!" << std::endl;
  } // end of switch(v)

  if( conf.exists("cutsToIgnore") )
    setIgnoredCuts( conf.getParameter<std::vector<std::string> >("cutsToIgnore") );

  retInternal_ = getBitTemplate();
}

void VGammaMuonSelector::init( const std::vector<std::string>& vmt, // valid muon types
                               const std::string & tmid,
                               const int & pixHits, // minimum number of pix hits
                               const int & tkHits, // min. tk hits
                               const double & d0, // d0 w.r.t. beam spot
                               const double & dz, // dz w.r.t. beam spot
                               const double & chi2, // global chi2/ndof
                               const int & chambers, // number of chambers in SA muon
                               const int & stations, // number of stations
                               const double & p, // min p
                               const double & pt, // min pt
                               const double & eta, // max eta
                               const double & maxIso // max iso
                               ) {
  muonTypeRequirement_ = vmt;
  TMidType_ = tmid;
  int vmt_size = vmt.size();
  push_back("MuonTypeRequirements", vmt_size);
  push_back("TrackerMuonIdType", 1);
  push_back("minPixHits", pixHits);
  push_back("minTkHits",  tkHits);
  push_back("d0", d0);
  push_back("dz", dz);
  push_back("maxNormChi2", chi2);
  push_back("minChambers", chambers);
  push_back("minStations", stations);
  push_back("minP", p);
  push_back("minPt", pt);
  push_back("maxEta", eta);
  push_back("maxIso", maxIso);

  set("MuonTypeRequirements");
  set("TrackerMuonIdType");
  set("minPixHits");
  set("minTkHits");
  set("d0");
  set("dz");
  set("maxNormChi2");
  set("minChambers");
  set("minStations");
  set("minP");
  set("minPt");
  set("maxEta");
  set("maxIso");

  switch (version_) {
  case Jun252010 :
    set("dz",false);
    set("minP",false);
    break;
  case Sep062010 :
    set("dz",false);
    set("minP",false);
    break;
  case Jun262010_jpsi :
    set("minPixHits",false);
    set("minChambers",false);
    set("minStations",false);
    set("maxIso",false);
    break;
  case FsrApr082011: case N_VERSIONS: default:
    throw cms::Exception("ImpossibleOutcome")
      << "This should have been thrown previously!!!" << std::endl;
  }
}

void VGammaMuonSelector::FsrApr082011Init(
  // 1. muon is reconstructed as a "global muon" (out-in fit)
  const int & isGlobalMuon,
  // 2. maximum global muon fit normalized chi2
  const double & maxNormChi2,
  // 3. minimum number of valid muon hits matched to the global fit
  const int & minMuonHits,
  // 4. muon is reconsturcted as a "tracker muon" (in-out fit)
  const bool & isTrackerMuon,
  // 5. minimum number of muon stations with matched segments
  //    (global track: out-in fit)
  const int & minMatches,
  // 6. minimum number of tracker (pixels + strips) hits
  const int & minTkHits,
  // 7. minimum number of pixel hits
  const int & minPixelHits,
  // 8. maximum inner track transverse impact parameter w.r.t the beam spot
  //    absolute value
  const double & maxAbsTrackDxyBS,
  // 9. sum of pT of tracks with pT >1.5 within a cone of DR < 0.3 around
  //    the muon direction, vetoing a cone of 0.015 around that direction
  const double & maxTrackIso,
  // 10. minimum transverse momentum
  const double & minPt,
  // 11. maximum pseudo-rapidity absolute value
  const double & maxAbsEta
) {
  push_back("Inclusive", 0);
  push_back("isGlobalMuon", isGlobalMuon);
  push_back("maxNormChi2", maxNormChi2);
  push_back("minMuonHits", minMuonHits);
  push_back("isTrackerMuon", isTrackerMuon);
  push_back("minMatches", minMatches);
  push_back("minTkHits", minTkHits);
  push_back("minPixelHits", minPixelHits);
  push_back("maxAbsTrackDxyBS", maxAbsTrackDxyBS);
  push_back("maxTrackIso", maxTrackIso);
  push_back("minPt", minPt);
  push_back("maxAbsEta", maxAbsEta);

  set("Inclusive");
  set("isGlobalMuon");
  set("maxNormChi2");
  set("minMuonHits");
  set("isTrackerMuon");
  set("minMatches");
  set("minTkHits");
  set("minPixelHits");
  set("maxAbsTrackDxyBS");
  set("maxTrackIso");
  set("minPt");
  set("maxAbsEta");
} // end of VGammaMuonSelector::FsrApr082011Init(...)

bool VGammaMuonSelector::operator()( const pat::Muon & mu,
                                     edm::EventBase const & evt,
                                     pat::strbitset & ret ) {
  return operator()(mu, ret);
}

bool VGammaMuonSelector::operator()( const pat::Muon & mu,
                                     pat::strbitset &  ret) {
  switch (version_) {
    case Jun252010     : return Jun252010Cuts     (mu, ret);
    case Jun262010_jpsi: return Jun262010_jpsiCuts(mu, ret);
    case Sep062010     : return Sep062010Cuts     (mu, ret);
    case FsrApr082011  : return FsrApr082011Cuts  (mu, ret);
    case N_VERSIONS: default:
      throw cms::Exception("ImpossibleOutcome")
        << "This should have been thrown previously!!!" << std::endl;
  }
  return false;
}

bool VGammaMuonSelector::Jun252010Cuts( const pat::Muon& mu,
                                        pat::strbitset & ret ) {
  ret.set(false);

  if(!mu.innerTrack().isNonnull()) return false;
  if(!mu.globalTrack().isNonnull()) return false; // the basically requires a global muon.

  double pt(mu.pt()), eta(fabs(mu.eta())), norm_chi2(mu.normChi2()), corr_d0(mu.dB()),
    relIso((mu.trackIso() + mu.caloIso())/pt);
  int pixHits(mu.innerTrack()->hitPattern().numberOfValidPixelHits()),
    tkHits(mu.innerTrack()->hitPattern().numberOfValidStripHits()),
    //   muHits(mu.outerTrack()->hitPattern().numberOfValidMuonDTHits()+
    //   mu.outerTrack()->hitPattern().numberOfValidMuonCSCHits()),
    chambers(mu.numberOfMatches()),
    stations(0);
  unsigned stationMask(mu.stationMask());
  int typeMatches(0), tkMuId(0);

  for(unsigned i=0; i < 8; ++i)
    if(stationMask & 1 << i) ++stations;

  for(unsigned i=0;i<muonTypeRequirement_.size();++i)
    if(mu.muonID(muonTypeRequirement_[i])) ++typeMatches;

  if(mu.muonID(TMidType_)) tkMuId = 1;

  if(typeMatches == cut("MuonTypeRequirements", int()) || ignoreCut("MuonTypeRequirements")) passCut(ret,"MuonTypeRequirements");
  if(tkMuId == cut("TrackerMuonIdType", int()) || ignoreCut("TrackerMuonIdType")) passCut(ret,"TrackerMuonIdType");
  if(pixHits > cut("minPixHits",int()) || ignoreCut("minPixHits")) passCut(ret,"minPixHits");
  if(tkHits > cut("minTkHits",int()) || ignoreCut("minTkHits")) passCut(ret,"minTkHits");
  if(corr_d0 < cut("d0", double()) || ignoreCut("d0")) passCut(ret,"d0");
  if(norm_chi2 < cut("maxNormChi2", double()) || ignoreCut("maxNormChi2")) passCut(ret,"maxNormChi2");
  if(chambers > cut("minChambers", double()) || ignoreCut("minChambers")) passCut(ret,"minChambers");
  if(stations > cut("minStations", double()) || ignoreCut("minStations")) passCut(ret,"minStations");
  if(pt > cut("minPt", double()) || ignoreCut("minPt")) passCut(ret,"minPt");
  if(eta < cut("maxEta", double()) || ignoreCut("maxEta")) passCut(ret,"maxEta");
  if(relIso < cut("maxRelIso", double()) || ignoreCut("maxRelIso")) passCut(ret,"maxRelIso");

  setIgnored(ret);

  return (bool)ret;
}

bool VGammaMuonSelector::Sep062010Cuts( const pat::Muon& mu,
                                        pat::strbitset & ret ) {
  ret.set(false);

  if(!mu.innerTrack().isNonnull()) return false;
  if(!mu.globalTrack().isNonnull()) return false; // the basically requires a global muon.

  double pt(mu.pt()), eta(fabs(mu.eta())), norm_chi2(mu.normChi2()), corr_d0(mu.dB()),
    Iso(mu.trackIso());
  int pixHits(mu.innerTrack()->hitPattern().numberOfValidPixelHits()),
    tkHits(mu.innerTrack()->hitPattern().numberOfValidStripHits()),
    //    muHits(mu.outerTrack()->hitPattern().numberOfValidMuonDTHits()+
    //    mu.outerTrack()->hitPattern().numberOfValidMuonCSCHits()),
    chambers(mu.numberOfMatches()),
    stations(0);
  unsigned stationMask(mu.stationMask());
  int typeMatches(0), tkMuId(0);

  for(unsigned i=0; i < 8; ++i)
    if(stationMask & 1 << i) ++stations;

  for(unsigned i=0;i<muonTypeRequirement_.size();++i)
    if(mu.muonID(muonTypeRequirement_[i])) ++typeMatches;

  if(mu.muonID(TMidType_)) tkMuId = 1;

  if(typeMatches == cut("MuonTypeRequirements", int()) || ignoreCut("MuonTypeRequirements")) passCut(ret,"MuonTypeRequirements");
  if(tkMuId == cut("TrackerMuonIdType", int()) || ignoreCut("TrackerMuonIdType")) passCut(ret,"TrackerMuonIdType");
  if(pixHits > cut("minPixHits",int()) || ignoreCut("minPixHits")) passCut(ret,"minPixHits");
  if(tkHits > cut("minTkHits",int()) || ignoreCut("minTkHits")) passCut(ret,"minTkHits");
  if(corr_d0 < cut("d0", double()) || ignoreCut("d0")) passCut(ret,"d0");
  if(norm_chi2 < cut("maxNormChi2", double()) || ignoreCut("maxNormChi2")) passCut(ret,"maxNormChi2");
  if(chambers > cut("minChambers", double()) || ignoreCut("minChambers")) passCut(ret,"minChambers");
  if(stations > cut("minStations", double()) || ignoreCut("minStations")) passCut(ret,"minStations");
  if(pt > cut("minPt", double()) || ignoreCut("minPt")) passCut(ret,"minPt");
  if(eta < cut("maxEta", double()) || ignoreCut("maxEta")) passCut(ret,"maxEta");
  if(Iso < cut("maxIso", double()) || ignoreCut("maxIso")) passCut(ret,"maxIso");

  setIgnored(ret);

  return (bool)ret;
}


bool VGammaMuonSelector::Jun262010_jpsiCuts( const pat::Muon& mu,
                                             pat::strbitset & ret ) {
  ret.set(false);

  if(!mu.innerTrack().isNonnull()) return false;

  double pt(mu.pt()), eta(fabs(mu.eta())), norm_chi2(mu.innerTrack()->normalizedChi2()), dz(mu.innerTrack()->dz()),
    corr_d0(mu.dB()), p(mu.p());
  int tkHits(mu.innerTrack()->found());
  int typeMatches(0), tkMuId(0);

  for(unsigned i=0;i<muonTypeRequirement_.size();++i)
    if(mu.muonID(muonTypeRequirement_[i])) ++typeMatches;

  if(mu.muonID(TMidType_)) tkMuId = 1;

  if(typeMatches == cut("MuonTypeRequirements", int()) || ignoreCut("MuonTypeRequirements")) passCut(ret,"MuonTypeRequirements");
  if(tkMuId == cut("TrackerMuonIdType", int()) || ignoreCut("TrackerMuonIdType")) passCut(ret,"TrackerMuonIdType");
  if(tkHits > cut("minTkHits",int()) || ignoreCut("minTkHits")) passCut(ret,"minTkHits");
  if(norm_chi2 < cut("maxNormChi2", double()) || ignoreCut("maxNormChi2")) passCut(ret,"maxNormChi2");
  if(corr_d0 < cut("d0", double()) || ignoreCut("d0")) passCut(ret,"d0");
  if(dz < cut("dz", double()) || ignoreCut("dz")) passCut(ret,"dz");
  if(p > cut("minP", double()) || ignoreCut("minP")) passCut(ret,"minP");
  if(eta < cut("maxEta",double()) || ignoreCut("maxEta")) passCut(ret,"maxEta");
  if(pt > cut("minPt",double()) || ignoreCut("minPt")) passCut(ret,"minPt");

  setIgnored(ret);

  return (bool)ret;
}


bool VGammaMuonSelector::FsrApr082011Cuts( const pat::Muon& mu,
                                           pat::strbitset & ret ) {
  ret.set(false);
  setIgnored(ret);

  // 0. all muons
  passCut(ret, "Inclusive");

  // 1. muon is reconstructed as a "global muon" (out-in fit)
  int isGlobalMuon( mu.isGlobalMuon() ? 1 : 0 );
  if (isGlobalMuon == cut("isGlobalMuon", int()) ||
      ignoreCut("isGlobalMuon") )
    passCut(ret, "isGlobalMuon");
  else return false;

  // 2. maximum global muon fit normalized chi2
  if (mu.normChi2() < cut("maxNormChi2", double()) ||
      ignoreCut("maxNormChi2") )
    passCut(ret, "maxNormChi2");
  else return false;

  // 3. minimum number of valid muon hits matched to the global fit
  int muonHits = 0;
  if ( mu.globalTrack().isNonnull() )
    muonHits = mu.globalTrack()->hitPattern().numberOfValidMuonHits();
  if (muonHits >= cut("minMuonHits", int()) ||
      ignoreCut("minMuonHits") )
    passCut(ret, "minMuonHits");
  else return false;

  // 4. muon is reconsturcted as a "tracker muon" (in-out fit)
  int isTrackerMuon( mu.isTrackerMuon() ? 1 : 0 );
  if (isTrackerMuon == cut("isTrackerMuon", int()) ||
      ignoreCut("isTrackerMuon") )
    passCut(ret, "isTrackerMuon");
  else return false;

  // 5. minimum number of muon stations with matched segments
  if (mu.numberOfMatches() >= cut("minMatches", int()) ||
      ignoreCut("minMatches") )
    passCut(ret, "minMatches");
  else return false;

  // 6. minimum number of tracker (pixels + strips) hits
  int tkHits = 0;
  if ( mu.track().isNonnull() )
    tkHits = mu.track()->numberOfValidHits();
  if (tkHits >= cut("minTkHits", int()) ||
      ignoreCut("minTkHits") )
    passCut(ret, "minTkHits");
  else return false;

  // 7. minimum number of pixel hits
  int pixelHits = 0;
  if ( mu.track().isNonnull() )
    pixelHits = mu.track()->hitPattern().numberOfValidPixelHits();
  if (pixelHits >= cut("minPixelHits", int()) ||
      ignoreCut("minPixelHits") )
    passCut(ret, "minPixelHits");
  else return false;

  // 8. maximum inner track transverse impact parameter w.r.t the beam spot
  //    absolute value
  double absInnerTrackDxyBS = fabs( mu.dB(pat::Muon::BS2D) );
  if (absInnerTrackDxyBS < cut("maxAbsTrackDxyBS", double()) ||
      ignoreCut("maxAbsTrackDxyBS") )
    passCut(ret, "maxAbsTrackDxyBS");
  else return false;

  // 9. sum of pT of tracks with pT >1.5 within a cone of DR < 0.3 around
  //    the muon direction, vetoing a cone of 0.015 around that direction
  if (mu.trackIso() < cut("maxTrackIso", double()) ||
      ignoreCut("maxTrackIso") )
    passCut(ret, "maxTrackIso");
  else return false;

  // 10. minimum transverse momentum
  if (mu.pt() > cut("minPt", double()) || ignoreCut("minPt") )
    passCut(ret, "minPt");
  else return false;

  // 11. minimum transverse momentum
  if (fabs( mu.eta() ) < cut("maxAbsEta", double()) || ignoreCut("maxAbsEta") )
    passCut(ret, "maxAbsEta");
  else return false;

  return (bool) ret;
}  // end of VGammaMuonSelector::FsrApr082011Cuts(...)
