/** \class HLTMuonTurnOnAnalyzer
 *  Get L1/HLT turn on curves
 *
 *  \author J. Alcaraz
 */

#include "HLTrigger/Muon/test/HLTMuonTurnOnAnalyzer.h"

// Collaborating Class Header
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TFile.h"
#include "TH1F.h"

using namespace std;
using namespace edm;
using namespace reco;

/// Constructor
HLTMuonTurnOnAnalyzer::HLTMuonTurnOnAnalyzer(const ParameterSet& pset)
{
  theGenLabel = pset.getUntrackedParameter<InputTag>("GenLabel");
  theL1CollectionLabel = pset.getUntrackedParameter<InputTag>("L1CollectionLabel");
  theHLTCollectionLabels = pset.getUntrackedParameter<std::vector<InputTag> >("HLTCollectionLabels");
  theL1ReferenceThreshold = pset.getUntrackedParameter<double>("L1ReferenceThreshold");
  theHLTReferenceThresholds = pset.getUntrackedParameter<std::vector<double> >("HLTReferenceThresholds");
  theNSigmas = pset.getUntrackedParameter<std::vector<double> >("NSigmas90");

  theNumberOfObjects = pset.getUntrackedParameter<unsigned int>("NumberOfObjects");

  thePtMin = pset.getUntrackedParameter<double>("PtMin");
  thePtMax = pset.getUntrackedParameter<double>("PtMax");
  theNbins = pset.getUntrackedParameter<unsigned int>("Nbins");

  theRootFileName = pset.getUntrackedParameter<string>("RootFileName");

  theNumberOfEvents = 0.;
}

/// Destructor
HLTMuonTurnOnAnalyzer::~HLTMuonTurnOnAnalyzer(){
}

void HLTMuonTurnOnAnalyzer::beginJob(const EventSetup& eventSetup){
  // Create the root file
  theFile = new TFile(theRootFileName.c_str(), "RECREATE");
  theFile->cd();

  char chname[256];
  char chtitle[256];
  snprintf(chname, 255, "eff_%s", theL1CollectionLabel.encode().c_str());
  snprintf(chtitle, 255, "Efficiency (%%) vs Generated Pt (GeV), label=%s", theL1CollectionLabel.encode().c_str());
  hL1eff = new TH1F(chname, chtitle, theNbins, thePtMin, thePtMax);
  hL1nor = new TH1F(chname, chtitle, theNbins, thePtMin, thePtMax);

  for (unsigned int i=0; i<theHLTCollectionLabels.size(); i++) {
      snprintf(chname, 255, "eff_%s", theHLTCollectionLabels[i].encode().c_str());
      snprintf(chtitle, 255, "Efficiency (%%) vs Generated Pt (GeV), label=%s", theHLTCollectionLabels[i].encode().c_str());
      hHLTeff.push_back(new TH1F(chname, chtitle, theNbins, thePtMin, thePtMax));
      hHLTnor.push_back(new TH1F(chname, chtitle, theNbins, thePtMin, thePtMax));
  }

}

void HLTMuonTurnOnAnalyzer::endJob(){
   LogInfo("HLTMuonTurnOnAnalyzer") << " (Weighted) number of analyzed events= " << theNumberOfEvents;
  theFile->cd();

  if (theNumberOfEvents==0) {
   LogInfo("HLTMuonTurnOnAnalyzer") << " No histograms will be written because number of events=0!!!";
      theFile->Close();
      return;
  }

  // L1 operations
  hL1eff->Divide(hL1nor);
  hL1eff->Scale(100.);

  // HLT operations
  for (unsigned int i=0; i<theHLTCollectionLabels.size(); i++) {
      hHLTeff[i]->Divide(hHLTnor[i]);
      hHLTeff[i]->Scale(100.);
  }

  // Write the histos to file
  hL1eff->Write();
  for (unsigned int i=0; i<theHLTCollectionLabels.size(); i++) {
      hHLTeff[i]->Write();
  }

  theFile->Close();
}

void HLTMuonTurnOnAnalyzer::analyze(const Event & event, const EventSetup& eventSetup){
  
  theFile->cd();

  // Get the HepMC product
  double this_event_weight=1.;
  Handle<HepMCProduct> genProduct;
  event.getByLabel(theGenLabel,genProduct);

  const HepMC::GenEvent* evt = genProduct->GetEvent();
  HepMC::WeightContainer weights = evt->weights();
  if ( weights.size() > 0 )  this_event_weight=weights[0];
  theNumberOfEvents += this_event_weight;

  // Get the muon with maximum pt at generator level
  bool genmuon_found = false;
  double ptgen = -1;
  HepMC::GenEvent::particle_const_iterator part;
  for (part = evt->particles_begin(); part != evt->particles_end(); ++part ) {
      int id1 = (*part)->pdg_id();
      if (id1!=13 && id1!=-13) continue;
      float pt1 = (*part)->momentum().perp();
      if (pt1>ptgen) {
            genmuon_found = true;
            ptgen = pt1;
      }
  }

  if (!genmuon_found) {
      LogInfo("HLTMuonTurnOnAnalyzer") << " NO generated muon found!!!";
      LogInfo("HLTMuonTurnOnAnalyzer") << " Skipping event";
      return;
  }


  // Get the L1 collection
  Handle<HLTFilterObjectWithRefs> l1cands;
  event.getByLabel(theL1CollectionLabel, l1cands);

  // Get the HLT collections
  std::vector<Handle<HLTFilterObjectWithRefs> > hltcands;
  hltcands.reserve(theHLTCollectionLabels.size());
  unsigned int modules_in_this_event = 0;
  for (unsigned int i=0; i<theHLTCollectionLabels.size(); i++) {
      event.getByLabel(theHLTCollectionLabels[i], hltcands[i]);
      modules_in_this_event++;
  }

  // Fix L1 thresholds to obtain the efficiecy plot
  unsigned int nL1FoundRef = 0;
  double epsilon = 0.001;
  for (unsigned int k=0; k<l1cands->size(); k++) {
      RefToBase<Candidate> candref = l1cands->getParticleRef(k);
      // L1 PTs are "quantized" due to LUTs. 
      // Their meaning: true_pt > ptLUT more than 90% pof the times
      double ptLUT = candref->pt();
      // Add "epsilon" to avoid rounding errors when ptLUT==L1Threshold
      if (ptLUT+epsilon>theL1ReferenceThreshold) nL1FoundRef++;
  }
  hL1nor->Fill(ptgen,this_event_weight);
  if (nL1FoundRef>=theNumberOfObjects) hL1eff->Fill(ptgen,this_event_weight);
  //if (nL1FoundRef<theNumberOfObjects) return;

  // HLT filling
  for (unsigned int i=0; i<modules_in_this_event; i++) {
      double ptcut = theHLTReferenceThresholds[i];
      unsigned nFound = 0;
      for (unsigned int k=0; k<hltcands[i]->size(); k++) {
            RefToBase<Candidate> candref = hltcands[i]->getParticleRef(k);
            TrackRef tk = candref->get<TrackRef>();
            double pt = tk->pt();
            double err0 = tk->error(0);
            double abspar0 = fabs(tk->parameter(0));
            // convert to 90% efficiency threshold
            if (abspar0>0) pt += theNSigmas[i]*err0/abspar0*pt;
            if (pt>ptcut) nFound++;
      }
      hHLTnor[i]->Fill(ptgen,this_event_weight);
      if (nFound>=theNumberOfObjects) hHLTeff[i]->Fill(ptgen,this_event_weight);
  }

}

DEFINE_FWK_MODULE(HLTMuonTurnOnAnalyzer);
