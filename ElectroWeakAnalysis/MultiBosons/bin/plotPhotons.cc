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

#include "ElectroWeakAnalysis/MultiBosons/interface/PhotonHistogrammer.h"

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
  edm::ParameterSet const& photonHistosCfg = cfg->getParameter<edm::ParameterSet>("photonHistos");
  edm::InputTag photonSrcTag = photonHistosCfg.getParameter<edm::InputTag>("src");

  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
//   TFileDirectory theDir = fs.mkdir( "histos" );
  TFileDirectory photonHistosDir = fs.mkdir( "photonHistos" );

//   TH1F* photonPt_  = theDir.make<TH1F>("photonPt", "pt",    100,  0.,300.);

//   ExpressionHisto<reco::Candidate> exprHisto(ptHisto);
//   exprHisto.initialize(theDir);


  vector<ExpressionHisto<pat::Photon>* > photonHistos;
  vector<edm::ParameterSet> histograms =
    photonHistosCfg.getParameter<vector<edm::ParameterSet> >("histograms");
  for (vector<edm::ParameterSet>::const_iterator iHist = histograms.begin();
       iHist != histograms.end(); ++iHist)
  {
    ExpressionHisto<pat::Photon> *hist = new ExpressionHisto<pat::Photon>(*iHist);
    hist->initialize(photonHistosDir);
    photonHistos.push_back(hist);
  }

  PhotonHistogrammer photonHistogrammer(photonHistosCfg, fs);


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

    edm::Handle<vector<pat::Photon> > photons;
    event.getByLabel(photonSrcTag, photons);

    // loop over photons
//     for(unsigned i=0; i<photons->size(); ++i){
//       exprHisto.fill(photons->at(i) );
//
//
//     } // loop over photons

    // loop over photon histos
    for (vector<ExpressionHisto<pat::Photon>* >::const_iterator iHist = photonHistos.begin();
          iHist != photonHistos.end(); ++iHist)
    {
      // loop over photons
      vector<pat::Photon>::const_iterator iPhoton = photons->begin();
      for (uint32_t i = 0; iPhoton != photons->end(); ++i, ++iPhoton) {
        const double dummyWeight = 1.0;
        if ( !(*iHist)->fill(*iPhoton, dummyWeight, i) ) break;
      } // loop over photons
    } // loop over photon histos

//     photonHistogrammer.analyze(event);
    photonHistogrammer.analyze(*photons);
  } // end loop over events

  // delete all histograms and clear the vector of pointers
  for (vector<ExpressionHisto<pat::Photon>* >::const_iterator iHist = photonHistos.begin();
       iHist != photonHistos.end(); ++iHist)
  {
    (*iHist)->~ExpressionHisto<pat::Photon>();
  }
  photonHistos.clear();
}
