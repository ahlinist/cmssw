#ifndef BTagPerformanceAnalyzer_H
#define BTagPerformanceAnalyzer_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoBTag/Analysis/interface/BTagDifferentialPlot.h"
#include "RecoBTag/Analysis/interface/AcceptJet.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "RecoBTag/Analysis/interface/BaseTagInfoPlotter.h"
#include "RecoBTag/Analysis/interface/Tools.h"
#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"

#include <string>
#include <vector>

//class CaloJetRef;

/** \class BTagPerformanceAnalyzer
 *
 *  Top level steering routine for b tag performance analysis.
 *
 */

class BTagPerformanceAnalyzer : public edm::EDAnalyzer {
   public:
      explicit BTagPerformanceAnalyzer(const edm::ParameterSet& pSet);

      ~BTagPerformanceAnalyzer();

      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

  virtual void endJob();

   private:

  // Get histogram plotting options from configuration.
  void init(const edm::ParameterSet& iConfig);
  void bookHistos(const edm::ParameterSet& pSet);
  EtaPtBin getEtaPtBin(int iEta, int iPt);
  typedef std::map<reco::CaloJetRef, unsigned int> FlavourMap;
  JetFlavour getJetFlavour(
	edm::RefToBase<reco::Jet> caloRefTB, bool fastMC, FlavourMap flavours);

  std::string rootFile;
  vector<edm::InputTag> jetTagModuleLabel;
  vector<std::string> dataFormatType, tiDataFormatType;
  bool partonKinematics;
  double etaMin, etaMax;
  double ptRecJetMin, ptRecJetMax;
  double ptPartonMin, ptPartonMax;
  AcceptJet jetSelector;   // Decides if jet and parton satisfy kinematic cuts.
  std::vector<double> etaRanges, ptRanges;
  double discriminatorStart, discriminatorEnd, effBConst;
  int   nBinEffPur;
  double startEffPur, endEffPur;
  bool produceEps, producePs;
  TString psBaseName, epsBaseName, inputFile;
  bool update;
  bool fastMC;
  edm::InputTag jetMCSrc;

  vector< vector<JetTagPlotter*> > binJetTagPlotters;
  vector< vector<BaseTagInfoPlotter*> > binTagInfoPlotters;
  vector<edm::InputTag> jetTagInputTags, tagInfoInputTags;
  // Contains plots for each bin of rapidity and pt.
  vector< vector<BTagDifferentialPlot*> > differentialPlots;
  JetFlavourIdentifier jfi;
};


#endif
