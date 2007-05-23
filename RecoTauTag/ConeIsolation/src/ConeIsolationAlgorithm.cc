#include "RecoTauTag/ConeIsolation/interface/ConeIsolationAlgorithm.h"

// C++ standard library include files
#include <string>
#include <algorithm>
#include <cctype>     

// ROOT include files
#include <TMath.h>

// CMSSW include files
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PhysicsTools/IsolationUtils/interface/FixedAreaIsolationCone.h"

using namespace reco;

//
// declaration of auxiliary functions
//
int findStrIndex(const std::string& coneType, std::map<std::string, int>& supportedConeTypes, int& error);
void replaceSubStr(std::string& s, const std::string& oldSubStr, const std::string& newSubStr);

ConeIsolationAlgorithm::ConeIsolationAlgorithm()
{
//--- "default" constructor, used in case no edm::Parameterset is given;
//    no configuration parameters defined !
//
//    NOTE: this constructor should not be used (hence the Error output to the Log)
//
  edm::LogError("") << "No edm::Parameterset given --> Configuration Parameters undefined !" << std::endl;
}

ConeIsolationAlgorithm::ConeIsolationAlgorithm(const edm::ParameterSet& configuration)
  : trackSelector_(configuration)
{
//--- "standard" constructor for track selection;
//    configuration parameters taken from edm::Parameterset

//--- Pt threshold for tracks to be considered as "leading" track
//    (i.e. highest Pt track that defines the signal and isolation cone axes)
  leadingTrackMinPt_    = configuration.getParameter<double>("MinimumTransverseMomentumLeadingTrack"); 
  //otherTrackMinPt_    = configuration.getParameter<double>("MinimumTransverseMomentum");

//--- maximum distance along beam-axis 
//    between leading track and tracks taken into account when checking isolation
  trackMaxDzVertex_     = configuration.getParameter<double>("DeltaZetTrackVertex"); // to be renamed...

//--- flag steering whether or not tracks that are compatible with originating from the same interaction vertex as the leading track 
//    are taken into acount for the isolation critereon or not
  useVertexConstraint_  = configuration.getParameter<bool>("useVertexConstraint");

//--- size of "matching" cone around jet-axis
//    within which leading (i.e. highest Pt) track is searched for
  matchingConeSizeStr_  = configuration.getParameter<std::string>("MatchingConeSize");
  matchingConeSizeMin_  = configuration.getParameter<double>("MatchingConeSizeMin");
  matchingConeSizeMax_  = configuration.getParameter<double>("MatchingConeSizeMax");
  matchingConeTypeStr_  = configuration.getParameter<std::string>("MatchingConeType");

//--- size of "signal" cone around leading track
//    within which all tau decay products are expected to be contained
  signalConeSizeStr_    = configuration.getParameter<std::string>("SignalConeSize");
  signalConeSizeMin_    = configuration.getParameter<double>("SignalConeSizeMin");
  signalConeSizeMax_    = configuration.getParameter<double>("SignalConeSizeMax");
  signalConeTypeStr_    = configuration.getParameter<std::string>("SignalConeType");

//--- size of "isolation" cone around leading track;
//    the presence of tracks (passing the track selection) within this cone 
//    indicates that a jet is not a tau-jet, but a QCD-jet
  isolationConeSize_    = configuration.getParameter<double>("IsolationConeSize");
  isolationConeTypeStr_ = configuration.getParameter<std::string>("IsolationConeType");

//--- number of (selected) tracks within the isolation cone
//    tolerated for jets to pass as tau-jets
  numTrackIsolationRingMax_ = configuration.getParameter<unsigned int>("MaximumNumberOfTracksIsolationRing"); 

//--- parse configuration parameters passed in edm::ParameterSet
//    and check validity
  checkConfiguration();
}

