
/** \executable FWLiteZllRecoilCorrectionNtupleProducer
 *
 * Produce "plain" ROOT Ntuple for fitting Z-recoil correction parameters
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.7 $
 *
 * $Id: FWLiteZllRecoilCorrectionNtupleProducer.cc,v 1.7 2011/09/30 12:30:41 veelken Exp $
 *
 */

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/LuminosityBlock.h"
#include "DataFormats/FWLite/interface/Run.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TBenchmark.h>

#include <vector>
#include <string>

typedef std::vector<edm::InputTag> vInputTag;
typedef std::vector<std::string> vstring;

int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<FWLiteZllRecoilCorrectionNtupleProducer>:" << std::endl;

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("FWLiteZllRecoilCorrectionNtupleProducer");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("FWLiteZllRecoilCorrectionNtupleProducer") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgZllRecoilCorrectionNtupleProducer = cfg.getParameter<edm::ParameterSet>("ZllRecoilCorrectionNtupleProducer");

  edm::InputTag srcZllCandidates = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcZllCandidates");
  edm::InputTag srcMEt = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcMEt");
  edm::InputTag srcJets = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcJets");
  edm::InputTag srcUnclPFCands = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcUnclPFCands");

  edm::InputTag srcTrigger = cfgZllRecoilCorrectionAnalyzer.getParameter<edm::InputTag>("srcTrigger");
  vstring hltPaths = cfgZllRecoilCorrectionAnalyzer.getParameter<vstring>("hltPaths");

  vInputTag srcWeights = cfgZllRecoilCorrectionNtupleProducer.getParameter<vInputTag>("srcWeights");

  edm::InputTag srcVertices = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcVertices");
  edm::InputTag srcRhoNeutral = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcRhoNeutral");

  TFile* addPUreweightFile = 0;
  TH1* addPUreweightHistogram = 0;  
  double minPUreweight = 1.0;
  double maxPUreweight = 1.0;
  if ( cfgZllRecoilCorrectionNtupleProducer.exists("addPUreweight") ) {
    edm::ParameterSet cfgAddPUreweight = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::ParameterSet>("addPUreweight");
    edm::FileInPath addPUreweightFileName = cfgAddPUreweight.getParameter<edm::FileInPath>("inputFileName");
    std::string addPUreweightName = cfgAddPUreweight.getParameter<std::string>("meName");
    if ( addPUreweightFileName.relativePath() != "" && addPUreweightName != "" ) {
      if ( !addPUreweightFileName.isLocal() ) 
	throw cms::Exception("FWLiteZllRecoilCorrectionNtupleProducer") 
	  << " Failed to find File = " << addPUreweightFileName << " !!\n";
      addPUreweightFile = new TFile(addPUreweightFileName.fullPath().data());
      addPUreweightHistogram = dynamic_cast<TH1*>(addPUreweightFile->Get(addPUreweightName.data()));
      if ( !addPUreweightHistogram )
	throw cms::Exception("FWLiteZllRecoilCorrectionNtupleProducer") 
	  << "Failed to find histogram = " << addPUreweightName << " in file = " << addPUreweightFileName << " !!\n";
    }
    minPUreweight = cfgAddPUreweight.getParameter<double>("minPUreweight");
    maxPUreweight = cfgAddPUreweight.getParameter<double>("maxPUreweight");
  }

  std::string directory = cfgZllRecoilCorrectionNtupleProducer.getParameter<std::string>("directory");

  fwlite::InputSource inputFiles(cfg); 
  int maxEvents = inputFiles.maxEvents();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

