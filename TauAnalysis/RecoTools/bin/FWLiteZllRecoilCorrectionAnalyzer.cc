
/** \executable FWLiteZllRecoilCorrectionAnalyzer
 *
 * Fill Z-recoil correction control plots
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.14 $
 *
 * $Id: FWLiteZllRecoilCorrectionAnalyzer.cc,v 1.14 2012/04/16 07:26:50 veelken Exp $
 *
 */

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/LuminosityBlock.h"
#include "DataFormats/FWLite/interface/Run.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "DataFormats/Common/interface/Handle.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionHistManager.h"
#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionAlgorithm.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TBenchmark.h>
#include <TMath.h>
#include <TFormula.h>

#include <vector>
#include <string>
#include <fstream>

typedef std::vector<edm::InputTag> vInputTag;
typedef std::vector<std::string> vstring;

double square(double x)
{
  return x*x;
}

int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<FWLiteZllRecoilCorrectionAnalyzer>:" << std::endl;

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("FWLiteZllRecoilCorrectionAnalyzer");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("FWLiteZllRecoilCorrectionAnalyzer") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgZllRecoilCorrectionAnalyzer = cfg.getParameter<edm::ParameterSet>("ZllRecoilCorrectionAnalyzer");

  edm::InputTag srcZllCandidates = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcZllCandidates");
  edm::InputTag srcMuons = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcMuons");

  edm::InputTag srcJets = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcJets");
  edm::InputTag srcMEt = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcMEt");
  edm::InputTag srcMEtSignCovMatrix = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcMEtSignCovMatrix");
  edm::InputTag srcPFCandidates = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcPFCandidates");

  std::string shiftedMEtCorrX_string = cfgZllRecoilCorrectionAnalyzer.getParameter<std::string>("shiftedMEtCorrX");
  TFormula* shiftedMEtCorrX = new TFormula("shiftedMEtCorrX", shiftedMEtCorrX_string.data());
  std::string shiftedMEtCorrY_string = cfgZllRecoilCorrectionAnalyzer.getParameter<std::string>("shiftedMEtCorrY");
  TFormula* shiftedMEtCorrY = new TFormula("shiftedMEtCorrY", shiftedMEtCorrY_string.data());
  bool applyMEtShiftCorr = cfgZllRecoilCorrectionAnalyzer.getParameter<bool>("applyMEtShiftCorr");

  edm::InputTag srcTrigger = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcTrigger");
  vstring hltPaths = cfgZllRecoilCorrectionAnalyzer.getParameter<vstring>("hltPaths");

  vInputTag srcWeights = cfgZllRecoilCorrectionAnalyzer.getParameter<vInputTag>("srcWeights");

  edm::InputTag srcGenPileUpInfo = cfgZllRecoilCorrectionAnalyzer.exists("srcGenPileUpInfo") ?
    cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcGenPileUpInfo") : edm::InputTag();

  edm::InputTag srcVertices = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcVertices");
  edm::InputTag srcRhoNeutral = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcRhoNeutral");

  edm::InputTag srcEventCounter = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcEventCounter");

  TFile* addPUreweightFile = 0;
  TH1* addPUreweightHistogram = 0;  
  double minPUreweight = 1.0;
  double maxPUreweight = 1.0;
  if ( cfgZllRecoilCorrectionAnalyzer.exists("addPUreweight") ) {
    edm::ParameterSet cfgAddPUreweight = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::ParameterSet>("addPUreweight");
    edm::FileInPath addPUreweightFileName = cfgAddPUreweight.getParameter<edm::FileInPath>("inputFileName");
    std::string addPUreweightName = cfgAddPUreweight.getParameter<std::string>("meName");
    if ( addPUreweightFileName.relativePath() != "" && addPUreweightName != "" ) {
      if ( !addPUreweightFileName.isLocal() ) 
	throw cms::Exception("FWLiteZllRecoilCorrectionAnalyzer") 
	  << " Failed to find File = " << addPUreweightFileName << " !!\n";
      addPUreweightFile = new TFile(addPUreweightFileName.fullPath().data());
      addPUreweightHistogram = dynamic_cast<TH1*>(addPUreweightFile->Get(addPUreweightName.data()));
      if ( !addPUreweightHistogram )
	throw cms::Exception("FWLiteZllRecoilCorrectionAnalyzer") 
	  << "Failed to find histogram = " << addPUreweightName << " in file = " << addPUreweightFileName << " !!\n";
    }
    minPUreweight = cfgAddPUreweight.getParameter<double>("minPUreweight");
    maxPUreweight = cfgAddPUreweight.getParameter<double>("maxPUreweight");
  }

  std::string directory = cfgZllRecoilCorrectionAnalyzer.getParameter<std::string>("directory");

  std::string selEventsFileName = ( cfgZllRecoilCorrectionAnalyzer.exists("selEventsFileName") ) ? 
    cfgZllRecoilCorrectionAnalyzer.getParameter<std::string>("selEventsFileName") : "";

  fwlite::InputSource inputFiles(cfg); 
  int maxEvents = inputFiles.maxEvents();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  std::string processType = cfgZllRecoilCorrectionAnalyzer.getParameter<std::string>("type");
  std::cout << " type = " << processType << std::endl;
  bool isData = (processType == "Data");
  bool isMC_background = (processType == "MC_background");
  bool isMC_signal = !(isData || isMC_background);

  ZllRecoilCorrectionAlgorithm* corrAlgorithm = 0;
  if ( isMC_signal && cfgZllRecoilCorrectionAnalyzer.exists("algorithm") ) {
    edm::ParameterSet cfgZllRecoilCorrectionAlgorithm = 
      cfgZllRecoilCorrectionAnalyzer.getParameter<edm::ParameterSet>("algorithm");
    corrAlgorithm = new ZllRecoilCorrectionAlgorithm(cfgZllRecoilCorrectionAlgorithm);
  }
  
  ZllRecoilCorrectionParameterSet* ZllRecoilCorrParameter_data = 0;
  if ( cfgZllRecoilCorrectionAnalyzer.exists("algorithm") ) {
    edm::ParameterSet cfgZllRecoilCorrectionAlgorithm = 
      cfgZllRecoilCorrectionAnalyzer.getParameter<edm::ParameterSet>("algorithm");
    edm::ParameterSet cfgZllRecoilCorrParameters = cfgZllRecoilCorrectionAlgorithm.getParameter<edm::ParameterSet>("parameter");
    edm::ParameterSet cfgZllRecoilCorrParameters_data = cfgZllRecoilCorrParameters.getParameter<edm::ParameterSet>("data");
    ZllRecoilCorrParameter_data = new ZllRecoilCorrectionParameterSet(cfgZllRecoilCorrParameters_data);
  }
  