void ConeIsolationAlgorithm::checkConfiguration()
{
//--- check type of matching cone
//
//    (currently) supported matching cone types are : "eta-phi"       for deltaR cone of size matchingConeSize given as configuration parameter;
//                                                                    defined in eta-phi metric
//                                                    "Opening Angle" for three-dimensional opening angle cone of size matchingConeSize given as configuration parameter;
//                                                                    defined by scalar product
//
  std::map<std::string, int> supportedMatchingConeTypes;
  supportedMatchingConeTypes["eta-phi"] = IsolatedTauTagInfo::kEtaPhiCone;
  supportedMatchingConeTypes["Opening Angle"] = IsolatedTauTagInfo::kOpeningAngleCone;

  int errorFlag = 0;
  matchingConeType_ = findStrIndex(matchingConeTypeStr_, supportedMatchingConeTypes, errorFlag);
  if ( errorFlag != 0 ) {
    throw cms::Exception("") << "\n unsupported Matching Cone Type " << matchingConeTypeStr_ << std::endl;
  }

  matchingConeSizeFormula_.SetName("matchingConeSize");
  checkFormula(matchingConeSizeFormula_, matchingConeSizeStr_, "Matching Cone Size");

//--- check type of signal cone
//
//    (currently) supported signal cone types are : "eta-phi"       for deltaR cone of size signalConeSize given as configuration parameter;
//                                                                defined in eta-phi metric
//                                                "Opening Angle" for three-dimensional opening angle cone of size signalConeSize given as configuration parameter;
//                                                                defined by scalar product
//
  std::map<std::string, int> supportedSignalConeTypes;
  supportedSignalConeTypes["eta-phi"] = IsolatedTauTagInfo::kEtaPhiCone;
  supportedSignalConeTypes["Opening Angle"] = IsolatedTauTagInfo::kOpeningAngleCone;

  signalConeType_ = findStrIndex(signalConeTypeStr_, supportedSignalConeTypes, errorFlag);
  if ( errorFlag != 0 ) {
    throw cms::Exception("") << "\n unsupported Signal Cone Type " << signalConeTypeStr_ << std::endl;
  }

  signalConeSizeFormula_.SetName("signalConeSize");
  checkFormula(signalConeSizeFormula_, signalConeSizeStr_, "Signal Cone Size");

//--- check type of isolation cone
//
//    (currently) supported isolation cone types are : "eta-phi"       for deltaR cone of size signalConeSize given as configuration parameter;
//                                                                     defined in eta-phi metric
//                                                     "Opening Angle" for three-dimensional opening angle cone of size signalConeSize given as configuration parameter;
//                                                                     defined by scalar product
//                                                     "fixed Area"    for three-dimensional opening angle cone the size of which is adjusted 
//                                                                     to shrink in the region of high particle density (i.e. at high |eta|), 
//				  	  			       such that the inefficiency caused by particles from the underlying event 
//                                                                     being by chance in the isolation cone is constant  
//
  std::map<std::string, int> supportedIsolationConeTypes;
  supportedIsolationConeTypes["eta-phi"] = IsolatedTauTagInfo::kEtaPhiCone;
  supportedIsolationConeTypes["Opening Angle"] = IsolatedTauTagInfo::kOpeningAngleCone;
  supportedIsolationConeTypes["fixed Area"] = IsolatedTauTagInfo::kFixedAreaIsolationCone;

  isolationConeType_ = findStrIndex(isolationConeTypeStr_, supportedIsolationConeTypes, errorFlag);
  if ( errorFlag != 0 ) {
    throw cms::Exception("") << "\n unsupported Isolation Cone Type " << isolationConeTypeStr_ << std::endl;
  }

//--- print configuration parameters
//    if debug output enabled
  if ( debugLevel_ > 0 ) {
    printConfiguration();
  }
}

ConeIsolationAlgorithm::~ConeIsolationAlgorithm()
{
//--- nothing to be done yet
}

void ConeIsolationAlgorithm::checkFormula(TFormula& coneSizeFormula, const std::string& coneSizeStr, const char* errorMessage)
{
//--- check functional form 
//    given as configuration parameter for matching and signal cone sizes;
//
//    The size of the matching and signal cones may depend on the energy "E" and/or transverse energy "ET" of the tau-jet candidate.
//    Any functional form that is supported by ROOT's TFormula class can be used (e.g. "3.0/E", "0.25/sqrt(ET)")
//
//    replace "E"  by TFormula variable "x"
//            "ET"                      "y"
  std::string tempConeSizeStr = coneSizeStr;
  replaceSubStr(tempConeSizeStr, "E",  "x");
  replaceSubStr(tempConeSizeStr, "ET", "y");

//--- check if TFormula supports the given parametric form 
  coneSizeFormula.SetName("coneSize");
  coneSizeFormula.SetTitle(tempConeSizeStr.data()); // the function definition is actually stored in the "Title" data-member of the TFormula object
  int errorFlag = coneSizeFormula.Compile();
  if ( errorFlag != 0 ) {
    throw cms::Exception("") << "\n unsupported functional Form for " << errorMessage << " " << signalConeSizeStr_ << std::endl
			     << "Please check that the Definition in \"" << coneSizeFormula.GetName() << "\" only contains the variables \"E\" or \"ET\""
			     << " and Functions that are supported by ROOT's TFormular Class." << std::endl;
  }
}

