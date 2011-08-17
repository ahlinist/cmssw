
/** \executable FWLiteZllRecoilCorrectionNtupleProducer
 *
 * Produce "plain" ROOT Ntuple for fitting Z-recoil correction parameters
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: FWLiteZllRecoilCorrectionNtupleProducer.cc,v 1.2 2011/08/15 17:10:31 veelken Exp $
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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"

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

  vInputTag srcWeights = cfgZllRecoilCorrectionNtupleProducer.getParameter<vInputTag>("srcWeights");

  edm::InputTag srcVertices = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcVertices");
  edm::InputTag srcPFNeutralRho = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::InputTag>("srcPFNeutralRho");

  edm::ParameterSet cfgAddPUreweight = cfgZllRecoilCorrectionNtupleProducer.getParameter<edm::ParameterSet>("addPUreweight");
  edm::FileInPath addPUreweightFileName = cfgAddPUreweight.getParameter<edm::FileInPath>("inputFileName");
  std::string addPUreweightName = cfgAddPUreweight.getParameter<std::string>("meName");
  TH1* addPUreweightHistogram = 0;
  if ( addPUreweightFileName != "" && addPUreweightName != "" ) {
    if ( !addPUreweightFileName.isLocal() ) 
      throw cms::Exception("VertexMultiplicityReweightExtractor") 
	<< " Failed to find File = " << addPUreweightFileName << " !!\n";
    TFile* addPUreweightFile = new TFile(addPUreweightFileName.fullPath().data());
    TH1* addPUreweightHistogram_object = dynamic_cast<TH1*>(addPUreweightFile->Get(addPUreweightName.data()));
    if ( !addPUreweightHistogram_object )
      throw cms::Exception("FWLiteZllRecoilCorrectionNtupleProducer") 
	<< "Failed to find histogram = " << addPUreweightName << " in file = " << addPUreweightFileName << " !!\n";
    std::string addPUreweightHistogramName = std::string(addPUreweightHistogram_object->GetName()).append("_cloned");
    addPUreweightHistogram = (TH1*)addPUreweightHistogram_object->Clone(addPUreweightHistogramName.data());
    delete addPUreweightFile;
  }
  double minPUreweight = cfgAddPUreweight.getParameter<double>("minPUreweight");
  double maxPUreweight = cfgAddPUreweight.getParameter<double>("maxPUreweight");

  std::string directory = cfgZllRecoilCorrectionNtupleProducer.getParameter<std::string>("directory");

  fwlite::InputSource inputFiles(cfg); 
  int maxEvents = inputFiles.maxEvents();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

//--- create "plain" ROOT Ntuple
  TFileDirectory dir = ( directory != "" ) ? fs.mkdir(directory) : fs;
  TTree* tree = dir.make<TTree>("ZllRecoilCorrectionNtuple", "ZllRecoilCorrectionNtuple");
  const int defaultBranchBufferSize = 64000;
  Float_t qT, u1, u2, evtWeight;  
  tree->Branch("qT",        &qT,        "qT/F",        defaultBranchBufferSize);
  tree->Branch("u1",        &u1,        "u1/F",        defaultBranchBufferSize);
  tree->Branch("u2",        &u2,        "u2/F",        defaultBranchBufferSize);
  tree->Branch("evtWeight", &evtWeight, "evtWeight/F", defaultBranchBufferSize);

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
    TTree* tree = dynamic_cast<TTree*>(inputFile->Get("Events"));
    if ( tree ) std::cout << " (" << tree->GetEntries() << " Events)";
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
      double evtWeight = 1.0;
      for ( vInputTag::const_iterator srcWeight = srcWeights.begin();
	    srcWeight != srcWeights.end(); ++srcWeight ) {
	edm::Handle<double> weight;
	evt.getByLabel(*srcWeight, weight);
	evtWeight *= (*weight);
      }

      if ( srcPFNeutralRho.label() != "" && srcVertices.label() != "" && addPUreweightHistogram ) {
	edm::Handle<reco::VertexCollection> vertices;
	evt.getByLabel(srcVertices, vertices);
	size_t vtxMultiplicity = vertices->size();
  
	edm::Handle<double> pfNeutralRho_handle;
	evt.getByLabel(srcPFNeutralRho, pfNeutralRho_handle);
	double pfNeutralRho = (*pfNeutralRho_handle);

	int bin = addPUreweightHistogram->FindBin(vtxMultiplicity, pfNeutralRho);
	double addPUreweight = addPUreweightHistogram->GetBinContent(bin);
	if ( addPUreweight < minPUreweight ) addPUreweight = minPUreweight;
	if ( addPUreweight > maxPUreweight ) addPUreweight = maxPUreweight;

	evtWeight *= addPUreweight;
      }

//--- find Z --> mu+ mu- candidate closest to nominal Z0 mass
      edm::Handle<reco::CandidateView> ZllCandidates;
      evt.getByLabel(srcZllCandidates, ZllCandidates);

      const reco::Candidate* bestZllCandidate = 0;
      const double nominalZmass = 91.19;
      double minMassDiff = -1.;
      for ( reco::CandidateView::const_iterator ZllCandidate = ZllCandidates->begin();
	    ZllCandidate != ZllCandidates->end(); ++ZllCandidate ) {
	double massDiff = TMath::Abs(ZllCandidate->mass() - nominalZmass);
	if ( minMassDiff == -1. || massDiff < minMassDiff ) {
	  bestZllCandidate = &(*ZllCandidate);
	  minMassDiff = massDiff;
	}
      }
	    
      if ( !bestZllCandidate ) continue;

      edm::Handle<std::vector<pat::MET> > met;
      evt.getByLabel(srcMEt, met);
  
      if ( met->size() != 1 ) 
	throw cms::Exception("FWLiteTauFakeRateAnalyzer") 
	  << "Failed to find unique MET object !!\n";
  
      const pat::MET& theEventMEt = (*met->begin());

      qT = bestZllCandidate->pt();

      int errorFlag = 0;
      std::pair<double, double> uT = compMEtProjU(bestZllCandidate->p4(), theEventMEt.px(), theEventMEt.py(), errorFlag);
      if ( errorFlag ) continue;
      
      u1 = uT.first;
      u2 = uT.second;

      tree->Fill();
    }

//--- close input file
    delete inputFile;
  }

  std::cout << "<FWLiteZllRecoilCorrectionNtupleProducer>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed 
	    << " (weighted = " << numEventsWeighted_processed << ")" << std::endl;

  clock.Show("FWLiteZllRecoilCorrectionNtupleProducer");

  return 0;
}
