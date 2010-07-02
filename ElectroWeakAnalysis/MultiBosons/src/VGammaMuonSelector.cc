#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaMuonSelector.h"

VGammaMuonSelector::VGammaMuonSelector( const edm::ParameterSet& conf ) {
  
  std::string verStr(conf.getUntrackedParameter<std::string>("version"));
  
  version v;
  if( verStr == "Jun252010")
    v = Jun252010;
  else if( verStr == "Jun262010_jpsi" )
    v = Jun262010_jpsi;
  else
    throw cms::Exception("InvalidInput") << "\'version\' must be one of: Jun252010 Jun262010_jpsi" << std::endl;
  
  init( v,
	conf.getParameter<std::vector<std::string> >("muonTypeRequirement"),
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
	conf.getParameter<double>("maxRelIso")
      );
  if( conf.exists("cutToIgnore") )
    setIgnoredCuts( conf.getParameter<std::vector<std::string> >("cutsToIgnore") );
  
  retInternal_ = getBitTemplate();
}

void VGammaMuonSelector::init( const version& v,
			       const std::vector<std::string>& vmt, // valid muon types
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
			       const double & relIso // max rel comb iso
			       ) {
  version_ = v;
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
  push_back("maxRelIso", relIso);
  
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
  set("maxRelIso");
  
  switch (version_) {
  case Jun252010 : 
    set("dz",false);
    set("minP",false);
    break;
  case Jun262010_jpsi :
    set("minPixHits",false);
    set("minChambers",false);
    set("minStations",false);
    set("maxRelIso",false);
    break;
  default:
    throw cms::Exception("ImpossibleOutcome") << "This should have been thrown previously!!!" << std::endl;
  }
}

bool VGammaMuonSelector::operator()( const pat::Muon & mu, 
				     edm::EventBase const & evt, 
				     pat::strbitset & ret ) {
  switch (version_) {
  case Jun252010:
    return Jun252010Cuts(mu, ret);
    break;
  case Jun262010_jpsi:
    return Jun262010_jpsiCuts(mu, ret);
    break;
  default:
    break;
  }
  return false;
}

bool VGammaMuonSelector::operator()( const pat::Muon & mu, 
				     pat::strbitset &  ret) {
  switch (version_) {
  case Jun252010:
    return Jun252010Cuts(mu, ret);
    break;
  case Jun262010_jpsi:
    return Jun262010_jpsiCuts(mu, ret);
    break;
  default:
    break;
  }
  return false;
}

bool VGammaMuonSelector::Jun252010Cuts( const pat::Muon& mu, 
					pat::strbitset & ret ) {
  ret.set(false);
  
  double pt(mu.pt()), eta(fabs(mu.eta())), norm_chi2(mu.normChi2()), corr_d0(mu.dB()),
    relIso((mu.trackIso() + mu.caloIso())/pt);
  int pixHits(mu.innerTrack()->hitPattern().numberOfValidPixelHits()),
    tkHits(mu.innerTrack()->hitPattern().numberOfValidStripHits()),
    //    muHits(mu.outerTrack()->hitPattern().numberOfValidMuonDTHits()+
    //	   mu.outerTrack()->hitPattern().numberOfValidMuonCSCHits()),
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
  if(tkMuId == cut("TrackerMuonIdType", int()) || ignoreCut("TypeMuonIdType")) passCut(ret,"TrackMuonIdType");
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

bool VGammaMuonSelector::Jun262010_jpsiCuts( const pat::Muon& mu,
					     pat::strbitset & ret ) {
  ret.set(false);
  
  double pt(mu.pt()), eta(fabs(mu.eta())), norm_chi2(mu.innerTrack()->normalizedChi2()), dz(mu.innerTrack()->dz()), 
    corr_d0(mu.dB()), p(mu.p());
  int tkHits(mu.innerTrack()->found());
  int typeMatches(0), tkMuId(0);
  
  for(unsigned i=0;i<muonTypeRequirement_.size();++i) 
    if(mu.muonID(muonTypeRequirement_[i])) ++typeMatches;
  
  if(mu.muonID(TMidType_)) tkMuId = 1;
  
  if(typeMatches == cut("MuonTypeRequirements", int()) || ignoreCut("MuonTypeRequirements")) passCut(ret,"MuonTypeRequirements");
  if(tkMuId == cut("TrackerMuonIdType", int()) || ignoreCut("TypeMuonIdType")) passCut(ret,"TrackMuonIdType");
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
