#ifndef TauAnalysis_Skimming_ewkTauValAuxFunctions_h
#define TauAnalysis_Skimming_ewkTauValAuxFunctions_h

/** 
 *
 * Auxiliary functions to compute quantities used by EWK Tau DQM
 * (shared by different channels)
 * 
 * \author Joshua Swanson
 *
 * \version $Revision: 1.2 $
 *
 * $Id: ewkTauValAuxFunctions.h,v 1.2 2010/01/21 09:40:36 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include <string>

enum { kAbsoluteIso, kRelativeIso, kUndefinedIso };

template<typename T>
void readEventData(const edm::Event& evt, const edm::InputTag& src, edm::Handle<T>& handle, long& numWarnings, int maxNumWarnings, 
		   bool& error, const char* errorMessage)
{
  if ( !evt.getByLabel(src, handle) ) {
    if ( numWarnings < maxNumWarnings || maxNumWarnings == -1 )
      edm::LogWarning ("readEventData") << errorMessage << " !!";
    ++numWarnings;
    error = true;
  }
}

int getIsoMode(const std::string&, int&);

double calcDeltaPhi(double, double);
double calcMt(double, double, double, double);
double calcPzeta(const reco::Candidate::LorentzVector&, const reco::Candidate::LorentzVector&, double, double);

bool passesElectronPreId(const reco::GsfElectron&);
bool passesElectronId(const reco::GsfElectron&);

const reco::GsfElectron* getTheElectron(const reco::GsfElectronCollection&, double, double);
const reco::Muon* getTheMuon(const reco::MuonCollection&, double, double);
const reco::PFTau* getTheTauJet(const reco::PFTauCollection&, double, double, int&);

double getVertexD0(const reco::Vertex&, const reco::BeamSpot&);

#endif