void ConeIsolationAlgorithm::printConfiguration()
{
//--- indent all sub-category printout by one space
//    for enhanced visual clarity
  const std::string indent = " ";

//--- print configuration parameters of track selection
  trackSelector_.printConfiguration(indent.data());

//--- print Pt threshold for leading track
  const std::string category = "ConeIsolationAlgorithm::printConfiguration";
  edm::LogVerbatim(category) << "<ConeIsolationAlgorithm::printConfiguration>:" << std::endl;
  edm::LogVerbatim(category) << "Leading Track:" << std::endl;
  edm::LogVerbatim(category) << indent << "minPt = " << leadingTrackMinPt_ << std::endl;
  
//--- print whether vertex constraint is applied or not
  edm::LogVerbatim(category) << "Vertex Constraint:" << std::endl;
  const std::string vertexOption = (useVertexConstraint_) ? "enabled" : "disabled";
  edm::LogVerbatim(category) << indent << vertexOption << std::endl;
  
//--- print size of matching cone
  edm::LogVerbatim(category) << "Matching Cone Parameters:" << std::endl;
  edm::LogVerbatim(category) << indent << "Size = " << matchingConeSizeStr_ << std::endl;
  edm::LogVerbatim(category) << indent << "Metric = " << matchingConeTypeStr_ << std::endl;
  
//--- print size and metric of signal cone
  edm::LogVerbatim(category) << "Signal Cone Parameters:" << std::endl;
  edm::LogVerbatim(category) << indent << "Size = " << signalConeSizeStr_ << std::endl;
  edm::LogVerbatim(category) << indent << "Metric = " << signalConeTypeStr_ << std::endl;

//--- print size and metric/type of isolation cone
  edm::LogVerbatim(category) << "Isolation Cone Parameters:" << std::endl;
  edm::LogVerbatim(category) << indent << "Size = " << isolationConeSize_ << std::endl;
  edm::LogVerbatim(category) << indent << "Metric/Type = " << isolationConeTypeStr_ << std::endl;

//--- print value of cut on number of (selected) tracks within the isolation cone
//    for jets to pass as tau-jets
  edm::LogVerbatim(category) << "Tau-Jet Selection Parameters:" << std::endl;
  edm::LogVerbatim(category) << indent << "numTrackIsolationRingMax = " << numTrackIsolationRingMax_ << std::endl;
  edm::LogVerbatim(category) << std::endl;
}

void ConeIsolationAlgorithm::printEvent(const reco::Vertex* primaryEventVertex)
{
  const std::string category = "ConeIsolationAlgorithm::printEvent";
  if ( primaryEventVertex ) {
    edm::LogVerbatim(category) << "primary Event Vertex:"
			       << " x = " << primaryEventVertex->position().x() << ","
			       << " y = " << primaryEventVertex->position().y() << ","
			       << " z = " << primaryEventVertex->position().z() << std::endl;
  } else {
    edm::LogVerbatim(category) << "no primary Event Vertex found" << std::endl;
  }
}