//--- create "plain" ROOT Ntuple
  TFileDirectory dir = ( directory != "" ) ? fs.mkdir(directory) : fs;
  TTree* outputTree = dir.make<TTree>("ZllRecoilCorrectionNtuple", "ZllRecoilCorrectionNtuple");
  const int defaultBranchBufferSize = 64000;
  Double_t qT, uParl, uPerp;          // variables for "regular" Z-recoil corrections
  outputTree->Branch("qT",        &qT,        "qT/D",        defaultBranchBufferSize);
  outputTree->Branch("uParl",     &uParl,     "uParl/D",     defaultBranchBufferSize);
  outputTree->Branch("uPerp",     &uPerp,     "uPerp/D",     defaultBranchBufferSize);
  Double_t rT_ii, vParl_ii, vPerp_ii; // variables for determining type-II MEt corrections/calibrating "unclustered energy"
                                      // (PFJets of Pt < 10 GeV plus PFCandidates not in jets)
  outputTree->Branch("rT_ii",     &rT_ii,     "rT_ii/D",     defaultBranchBufferSize);
  outputTree->Branch("vParl_ii",  &vParl_ii,  "vParl_ii/D",  defaultBranchBufferSize);
  outputTree->Branch("vPerp_ii",  &vPerp_ii,  "vPerp_ii/D",  defaultBranchBufferSize);
  Double_t evtWeight; 
  outputTree->Branch("evtWeight", &evtWeight, "evtWeight/D", defaultBranchBufferSize);

  int    numEvents_processed         = 0; 
  double numEventsWeighted_processed = 0.;

  bool maxEvents_processed = false;
  for ( vstring::const_iterator inputFileName = inputFiles.files().begin();
	inputFileName != inputFiles.files().end() && !maxEvents_processed; ++inputFileName ) {

//--- open input file
    TFile* inputFile = TFile::Open(inputFileName->data());
    if ( !inputFile ) 
      throw cms::Exception("FWLiteTauFakeRateAnalyzer") 
	<< "Failed to open inputFile = " << (*inputFileName) << " !!\n";

    std::cout << "opening inputFile = " << (*inputFileName);
    TTree* inputTree = dynamic_cast<TTree*>(inputFile->Get("Events"));
    if ( inputTree ) std::cout << " (" << inputTree->GetEntries() << " Events)";
    std::cout << std::endl;

    fwlite::Event evt(inputFile);
    for ( evt.toBegin(); !(evt.atEnd() || maxEvents_processed); ++evt ) {

      //std::cout << "processing run = " << evt.id().run() << ":" 
      //	  << " ls = " << evt.luminosityBlock() << ", event = " << evt.id().event() << std::endl;

//--- quit event loop if maximal number of events to be processed is reached 
      ++numEvents_processed;
      numEventsWeighted_processed += evtWeight;
      if ( maxEvents > 0 && numEvents_processed >= maxEvents ) maxEvents_processed = true;      

//--- compute event weight
//   (reweighting for in-time and out-of-time pile-up, Data/MC correction factors,...)
      evtWeight = 1.0;
      for ( vInputTag::const_iterator srcWeight = srcWeights.begin();
	    srcWeight != srcWeights.end(); ++srcWeight ) {
	edm::Handle<double> weight;
	evt.getByLabel(*srcWeight, weight);
	evtWeight *= (*weight);
      }

      if ( srcRhoNeutral.label() != "" && srcVertices.label() != "" && addPUreweightHistogram ) {
	edm::Handle<reco::VertexCollection> vertices;
	evt.getByLabel(srcVertices, vertices);
	size_t vtxMultiplicity = vertices->size();
  
	edm::Handle<double> rhoNeutral_handle;
	evt.getByLabel(srcRhoNeutral, rhoNeutral_handle);
	double rhoNeutral = (*rhoNeutral_handle);

	int bin = addPUreweightHistogram->FindBin(vtxMultiplicity, rhoNeutral);
	double addPUreweight = addPUreweightHistogram->GetBinContent(bin);
	if ( addPUreweight < minPUreweight ) addPUreweight = minPUreweight;
	if ( addPUreweight > maxPUreweight ) addPUreweight = maxPUreweight;

	evtWeight *= addPUreweight;
      }

      bool isTriggered = false;
      if ( hltPaths.size() == 0 ) {
	isTriggered = true;
      } else {
	edm::Handle<edm::TriggerResults> hltResults;
	evt.getByLabel(srcTrigger_, hltResults);
  
	const edm::TriggerNames& triggerNames = evt.triggerNames(*hltResults);

	for ( vstring::const_iterator hltPath = hltPaths.begin();
	      hltPath != hltPaths.end(); ++hltPath ) {
	  bool isHLTpath_passed = false;
	  unsigned int idx = triggerNames.triggerIndex(*hltPath);
	  if ( idx < triggerNames.size() ) isHLTpath_passed = hltResults.accept(idx);
	}
      }

      if ( !isTriggered ) continue;

//--- find Z --> mu+ mu- candidate closest to nominal Z0 mass
      edm::Handle<reco::CompositeCandidateCollection> ZllCandidates;
      evt.getByLabel(srcZllCandidates, ZllCandidates);

      const reco::Candidate* bestZllCandidate = 0;
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

      edm::Handle<pat::METCollection> met;
      evt.getByLabel(srcMEt, met);
  
      if ( met->size() != 1 ) 
	throw cms::Exception("FWLiteTauFakeRateAnalyzer") 
	  << "Failed to find unique MET object !!\n";
  
      const pat::MET& theEventMEt = (*met->begin());

      // CV: fit Z-recoil correction parameters for events with MEt < 60 GeV only,
      //     as di-boson and TTbar backgrounds dominate in high MEt tail
      if ( theEventMEt.pt() > 60. ) continue;

      qT = bestZllCandidate->pt();

      int errorFlag = 0;
      std::pair<double, double> uT = compMEtProjU(bestZllCandidate->p4(), theEventMEt.px(), theEventMEt.py(), errorFlag);
      if ( errorFlag ) continue;
      
      uParl = uT.first;
      uPerp = uT.second;

      reco::Candidate::LorentzVector r_ii = bestZllCandidate->p4();
      reco::Candidate::LorentzVector v_ii;

      edm::Handle<pat::JetCollection> calibJets;
      evt.getByLabel(srcJets, calibJets);
      for ( pat::JetCollection::const_iterator calibJet = calibJets->begin();
	    calibJet != calibJets->end(); ++calibJet ) {
	if ( calibJet->pt() > 10. ) r_ii += calibJet->p4();
	else                        v_ii += calibJet->p4();
      }

      edm::Handle<reco::PFCandidateCollection> unclPFCands;
      evt.getByLabel(srcUnclPFCands, unclPFCands);
      for ( reco::PFCandidateCollection::const_iterator unclPFCand = unclPFCands->begin();
	    unclPFCand != unclPFCands->end(); ++unclPFCand ) {
	assert(bestZllCandidate->numberOfDaughters() == 2);
	//if ( reco::deltaR(unclPFCand->p4(), bestZllCandidate->daughter(0)->p4()) < 0.3 ||
	//     reco::deltaR(unclPFCand->p4(), bestZllCandidate->daughter(1)->p4()) < 0.3 ) {
	//  std::cout << "Warning: unclustered PFCandidate close to Muon, pt = " << unclPFCand->pt() << std::endl;
	//}
	v_ii += unclPFCand->p4();
      }

      int errorFlag_ii = 0;
      std::pair<double, double> vT_ii = compMEtProjU(r_ii, -(r_ii.px() + v_ii.px()), -(r_ii.py() + v_ii.py()), errorFlag_ii);
      if ( errorFlag_ii ) continue;
      
      vParl_ii = vT_ii.first;
      vPerp_ii = vT_ii.second;

      outputTree->Fill();
    }

//--- close input file
    delete inputFile;
  }

  delete addPUreweightFile;

  std::cout << "<FWLiteZllRecoilCorrectionNtupleProducer>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed 
	    << " (weighted = " << numEventsWeighted_processed << ")" << std::endl;

  clock.Show("FWLiteZllRecoilCorrectionNtupleProducer");

  return 0;
}
