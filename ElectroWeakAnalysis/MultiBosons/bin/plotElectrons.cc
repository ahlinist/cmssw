#include "CommonTools/Utils/interface/ExpressionHisto.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/Histogrammer.h"

#include "Math/GenVector/PxPyPzM4D.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"

using namespace std;

int main ( int argc, char ** argv )
{

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }
  PythonProcessDesc builder(argv[1]);
  boost::shared_ptr<edm::ParameterSet> cfg = builder.processDesc()->getProcessPSet();
  edm::ParameterSet const& inputs =  cfg->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet const& outputs = cfg->getParameter<edm::ParameterSet>("outputs");
//   edm::ParameterSet const& ptHisto = cfg->getParameter<edm::ParameterSet>("ptHisto");
  edm::ParameterSet const& electronHistosCfg = cfg->getParameter<edm::ParameterSet>("electronHistos");
  edm::InputTag electronSrcTag = electronHistosCfg.getParameter<edm::InputTag>("src");

  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
//   TFileDirectory theDir = fs.mkdir( "histos" );
  TFileDirectory electronHistosDir = fs.mkdir( "electronHistos" );

//   TH1F* electronPt_  = theDir.make<TH1F>("electronPt", "pt",    100,  0.,300.);

//   ExpressionHisto<reco::Candidate> exprHisto(ptHisto);
//   exprHisto.initialize(theDir);


  vector<ExpressionHisto<pat::Electron>* > electronHistos;
  vector<edm::ParameterSet> histograms =
    electronHistosCfg.getParameter<vector<edm::ParameterSet> >("histograms");
  for (vector<edm::ParameterSet>::const_iterator iHist = histograms.begin();
       iHist != histograms.end(); ++iHist)
  {
    ExpressionHisto<pat::Electron> *hist = new ExpressionHisto<pat::Electron>(*iHist);
    hist->initialize(electronHistosDir);
    electronHistos.push_back(hist);
  }

  Histogrammer<pat::Electron> electronHistogrammer(electronHistosCfg, fs);


  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );

  //loop through each event
  unsigned iEvent=0;
  for (ev.toBegin(), iEvent=0; ! ev.atEnd() && iEvent < 100; ++ev, ++iEvent) {

//     std::cout << "run: " << ev.id().run()
//               << ", event: " << ev.id().event()
//               << std::endl;

    edm::EventBase const & event = ev;
//     edm::Event const & event = ev;

    edm::Handle<vector<pat::Electron> > electrons;
    event.getByLabel(electronSrcTag, electrons);

    // loop over electrons
//     for(unsigned i=0; i<electrons->size(); ++i){
//       exprHisto.fill(electrons->at(i) );
//
//
//     } // loop over electrons

    // loop over electron histos
    for (vector<ExpressionHisto<pat::Electron>* >::const_iterator iHist = electronHistos.begin();
          iHist != electronHistos.end(); ++iHist)
    {
      // loop over electrons
      vector<pat::Electron>::const_iterator iElectron = electrons->begin();
      for (uint32_t i = 0; iElectron != electrons->end(); ++i, ++iElectron) {
        const double dummyWeight = 1.0;
        if ( !(*iHist)->fill(*iElectron, dummyWeight, i) ) break;
      } // loop over electrons
    } // loop over electron histos

//     electronHistogrammer.analyze(event);
    electronHistogrammer.analyze(*electrons);
  } // end loop over events

  // delete all histograms and clear the vector of pointers
  for (vector<ExpressionHisto<pat::Electron>* >::const_iterator iHist = electronHistos.begin();
       iHist != electronHistos.end(); ++iHist)
  {
    (*iHist)->~ExpressionHisto<pat::Electron>();
  }
  electronHistos.clear();
}