void ConeIsolationAlgorithm::printJet(const reco::Jet& jet, const reco::JetTag& jetTag, const reco::IsolatedTauTagInfo& isolatedTauTagInfo)
{
  const std::string category = "ConeIsolationAlgorithm::printJet";
  edm::LogVerbatim(category) << "<ConeIsolationAlgorithm::printJet>:" << std::endl;
  edm::LogVerbatim(category) << "Jet:"
			     << " E = " << jet.energy() << ","
			     << " theta = " << jet.theta()*180/TMath::Pi() << ","
			     << " phi = " << jet.phi()*180/TMath::Pi() << std::endl;

  double matchingConeSize = computeMatchingConeSize(jet, matchingConeSizeFormula_, matchingConeSizeMin_, matchingConeSizeMax_);
  double signalConeSize = computeSignalConeSize(jet, signalConeSizeFormula_, signalConeSizeMin_, signalConeSizeMax_);
  edm::LogVerbatim(category) << "matchingConeSize = " << matchingConeSize << std::endl;
  edm::LogVerbatim(category) << "signalConeSize = " << signalConeSize << std::endl;
  
  int errorFlag = 0;
  const reco::TrackRef leadingSignalTrack = isolatedTauTagInfo.leadingSignalTrack(matchingConeSize, matchingConeType_, leadingTrackMinPt_, errorFlag);
  if ( !leadingSignalTrack.isNull() ) {
    edm::LogVerbatim(category) << "leading Track:" 
			       << " Pt = " << leadingSignalTrack->pt() << ","
			       << " theta = " << leadingSignalTrack->theta()*180/TMath::Pi() << ","
			       << " phi = " << leadingSignalTrack->phi()*180/TMath::Pi() << std::endl;
  } else {
    edm::LogVerbatim(category) << "no leading Track found" << std::endl;
  }

  const reco::TrackRefVector& selectedTracks = isolatedTauTagInfo.selectedTracks();
  if ( !selectedTracks.isNull() ) {
    edm::LogVerbatim(category) << "selected Tracks:" << std::endl;
    for ( reco::TrackRefVector::const_iterator selectedTrack = selectedTracks.begin();
	  selectedTrack != selectedTracks.end(); ++selectedTrack ) {
      edm::LogVerbatim(category) << " Pt = " << (*selectedTrack)->pt() << ","
				 << " theta = " << (*selectedTrack)->theta()*180/TMath::Pi() << ","
				 << " phi = " << (*selectedTrack)->phi()*180/TMath::Pi() << std::endl;

      if ( !leadingSignalTrack.isNull() ) {
	Angle<math::XYZVector> metricAngle;
	DeltaR<math::XYZVector> metricDeltaR;
	edm::LogVerbatim(category) << "  Distance to leading Track =" 
				   << " " << metricAngle(leadingSignalTrack->momentum(), (*selectedTrack)->momentum())*180/TMath::Pi() << " degrees" << "," 
				   << " " << metricDeltaR(leadingSignalTrack->momentum(), (*selectedTrack)->momentum()) << " in eta-phi space" << std::endl;
      }
    }
  } else {
    edm::LogVerbatim(category) << "no Tracks selected" << std::endl;
  }

  if ( !leadingSignalTrack.isNull() ) {
    math::XYZVector coneAxis = leadingSignalTrack->momentum();
    int numTracksSignalCone = isolatedTauTagInfo.tracksInCone(coneAxis, signalConeSize, signalConeType_, 0., errorFlag).size();
    reco::TrackRefVector isolationConeTracks;
    switch ( isolationConeType_ ) {
    case reco::IsolatedTauTagInfo::kEtaPhiCone :
      edm::LogVerbatim(category) << "isolationConeSize = " << isolationConeSize_ << std::endl;
      isolationConeTracks = isolatedTauTagInfo.tracksInCone(coneAxis, isolationConeSize_, isolationConeType_, 0., errorFlag);
      break;
    case reco::IsolatedTauTagInfo::kOpeningAngleCone :
      edm::LogVerbatim(category) << "isolationConeSize = " << isolationConeSize_ << std::endl;
      isolationConeTracks = isolatedTauTagInfo.tracksInCone(coneAxis, isolationConeSize_, isolationConeType_, 0., errorFlag);
      break;
    case reco::IsolatedTauTagInfo::kFixedAreaIsolationCone :
      {
	double isolationConeArea = isolationConeSize_;
	int errorFlag = 0;
	const double etaMaxTrackingAcceptance = 2.5; // maximum pseudo-rapidity at which charged particle can be reconstructed in SiStrip + Pixel detectors
	FixedAreaIsolationCone fixedAreaIsolationCone;
	fixedAreaIsolationCone.setAcceptanceLimit(etaMaxTrackingAcceptance);
	double isolationConeOpeningAngle = fixedAreaIsolationCone(coneAxis.theta(), coneAxis.phi(), signalConeSize, isolationConeArea, errorFlag);
	if ( errorFlag != 0 ) {
	  edm::LogError(category) << "Calculation of Isolation Cone Size failed !" << std::endl;
	}
	edm::LogVerbatim(category) << "isolationConeArea = " << isolationConeArea << std::endl;
	edm::LogVerbatim(category) << "isolationConeOpeningAngle = " << isolationConeOpeningAngle << std::endl;
	isolationConeTracks = isolatedTauTagInfo.tracksInCone(coneAxis, isolationConeOpeningAngle, reco::IsolatedTauTagInfo::kOpeningAngleCone, 0., errorFlag);
      }
    }
    int numTracksIsolationCone = isolationConeTracks.size() - numTracksSignalCone;
    edm::LogVerbatim(category) << "number of Tracks in Signal Cone = " << numTracksSignalCone << std::endl;
    edm::LogVerbatim(category) << "number of Tracks in Isolation Cone = " << numTracksIsolationCone << std::endl;
  }

  edm::LogVerbatim(category) << "--> Discriminator = " << jetTag.discriminator() << std::endl;
  edm::LogVerbatim(category) << std::endl;
}

