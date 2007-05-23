#ifndef RecoTauTag_ConeIsolation_ConeIsolationAlgorithm_h
#define RecoTauTag_ConeIsolation_ConeIsolationAlgorithm_h

// C++ standard library include files
#include <string>

// ROOT include files
#include <TFormula.h>

// CMSSW include files
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/Jet.h"

#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "RecoTauTag/ConeIsolation/interface/BTauTrackSelector.h"

class  ConeIsolationAlgorithm  
{
 public:
  // default constructor
  ConeIsolationAlgorithm();

  // "standard" constructor taking an edm::ParameterSet to initialize configuration parameters
  ConeIsolationAlgorithm(const edm::ParameterSet& configuration);

  // destructor
  ~ConeIsolationAlgorithm();

  std::pair<reco::JetTag, reco::IsolatedTauTagInfo> tag(const reco::JetTracksAssociationRef& jetTrackAssociation, 
							const reco::Vertex* primaryEventVertex); 

  void printConfiguration();
  void printEvent(const reco::Vertex* primaryEventVertex);
  void printJet(const reco::Jet& jet, const reco::JetTag& jetTag, const reco::IsolatedTauTagInfo& isolatedTauTagInfo);

 protected:
  // parse configuration parameters passed in edm::ParameterSet
  // and check validity
  void checkConfiguration();

  void findConfigurationStr();

  // check functional form 
  // given as configuration parameter for matching and signal cone sizes
  void checkFormula(TFormula& coneSizeFormula, const std::string& coneSizeStr, const char* errorMessage);

  // compute size of matching cone
  // (possibly depending on energy and/or transverse energy of the jet)
  double computeMatchingConeSize(const reco::Jet& jet, const TFormula& matchingConeSizeFormula, double matchingConeSizeMin, double matchingConeSizeMax) const;

  // compute size of signal cone
  // (possibly depending on energy and/or transverse energy of the jet)
  double computeSignalConeSize(const reco::Jet& jet, const TFormula& signalConeSizeFormula, double signalConeSizeMin, double signalConeSizeMax) const;

  // compute jet-tag discriminator
  //  value = 1 means jet is compatible with hadronic tau decay 
  //          0              fails tau identification critereon 
  //            (i.e. too many tracks selected in isolation ring)
  double computeDiscriminator(const reco::Jet& jet, const reco::IsolatedTauTagInfo& isolatedTauTagInfo, 
			      bool foundPrimaryEventVertex) const;

 private:
  //declare configuration parameters
  double leadingTrackMinPt_;
  //double otherTrackMinPt_;
  double trackMaxDzVertex_;
  bool useVertexConstraint_;

  std::string matchingConeSizeStr_;
  TFormula matchingConeSizeFormula_;
  double matchingConeSizeMin_;
  double matchingConeSizeMax_;
  std::string matchingConeTypeStr_;
  int matchingConeType_;

  std::string signalConeSizeStr_;
  TFormula signalConeSizeFormula_;
  double signalConeSizeMin_;
  double signalConeSizeMax_;
  std::string signalConeTypeStr_;
  int signalConeType_;

  double isolationConeSize_;
  std::string isolationConeTypeStr_;
  int isolationConeType_;

  unsigned int numTrackIsolationRingMax_;

  reco::BTauTrackSelector trackSelector_;

  static const unsigned int debugLevel_ = 1;
};

#endif 

