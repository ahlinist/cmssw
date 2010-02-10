#include "TauAnalysis/Skimming/interface/ewkTauValAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TMath.h>

int getIsoMode(const std::string& isoMode_string, int& error)
{
  int isoMode_int;
  if ( isoMode_string == "absoluteIso" ) {
    isoMode_int = kAbsoluteIso;
  } else if ( isoMode_string == "relativeIso" ) {
    isoMode_int = kRelativeIso;
  } else { 
    edm::LogError ("getIsoMode") << " Failed to decode isoMode string = " << isoMode_string << " !!";
    isoMode_int = kUndefinedIso;
    error = 1;
  }
  return isoMode_int;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

double calcDeltaPhi(double phi1, double phi2) 
{
  double deltaPhi = phi1 - phi2;

  if ( deltaPhi < 0. ) deltaPhi = -deltaPhi;

  if ( deltaPhi > TMath::Pi() ) deltaPhi = 2*TMath::Pi() - deltaPhi;

  return deltaPhi;
}

double calcMt(double px1, double py1, double px2, double py2) 
{
  double pt1 = TMath::Sqrt(px1*px1 + py1*py1);
  double pt2 = TMath::Sqrt(px2*px2 + py2*py2);

  double p1Dotp2 = px1*px2 + py1*py2;
  double cosAlpha = p1Dotp2/(pt1*pt2);

  return TMath::Sqrt(2*pt1*pt2*(1 - cosAlpha));
}

double calcPzeta(const reco::Candidate::LorentzVector& p1,const reco::Candidate::LorentzVector& p2, double pxMEt, double pyMEt)
{
  double cosPhi1 = cos(p1.phi());
  double sinPhi1 = sin(p1.phi());
  double cosPhi2 = cos(p2.phi());
  double sinPhi2 = sin(p2.phi());
  double zetaX = cosPhi1 + cosPhi2;
  double zetaY = sinPhi1 + sinPhi2;
  double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
  if ( zetaR > 0. ) {
    zetaX /= zetaR;
    zetaY /= zetaR;
  }

  double pxVis = p1.px() + p2.px();
  double pyVis = p1.py() + p2.py();
  double pZetaVis = pxVis*zetaX + pyVis*zetaY;

  double px = pxVis + pxMEt;
  double py = pyVis + pyMEt;
  double pZeta = px*zetaX + py*zetaY;

  return pZeta - 1.5*pZetaVis;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

bool passesElectronPreId(const reco::GsfElectron& electron)
{
  if ( (TMath::Abs(electron.eta()) < 1.479 || TMath::Abs(electron.eta()) > 1.566) && // cut ECAL barrel/endcap crack
       electron.deltaPhiSuperClusterTrackAtVtx() < 0.58 &&
       electron.deltaEtaSuperClusterTrackAtVtx() < 0.01 &&
       electron.sigmaIetaIeta() < 0.027 ) {
    return true;
  } else {
    return false;
  }
}

bool passesElectronId(const reco::GsfElectron& electron)
{
  if ( passesElectronPreId(electron) &&
       ((TMath::Abs(electron.eta()) > 1.566 && // electron reconstructed in ECAL barrel
	 electron.sigmaEtaEta() < 0.028 && electron.hcalOverEcal() < 0.1 && 
	 TMath::Abs(electron.deltaEtaSuperClusterTrackAtVtx()) < 0.0066 && 
	 TMath::Abs(electron.deltaPhiSuperClusterTrackAtVtx()) < 0.020 ) ||
	(TMath::Abs(electron.eta()) < 1.479 && // electron reconstructed in ECAL endcap
	 electron.sigmaEtaEta() < 0.0099 && electron.hcalOverEcal() < 0.1 && 
	 TMath::Abs(electron.deltaEtaSuperClusterTrackAtVtx()) < 0.004 && 
	 TMath::Abs(electron.deltaPhiSuperClusterTrackAtVtx()) < 0.025)) ) {
    return true;
  } else {
    return false;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

const reco::GsfElectron* getTheElectron(const reco::GsfElectronCollection& electrons, double electronEtaCut, double electronPtCut)
{
  const reco::GsfElectron* theElectron = 0;
  
  for ( reco::GsfElectronCollection::const_iterator electron = electrons.begin();
	electron != electrons.end(); ++electron ) {
    if ( TMath::Abs(electron->eta()) < electronEtaCut && electron->pt() > electronPtCut && passesElectronPreId(*electron) ) {
      if ( theElectron == 0 || electron->pt() > theElectron->pt() ) theElectron = &(*electron);
    }
  }
  
  return theElectron;
}

const reco::Muon* getTheMuon(const reco::MuonCollection& muons, double muonEtaCut, double muonPtCut)
{
  const reco::Muon* theMuon = 0;

  for ( reco::MuonCollection::const_iterator muon = muons.begin();
	muon != muons.end(); ++muon ) {
    if ( TMath::Abs(muon->eta()) < muonEtaCut && muon->pt() > muonPtCut ) {
      if ( theMuon == 0 || muon->pt() > theMuon->pt() ) theMuon = &(*muon);
    }
  }

  return theMuon;
}

const reco::PFTau* getTheTauJet(const reco::PFTauCollection& tauJets, double tauJetEtaCut, double tauJetPtCut, int& theTauJetIndex)
{
  const reco::PFTau* theTauJet = 0;
  theTauJetIndex = -1;

  int numTauJets = tauJets.size();
  for ( int iTauJet = 0; iTauJet < numTauJets; ++iTauJet ) {
    const reco::PFTau& tauJet = tauJets.at(iTauJet);

    if ( TMath::Abs(tauJet.eta()) < tauJetEtaCut && tauJet.pt() > tauJetPtCut ) {
      if ( theTauJet == 0 || tauJet.pt() > theTauJet->pt() ) {
	theTauJet = &tauJet;
	theTauJetIndex = iTauJet;
      }
    }
  }

  return theTauJet;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

double getVertexD0(const reco::Vertex& vertex, const reco::BeamSpot& beamSpot)
{
  double dX = vertex.x() - beamSpot.x0();
  double dY = vertex.y() - beamSpot.y0();
  return TMath::Sqrt(dX*dX + dY*dY);
}