//
//-------------------------------------------------------------------------------
//

double ConeIsolationAlgorithm::computeMatchingConeSize(const Jet& jet, const TFormula& matchingConeSizeFormula, double matchingConeSizeMin, double matchingConeSizeMax) const
{
//--- compute size of matching cone
//    (possibly depending on energy and/or transverse energy of the jet);
//    right now, the calculation of matching and signal cone sizes is identical,
//    so use one and the same function for both
  double matchingConeSize = computeSignalConeSize(jet, matchingConeSizeFormula, matchingConeSizeMin, matchingConeSizeMax);

  return matchingConeSize;
}

double ConeIsolationAlgorithm::computeSignalConeSize(const Jet& jet, const TFormula& signalConeSizeFormula, double signalConeSizeMin, double signalConeSizeMax) const
{
//--- compute size of signal cone
//    (possibly depending on energy and/or transverse energy of the jet)
  double x = jet.energy(); // E
  double y = jet.energy()*sin(jet.theta()); // ET

  double signalConeSize = signalConeSizeFormula.Eval(x, y);

  if ( signalConeSize < signalConeSizeMin ) signalConeSize = signalConeSizeMin;
  if ( signalConeSize > signalConeSizeMax ) signalConeSize = signalConeSizeMax;

  return signalConeSize;
}

double ConeIsolationAlgorithm::computeDiscriminator(const Jet& jet, const IsolatedTauTagInfo& isolatedTauTagInfo, 
						    bool foundPrimaryEventVertex) const
{
  math::XYZVector jetAxis(jet.px(),jet.py(), jet.pz());

  double discriminator;
  int errorFlag = 0;
  double matchingConeSize = computeMatchingConeSize(jet, matchingConeSizeFormula_, matchingConeSizeMin_, matchingConeSizeMax_);
  double signalConeSize = computeSignalConeSize(jet, signalConeSizeFormula_, signalConeSizeMin_, signalConeSizeMax_);
  //if ( useVertexConstraint_ && foundPrimaryEventVertex ) {
  if ( useVertexConstraint_ ) {
//--- compute discriminator value taking only tracks 
//    that are compatible with originating from the same interaction vertex as the leading track into acount
//    (no Pt cut neccessary for tracks within isolation ring, 
//     as Pt cut already applied in track selection)
    discriminator = isolatedTauTagInfo.discriminator(jetAxis, matchingConeSize, matchingConeType_, leadingTrackMinPt_, 0., trackMaxDzVertex_, 
						     signalConeSize, signalConeType_, isolationConeSize_, isolationConeType_, 
						     numTrackIsolationRingMax_, errorFlag);
  } else {
//--- compute discriminator value taking all tracks into acount
    discriminator = isolatedTauTagInfo.discriminator(jetAxis, matchingConeSize, matchingConeType_, leadingTrackMinPt_, 0., 
						     signalConeSize, signalConeType_, isolationConeSize_, isolationConeType_, 
						     numTrackIsolationRingMax_, errorFlag);
  }

  if ( errorFlag != 0 ) {
    throw cms::Exception("") << "\n failed to compute Discriminator value !" << std::endl;
  }

  return discriminator;
}

//
//-------------------------------------------------------------------------------
//