//--- book "dummy" histogram counting number of processed events
  TH1* histogramEventCounter = fs.make<TH1F>("numEventsProcessed", "Number of processed Events", 3, -0.5, +2.5);
  histogramEventCounter->GetXaxis()->SetBinLabel(1, "all Events (DBS)");      // CV: bin numbers start at 1 (not 0) !!
  histogramEventCounter->GetXaxis()->SetBinLabel(2, "processed by Skimming");
  histogramEventCounter->GetXaxis()->SetBinLabel(3, "analyzed in PAT-tuple");
  
  int allEvents_DBS = cfgZllRecoilCorrectionAnalyzer.getParameter<int>("allEvents_DBS");
  if ( allEvents_DBS > 0 ) {
    histogramEventCounter->SetBinContent(1, allEvents_DBS);
  } else {
    histogramEventCounter->SetBinContent(1, -1.);
  }
  
  double xSection = cfgZllRecoilCorrectionAnalyzer.getParameter<double>("xSection");
  double intLumiData = cfgZllRecoilCorrectionAnalyzer.getParameter<double>("intLumiData");

//--- create control plots
  TFileDirectory dir = ( directory != "" ) ? fs.mkdir(directory) : fs;
  edm::ParameterSet cfgZllRecoilCorrectionHistManager;
  ZllRecoilCorrectionHistManager* histogramsBeforeGenPUreweight 
    = new ZllRecoilCorrectionHistManager(cfgZllRecoilCorrectionHistManager);
  TFileDirectory subdirBeforeGenPUreweight = dir.mkdir("beforeGenPUreweight");
  histogramsBeforeGenPUreweight->bookHistograms(subdirBeforeGenPUreweight);
  ZllRecoilCorrectionHistManager* histogramsBeforeAddPUreweight 
    = new ZllRecoilCorrectionHistManager(cfgZllRecoilCorrectionHistManager);
  TFileDirectory subdirBeforeAddPUreweight = dir.mkdir("beforeAddPUreweight");
  histogramsBeforeAddPUreweight->bookHistograms(subdirBeforeAddPUreweight);
  ZllRecoilCorrectionHistManager* histogramsBeforeZllRecoilCorr 
    = new ZllRecoilCorrectionHistManager(cfgZllRecoilCorrectionHistManager);
  TFileDirectory subdirBeforeZllRecoilCorr = dir.mkdir("beforeZllRecoilCorr");
  histogramsBeforeZllRecoilCorr->bookHistograms(subdirBeforeZllRecoilCorr);
  ZllRecoilCorrectionHistManager* histogramsAfterZllRecoilMCtoDataCorr  
    = new ZllRecoilCorrectionHistManager(cfgZllRecoilCorrectionHistManager);
  TFileDirectory subdirAfterZllRecoilMCtoDataCorr = dir.mkdir("afterZllRecoilMCtoDataCorr");
  histogramsAfterZllRecoilMCtoDataCorr->bookHistograms(subdirAfterZllRecoilMCtoDataCorr);
  ZllRecoilCorrectionHistManager* histogramsAfterZllRecoilAbsCalib
    = new ZllRecoilCorrectionHistManager(cfgZllRecoilCorrectionHistManager);
  TFileDirectory subdirAfterZllRecoilAbsCalib  = dir.mkdir("afterZllRecoilAbsCalib");
  histogramsAfterZllRecoilAbsCalib->bookHistograms(subdirAfterZllRecoilAbsCalib);
  
  std::ofstream* selEventsFile = ( selEventsFileName != "" ) ?
    new std::ofstream(selEventsFileName.data(), std::ios::out) : 0;

  int numEvents_processed = 0; 

  edm::RunNumber_t lastLumiBlock_run = -1;
  edm::LuminosityBlockNumber_t lastLumiBlock_ls = -1;

  bool maxEvents_processed = false;
  for ( vstring::const_iterator inputFileName = inputFiles.files().begin();
	inputFileName != inputFiles.files().end() && !maxEvents_processed; ++inputFileName ) {

//--- open input file
    TFile* inputFile = TFile::Open(inputFileName->data());
    if ( !inputFile ) 
      throw cms::Exception("FWLiteZllRecoilCorrectionAnalyzer") 
	<< "Failed to open inputFile = " << (*inputFileName) << " !!\n";

    std::cout << "opening inputFile = " << (*inputFileName);
    TTree* tree = dynamic_cast<TTree*>(inputFile->Get("Events"));
    if ( tree ) std::cout << " (" << tree->GetEntries() << " Events)";
    std::cout << std::endl;

    fwlite::Event evt(inputFile);
    for ( evt.toBegin(); !(evt.atEnd() || maxEvents_processed); ++evt ) {

//--- quit event loop if maximal number of events to be processed is reached 
      ++numEvents_processed;
      if ( maxEvents > 0 && numEvents_processed >= maxEvents ) maxEvents_processed = true;

      //std::cout << "processing run = " << evt.id().run() << ":" 
      //	  << " ls = " << evt.luminosityBlock() << ", event = " << evt.id().event() << std::endl;

//--- check if new luminosity section has started;
//    if so, retrieve number of events contained in this luminosity section before skimming
      if ( !(evt.id().run() == lastLumiBlock_run && evt.luminosityBlock() == lastLumiBlock_ls) ) {
	const fwlite::LuminosityBlock& ls = evt.getLuminosityBlock();
	edm::Handle<edm::MergeableCounter> numEvents_skimmed;
	ls.getByLabel(srcEventCounter, numEvents_skimmed);
	if ( numEvents_skimmed.isValid() ) histogramEventCounter->Fill(1, numEvents_skimmed->value);
	lastLumiBlock_run = evt.id().run();
	lastLumiBlock_ls = evt.luminosityBlock();
      }
 
//--- fill "dummy" histogram counting number of processed events
      histogramEventCounter->Fill(2);

//--- compute event weight
//   (reweighting for in-time and out-of-time pile-up, Data/MC correction factors,...)
      double genPUreweight = 1.0;
      for ( vInputTag::const_iterator srcWeight = srcWeights.begin();
	    srcWeight != srcWeights.end(); ++srcWeight ) {
	edm::Handle<double> weight;
	evt.getByLabel(*srcWeight, weight);
	genPUreweight *= (*weight);
      }

      bool isTriggered = false;
      if ( hltPaths.size() == 0 ) {
	isTriggered = true;
      } else {
	edm::Handle<edm::TriggerResults> hltResults;
	evt.getByLabel(srcTrigger, hltResults);
  
	const edm::TriggerNames& triggerNames = evt.triggerNames(*hltResults);

	for ( vstring::const_iterator hltPath = hltPaths.begin();
	      hltPath != hltPaths.end(); ++hltPath ) {
	  bool isHLTpath_passed = false;
	  unsigned int idx = triggerNames.triggerIndex(*hltPath);
	  if ( idx < triggerNames.size() ) isHLTpath_passed = hltResults->accept(idx);
	  if ( isHLTpath_passed ) isTriggered = true;
	}
      }

      if ( !isTriggered ) continue;

      int numPU_bxMinus1 = -1;
      int numPU_bx0      = -1;
      int numPU_bxPlus1  = -1;
      if ( srcGenPileUpInfo.label() != "" ) {
	typedef std::vector<PileupSummaryInfo> PileupSummaryInfoCollection;
	edm::Handle<PileupSummaryInfoCollection> genPileUpInfos;
	evt.getByLabel(srcGenPileUpInfo, genPileUpInfos);

	for ( PileupSummaryInfoCollection::const_iterator genPileUpInfo = genPileUpInfos->begin();
	      genPileUpInfo != genPileUpInfos->end(); ++genPileUpInfo ) {
	  // CV: in-time PU is stored in getBunchCrossing = 0, 
	  //    cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupInformation
	  int bx = genPileUpInfo->getBunchCrossing();
	  if      ( bx == -1 ) numPU_bxMinus1 = genPileUpInfo->getPU_NumInteractions();
	  else if ( bx ==  0 ) numPU_bx0      = genPileUpInfo->getPU_NumInteractions();
	  else if ( bx == +1 ) numPU_bxPlus1  = genPileUpInfo->getPU_NumInteractions();
	}
      }

      edm::Handle<reco::VertexCollection> vertices;
      evt.getByLabel(srcVertices, vertices);
      size_t vtxMultiplicity = vertices->size();
  
      edm::Handle<double> rhoNeutral_handle;
      evt.getByLabel(srcRhoNeutral, rhoNeutral_handle);
      double rhoNeutral = (*rhoNeutral_handle);

      double addPUreweight = 1.0;
      if ( addPUreweightHistogram ) {
	int bin = addPUreweightHistogram->FindBin(vtxMultiplicity, rhoNeutral);
	addPUreweight = addPUreweightHistogram->GetBinContent(bin);
	//std::cout << "vtxMultiplicity = " << vtxMultiplicity << ", pfNeutralRho = " << pfNeutralRho << ":"
	//	    << " addPUreweight = " << addPUreweight << std::endl;
	if ( addPUreweight < minPUreweight ) addPUreweight = minPUreweight;
	if ( addPUreweight > maxPUreweight ) addPUreweight = maxPUreweight;
      }

//--- find Z --> mu+ mu- candidate closest to nominal Z0 mass
      edm::Handle<reco::CompositeCandidateCollection> ZllCandidates;
      evt.getByLabel(srcZllCandidates, ZllCandidates);

      const reco::CompositeCandidate* bestZllCandidate = 0;
      const double nominalZmass = 91.19;
      double minMassDiff = -1.;
      for ( reco::CompositeCandidateCollection::const_iterator ZllCandidate = ZllCandidates->begin();
	    ZllCandidate != ZllCandidates->end(); ++ZllCandidate ) {
	double massDiff = TMath::Abs(ZllCandidate->mass() - nominalZmass);
	if ( bestZllCandidate == 0 || massDiff < minMassDiff ) {
	  bestZllCandidate = &(*ZllCandidate);
	  minMassDiff = massDiff;
	}
      }
	    
      if ( !bestZllCandidate ) continue;

      edm::Handle<pat::MuonCollection> muons;
      evt.getByLabel(srcMuons, muons);
      
      edm::Handle<pat::JetCollection> jets;
      evt.getByLabel(srcJets, jets);

      edm::Handle<pat::METCollection> met;
      evt.getByLabel(srcMEt, met);
      if ( met->size() != 1 ) 
	throw cms::Exception("FWLiteZllRecoilCorrectionAnalyzer") 
	  << "Failed to find unique MET object !!\n";

      edm::Handle<PFMEtSignCovMatrix> metCov;
      evt.getByLabel(srcMEtSignCovMatrix, metCov);
  
      pat::MET rawMEt = (*met->begin());
      if ( applyMEtShiftCorr ) {
	double sumEt = rawMEt.sumEt();
	double rawMEtPx_sysShiftCorrected = rawMEt.px() - shiftedMEtCorrX->Eval(sumEt, vtxMultiplicity);
	double rawMEtPy_sysShiftCorrected = rawMEt.py() - shiftedMEtCorrY->Eval(sumEt, vtxMultiplicity);
	double rawMEtPt_sysShiftCorrected = TMath::Sqrt(square(rawMEtPx_sysShiftCorrected) + square(rawMEtPy_sysShiftCorrected));
	reco::Candidate::LorentzVector rawMEtP4_sysShiftCorrected(rawMEtPx_sysShiftCorrected, 
								  rawMEtPy_sysShiftCorrected, 
								  0., 
								  rawMEtPt_sysShiftCorrected);
	rawMEt.setP4(rawMEtP4_sysShiftCorrected);
      }

      //edm::Handle<reco::PFCandidateCollection> pfCandidates;
      //evt.getByLabel(srcPFCandidates, pfCandidates);
      
      reco::Candidate::LorentzVector p4PFChargedHadrons, p4PFNeutralHadrons, p4PFGammas;
      //for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates->begin();
      //      pfCandidate != pfCandidates->end(); ++pfCandidate ) {
      //  int pfCandidateType = pfCandidate->particleId();
      //  if      ( pfCandidateType == reco::PFCandidate::h     ) p4PFChargedHadrons += pfCandidate->p4();
      //  else if ( pfCandidateType == reco::PFCandidate::h0    ) p4PFNeutralHadrons += pfCandidate->p4();
      //  else if ( pfCandidateType == reco::PFCandidate::gamma ) p4PFGammas         += pfCandidate->p4();
      //}
      
      histogramsBeforeGenPUreweight->fillHistograms(
	*bestZllCandidate, *muons, *jets, rawMEt, *metCov, 
	p4PFChargedHadrons, p4PFNeutralHadrons, p4PFGammas, 
	numPU_bxMinus1, numPU_bx0, numPU_bxPlus1, *vertices, rhoNeutral, 1.0);
      histogramsBeforeAddPUreweight->fillHistograms(
        *bestZllCandidate, *muons, *jets, rawMEt, *metCov,
	p4PFChargedHadrons, p4PFNeutralHadrons, p4PFGammas, 
	numPU_bxMinus1, numPU_bx0, numPU_bxPlus1, *vertices, rhoNeutral, genPUreweight);
      histogramsBeforeZllRecoilCorr->fillHistograms(
	*bestZllCandidate, *muons, *jets, rawMEt, *metCov,
	p4PFChargedHadrons, p4PFNeutralHadrons, p4PFGammas, 
	numPU_bxMinus1, numPU_bx0, numPU_bxPlus1, *vertices, rhoNeutral, genPUreweight*addPUreweight);

      if ( selEventsFile ) (*selEventsFile) << evt.id().run() << ":" << evt.luminosityBlock() << ":" << evt.id().event() << std::endl;
      
      pat::MET mcToDataCorrMEt(rawMEt);
      if ( isMC_signal && corrAlgorithm ) {
	if ( !rawMEt.genMET() )
	  throw cms::Exception("FWLiteZllRecoilCorrectionAnalyzer") 
	    << "Failed to find generator level MET object !!\n";

	mcToDataCorrMEt = corrAlgorithm->buildZllCorrectedMEt(rawMEt, rawMEt.genMET()->p4(), bestZllCandidate->p4());
      }
      histogramsAfterZllRecoilMCtoDataCorr->fillHistograms(
        *bestZllCandidate, *muons, *jets, mcToDataCorrMEt, *metCov,
	p4PFChargedHadrons, p4PFNeutralHadrons, p4PFGammas, 
	numPU_bxMinus1, numPU_bx0, numPU_bxPlus1, *vertices, rhoNeutral, genPUreweight*addPUreweight);

      pat::MET absCalibMEt(rawMEt);
      if ( ZllRecoilCorrParameter_data ) {
	double qT = bestZllCandidate->pt();
	double k = ZllRecoilCorrParameter_data->k1()
                  *0.5*(1.0 - TMath::Erf(-ZllRecoilCorrParameter_data->k2()*TMath::Power(qT, ZllRecoilCorrParameter_data->k3())));
	double dHadRecoilPx = ((1. + k)/k)*(mcToDataCorrMEt.px() + bestZllCandidate->px());
	double dHadRecoilPy = ((1. + k)/k)*(mcToDataCorrMEt.py() + bestZllCandidate->py());
	double absCalibMEtPx = mcToDataCorrMEt.px() - dHadRecoilPx;
	double absCalibMEtPy = mcToDataCorrMEt.py() - dHadRecoilPy;
	double absCalibMEtPt = TMath::Sqrt(absCalibMEtPx*absCalibMEtPx + absCalibMEtPy*absCalibMEtPy);
	absCalibMEt.setP4(math::XYZTLorentzVector(absCalibMEtPx, absCalibMEtPy, 0., absCalibMEtPt));
      }
      histogramsAfterZllRecoilAbsCalib->fillHistograms(
        *bestZllCandidate, *muons, *jets, absCalibMEt, *metCov, 
	p4PFChargedHadrons, p4PFNeutralHadrons, p4PFGammas, 
	numPU_bxMinus1, numPU_bx0, numPU_bxPlus1, *vertices, rhoNeutral, genPUreweight*addPUreweight);
    }

//--- close input file
    delete inputFile;
  }

  delete shiftedMEtCorrX;
  delete shiftedMEtCorrY;
  delete addPUreweightFile;
  delete corrAlgorithm;
  delete ZllRecoilCorrParameter_data;