std::pair<reco::JetTag, reco::IsolatedTauTagInfo> ConeIsolationAlgorithm::tag(const reco::JetTracksAssociationRef& jetTracksAssociation, 
                                                                              const reco::Vertex* primaryEventVertex) 
{
edm::LogVerbatim("DEBUG") << "ConeIsolationAlgorithm::tag" << std::endl;

//--- select tracks to be considered as leading track 
//    and taken into account for isolation critereon
  edm::RefVector<reco::TrackCollection> trackCollection = jetTracksAssociation->second;

  edm::RefVector<reco::TrackCollection> selectedTrackCollection;
  for ( edm::RefVector<reco::TrackCollection>::const_iterator track = trackCollection.begin();
	track != trackCollection.end(); ++track ) {
    if ( trackSelector_(**track, primaryEventVertex) ) {
      selectedTrackCollection.push_back(*track);
    }
  }

//--- create IsolatedTauTagInfo object
//    (this constitutes the "extended" product)
  IsolatedTauTagInfo isolatedTauTagInfo(selectedTrackCollection, jetTracksAssociation);

  const Jet& jet = (*jetTracksAssociation->first);
  bool foundPrimaryEventVertex = (primaryEventVertex != NULL) ? true : false;
  double discriminator = computeDiscriminator(jet, isolatedTauTagInfo, foundPrimaryEventVertex);

//--- create JetTag object
//    (this constitutes the "base" product)
  JetTag jetTag(discriminator);

  if ( debugLevel_ > 0 ) {
    printJet(jet, jetTag, isolatedTauTagInfo);
  }

  return std::pair<JetTag, IsolatedTauTagInfo> (jetTag, isolatedTauTagInfo);
}

//
//-------------------------------------------------------------------------------
//

int findStrIndex(const std::string& coneType, std::map<std::string, int>& supportedConeTypes, int& error)
{
//--- reset error flag
//   
//    error codes : 1 = undefined cone type
//
  error = 0;

//--- convert all strings to upper-case;
//    in order to make matching between string given as configuration parameter
//    and "supported" strings case-insensitive
  std::string coneTypeStr = coneType;
  std::transform(coneTypeStr.begin(), coneTypeStr.end(), coneTypeStr.begin(), (int(*)(int))std::toupper);

  for ( std::map<std::string, int>::iterator supportedConeType = supportedConeTypes.begin();
	supportedConeType != supportedConeTypes.end(); ++supportedConeType ) {
    std::string supportedConeTypeStr = supportedConeType->first;
    std::transform(supportedConeTypeStr.begin(), supportedConeTypeStr.end(), supportedConeTypeStr.begin(), (int(*)(int))std::toupper);
    
//--- add upper-case string to map
    supportedConeTypes[supportedConeTypeStr] = supportedConeType->second;
  }

//--- find string given as configuration parameter in list of "supported" strings
//    and return index of match
  int matchingConeType;
  std::map<std::string, int>::const_iterator matchingConeTypeDef = supportedConeTypes.find(coneTypeStr);
  if ( matchingConeTypeDef != supportedConeTypes.end() ) {
    matchingConeType = matchingConeTypeDef->second;
  } else {
    error = 1;
  }

  return matchingConeType;
}

void replaceSubStr(std::string& s, const std::string& oldSubStr, const std::string& newSubStr)
{
//--- protect replacement algorithm
//    from case that oldSubStr and newSubStr are equal
//    (nothing to be done anyway)
  if ( oldSubStr == newSubStr ) return;

//--- protect replacement algorithm
//    from case that oldSubStr contains no characters
//    (i.e. matches everything)
  if ( oldSubStr.empty() ) return;

  const std::string::size_type lengthOldSubStr = oldSubStr.size();
  const std::string::size_type lengthNewSubStr = newSubStr.size();

  std::string::size_type positionPreviousMatch = 0;
  std::string::size_type positionNextMatch = 0;

//--- consecutively replace all occurences of oldSubStr by newSubStr;
//    keep iterating until no occurence of oldSubStr left
  while ( (positionNextMatch = s.find(oldSubStr, positionPreviousMatch)) != std::string::npos ) {
    s.replace(positionNextMatch, lengthOldSubStr, newSubStr);
    positionPreviousMatch = positionNextMatch + lengthNewSubStr;
  } 
}