//--- close ASCII file containing 
//     run:lumi-section:event 
//    numbers of events with MET > 40 GeV
  delete selEventsFile;

  std::cout << "<FWLiteZllRecoilCorrectionAnalyzer>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed << std::endl;

//--- scale histograms taken from Monte Carlo simulation
//    according to cross-section times luminosity
  if ( !isData ) {
    double mcScaleFactor = (intLumiData*xSection)/(double)allEvents_DBS;
    std::cout << " intLumiData = " << intLumiData << std::endl;
    std::cout << " xSection = " << xSection << std::endl;
    std::cout << " allEvents_DBS = " << allEvents_DBS << std::endl;
    std::cout << "--> scaling histograms by factor = " << mcScaleFactor
	      << " according to cross-section times luminosity." << std::endl;

//--- apply correction to scale-factor in order to account for events lost, 
//    due to aborted skimming/crab or PAT-tuple production/lxbatch jobs
    double lostStatCorrFactor = 1.;
    if ( histogramEventCounter->GetBinContent(1) > histogramEventCounter->GetBinContent(2) && 
	 histogramEventCounter->GetBinContent(2) > 0.                                      ) {
      lostStatCorrFactor = histogramEventCounter->GetBinContent(1)/histogramEventCounter->GetBinContent(2);
      std::cout << "--> scaling histograms by additional factor = " << lostStatCorrFactor
		<< " to account for events lost," << std::endl; 
      std::cout << "    due to aborted skimming/crab or PAT-tuple production/lxbatch jobs." << std::endl;
    }

    histogramsBeforeGenPUreweight->scaleHistograms(mcScaleFactor*lostStatCorrFactor);
    histogramsBeforeAddPUreweight->scaleHistograms(mcScaleFactor*lostStatCorrFactor);
    histogramsBeforeZllRecoilCorr->scaleHistograms(mcScaleFactor*lostStatCorrFactor);
    histogramsAfterZllRecoilMCtoDataCorr->scaleHistograms(mcScaleFactor*lostStatCorrFactor);
    histogramsAfterZllRecoilAbsCalib->scaleHistograms(mcScaleFactor*lostStatCorrFactor);
  }

  clock.Show("FWLiteZllRecoilCorrectionAnalyzer");

  return 0;
}
