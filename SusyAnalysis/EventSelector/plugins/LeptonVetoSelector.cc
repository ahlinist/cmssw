#include "SusyAnalysis/EventSelector/interface/LeptonVetoSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "TMath.h"

#include <DataFormats/PatCandidates/interface/Jet.h>

#include <vector>

//__________________________________________________________________________________________________
LeptonVetoSelector::LeptonVetoSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), electronTag_(pset.getParameter<edm::InputTag> ("electronTag")), muonTag_(pset.getParameter<
            edm::InputTag> ("muonTag")), vertex_(pset.getParameter<edm::InputTag> ("vertex")), minPtEle_(
            pset.getParameter<double> ("minElectronPt")), maxEtaEle_(pset.getParameter<double> ("maxElectronEta")),
            eleIso_(pset.getParameter<double> ("electronIsolation")), 
            eleIsoPF_(pset.getParameter<double> ("electronIsolationPF")),eleDxy_(pset.getParameter<double> (
                     "maxElectronDxy")), minPtMuon_(pset.getParameter<double> ("minMuonPt")), maxEtaMuon_(
                     pset.getParameter<double> ("maxMuonEta")), muonIso_(pset.getParameter<double> ("muonIsolation")),
            muonIsoPF_(pset.getParameter<double> ("muonIsolationPF")),
            muonHits_(pset.getParameter<unsigned int> ("muonMinHits")), muonDxy_(pset.getParameter<double> (
                     "maxMuonDxy")), invertVeto_  ( pset.getParameter<bool>        ("invertVeto")        )

//,
//  tauTag_    ( pset.getParameter<edm::InputTag>("tau")       ),
//  minEtTau_  ( pset.getParameter<double>("minTauEt")         ),
//  tauIso_    ( pset.getParameter<double>("tauIsolation")     )
{

   // Store counter
   defineVariable("nElectrons");
   defineVariable("nMuons");
   //  defineVariable("nTaus");
   defineVariable("bestIsoElec");
   defineVariable("bestIsoMuon");

   if (invertVeto_) {
     defineVariable("Electron1Pt");
     defineVariable("Muon1Pt");
     defineVariable("Electron1Eta");
     defineVariable("Muon1Eta");
     defineVariable("Electron1Phi");
     defineVariable("Muon1Phi");
     defineVariable("Electron1DR");
     defineVariable("Muon1DR");
     defineVariable("MuonMTW");
   }

}


//__________________________________________________________________________________________________
bool LeptonVetoSelector::select(const edm::Event& event) const {

  // Input collections
  edm::Handle<std::vector<pat::Electron> > eleHandle;
  event.getByLabel(electronTag_, eleHandle);
  edm::Handle<std::vector<pat::Muon> > muonHandle;
  event.getByLabel(muonTag_, muonHandle);

  reco::Vertex myBeamSpot;
  edm::Handle <edm::View<reco::Vertex> >vertices;
  event.getByLabel(vertex_, vertices); 
  if(!vertices->empty() && !vertices->front().isFake() )
    myBeamSpot = vertices->front();

  const math::XYZPointD & myPosition = myBeamSpot.position();

  //  edm::Handle< std::vector<pat::Tau> > tauHandle;
  //  event.getByLabel(tauTag_, tauHandle);

  resetVariables();

  int nElectrons = 0, nMuons = 0, nTaus = 0; // Counters

  double currIso = 0.;
  double bestIsoElec = 10.;
  double bestIsoMuon = 10.;
  double ElectronPt[3] = {-10.,-10.,-10.};
  double MuonPt[3] = {-10.,-10.,-10.};
  double ElectronEta[3] = {-10.,-10.,-10.};
  double MuonEta [3] = {-10.,-10.,-10.};
  double ElectronPhi [3] = {-10.,-10.,-10.};
  double MuonPhi [3] = {-10.,-10.,-10.};
  double ElectronDR [3] = {-10.,-10.,-10.};
  double MuonDR [3] = {-10.,-10.,-10.};
  double MTW=0;


  math::PtEtaPhiMLorentzVector mhtvec(0.0, 0.0, 0.0, 0.0);

  edm::Handle <edm::View<pat::Jet> >jets;
  if ( !event.getByLabel("patJetsAK5PF",jets) ) {
    std::cout<<"Could not extract tag muon cands with input tag " 
	     << "patJetsAK5PF"<<std::endl;
  }
  else (event.getByLabel("patJetsAK5PF",jets) );


  for (edm::View<pat::Jet>::const_iterator jet= jets->begin(); jet!=jets->end(); ++jet){
        if (jet->pt() < 30. || fabs(jet->eta()) > 5.) continue;
        mhtvec -= jet->p4();
  }

  //
  // electrons
  //
  if ( ! eleHandle.isValid()) {
    edm::LogWarning("LeptonVetoSelector") << "No results for InputTag " << electronTag_;
    return false;
  }

  for (std::vector<pat::Electron>::const_iterator ie = (*eleHandle).begin(); ie != (*eleHandle).end(); ++ie) {

    //common for PF and Reco:
    if (ie->pt() < minPtEle_)
      continue;

    if (fabs(ie->superCluster()->eta()) >= maxEtaEle_)
      continue;

    if (fabs(ie->superCluster()->eta()) >= 1.4442 && fabs(ie->superCluster()->eta()) <= 1.566)
      continue;

    if (fabs(ie->gsfTrack()->dxy(myPosition)) > eleDxy_)
      continue;

    if (abs(ie->vz() - myPosition.z()) > 1)     
      continue;

    currIso = 0;
 
    //PF only:
    if(ie->pfCandidateRef().isNonnull() )
      {
	if(ie->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() > 1) continue;
	/*
	for(int i =4; i<7; ++i)
	  {
	    const pat::IsoDeposit* Idep = ie->isoDeposit((pat::IsolationKeys)(i));   //only 4,5 and 6 filled
	    if(Idep)                                                                 //charged,neutral and Photon candidates
	      currIso += Idep->depositWithin(0.3)/ie->pt();
	    else{
	      std::cout<<"Isolation Key: "<<i<<" not found"<<std::endl;
	      continue;
	    }
	  }
	*/
	currIso = (ie->chargedHadronIso()+ie->photonIso()+ie->neutralHadronIso()) / ie->pt();
	if(currIso > eleIsoPF_) continue;

      } else {

      //reco only:
      if( ie->isEB() ) currIso = ( ie->dr03TkSumPt() + std::max(0., ie->dr03EcalRecHitSumEt() - 1.) + ie->dr03HcalTowerSumEt() ) / ie->p4().Pt();
      else             currIso = ( ie->dr03TkSumPt() + ie->dr03EcalRecHitSumEt() + ie->dr03HcalTowerSumEt() ) / ie->p4().Pt();

      if(currIso < bestIsoElec) bestIsoElec = currIso;

    if (ie->electronID("eidLoose") < 1)
      continue;

    if((ie->isEB() &&(( ie->dr03TkSumPt() + std::max(0., ie->dr03EcalRecHitSumEt() - 1.) + ie->dr03HcalTowerSumEt() ) / ie->p4().Pt() > eleIso_)) || ( ! ie->isEB() && ( ie->dr03TkSumPt() + ie->dr03EcalRecHitSumEt() + ie->dr03HcalTowerSumEt() ) / ie->p4().Pt() > eleIso_))   
      continue;
      if( currIso > eleIso_ ) continue;
    }
  
    if (nElectrons<3){
      ElectronPt[nElectrons] = ie->pt();
      ElectronEta[nElectrons] = ie->eta();
      ElectronPhi[nElectrons] = ie->phi();
      double DR = 9999.;
      double minDR = 9999.;
      for (edm::View<pat::Jet>::const_iterator jet= jets->begin(); jet!=jets->end(); ++jet){
	if ( jet->pt()>30 ){//&& jet->emEnergyFraction()<0.9 ){
	  DR = deltaR( ie->eta(), ie->phi(), jet->eta(), jet->phi());
	  if(DR < minDR && DR > 0.05) {
	    //if(DR < minDR) {
	    minDR = DR;
	  } 
	}
      } //end dr calculation
      

      ElectronDR[nMuons] = minDR;
    }

    LogDebug("LeptonVetoSelector") << "Isolated electron found";
    ++nElectrons;
  }
      
  setVariable("nElectrons", nElectrons);
  setVariable("bestIsoElec", bestIsoElec);

  //
  // muons
  //

  if ( ! muonHandle.isValid() ) {
    edm::LogWarning("LeptonVetoSelector") << "No results for InputTag " << muonTag_;
    return false;
  }

  for (std::vector<pat::Muon>::const_iterator im = (*muonHandle).begin(); im != (*muonHandle).end(); ++im) {

    //common for PF and Reco:
    
    if( im->isGood("GlobalMuonPromptTight") < 1 ) continue;
    if (im->pt() < minPtMuon_)
      continue;

    if (fabs(im->eta()) > maxEtaMuon_)
      continue;

    if (fabs(im->innerTrack()->dxy(myPosition)) > muonDxy_)
      continue;

    if (fabs(im->vz() - myPosition.z()) > 1)     
      continue;

    if (im->innerTrack()->numberOfValidHits() < muonHits_)
      continue;     

   if (im->innerTrack()->hitPattern().numberOfValidPixelHits() == 0) continue;

    currIso = 0;

    //only PF
    if(im->pfCandidateRef().isNonnull())
      {
	/*
	for(int i =4; i<7; ++i)
	  {
	    const pat::IsoDeposit* Idep = im->isoDeposit((pat::IsolationKeys)(i));
	    if(Idep)
	      currIso += Idep->depositWithin(0.3)/im->pt();
	    else
	      {
		std::cout<<"Isolation Key: "<<i<<" not found"<<std::endl;
		continue;
	      }
	  }
	*/
	currIso = (im->chargedHadronIso()+im->photonIso()+im->neutralHadronIso()) / im->pt();
	if(currIso < bestIsoMuon) bestIsoMuon = currIso;
	if(currIso > muonIsoPF_) continue;

	/*
	  for inversion tests only:
	if(currIso > bestIsoMuon) bestIsoMuon = currIso;
	if(currIso < muonIsoPF_) continue;
	*/

      } else {
      //only Reco:
      currIso = (im->trackIso() + im->ecalIso() + im->hcalIso()) / im->pt();
      if(currIso < bestIsoMuon) bestIsoMuon = currIso;

      if (currIso > muonIso_)
	continue;
    }

    if (nMuons<3){
      MuonPt[nMuons] = im->pt();
      MuonEta[nMuons] = im->eta();
      MuonPhi[nMuons] = im->phi();



      double DR = 9999.;
      double minDR = 9999.;
      for (edm::View<pat::Jet>::const_iterator jet= jets->begin(); jet!=jets->end(); ++jet){
	if ( jet->pt()>30 ){//&& jet->emEnergyFraction()<0.9 ){
	  DR = deltaR( im->eta(), im->phi(), jet->eta(), jet->phi());
	  if(DR < minDR && DR > 0.05) {
	    //if(DR < minDR) {
	    minDR = DR;
	  } 
	}
      } //end dr calculation
      

      MuonDR[nMuons] = minDR;
      double DPMuMHT = reco::deltaPhi(im->phi(), mhtvec.phi());
      MTW = sqrt(2 * im->pt() * mhtvec.pt() * (1 - cos(DPMuMHT )) );
    }

    LogDebug("LeptonVetoSelector") << "Isolated muon found";
    ++nMuons;

  }
  setVariable("nMuons", nMuons);
  setVariable("bestIsoMuon", bestIsoMuon); 
  if (invertVeto_){
    setVariable("Muon1Pt", MuonPt[0]);
    setVariable("Muon1Eta", MuonEta[0]);
    setVariable("Muon1Phi", MuonPhi[0]);
    setVariable("Electron1Pt", ElectronPt[0]);
    setVariable("Electron1Eta", ElectronEta[0]);
    setVariable("Electron1Phi", ElectronPhi[0]);
    setVariable("Muon1DR", MuonDR[0]);
    setVariable("Electron1DR", ElectronDR[0]);
    setVariable("MuonMTW", MTW);
  }
  

  // Selection
  if (!invertVeto_){
    if ( nElectrons>0 || nMuons>0 || nTaus>0 ) return false;

    //
    // all electrons, muons and taus are below Et cut or not isolated: event passes
    //
    return true;
  }
  else{
    if ( (nElectrons +  nMuons + nTaus) == 1 && nTaus ==0 && nElectrons == 0 ) return true;
    //    if ( (nElectrons +  nMuons + nTaus) == 1 && nTaus ==0 ) return true;
  }
  return false;
}
//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, LeptonVetoSelector, "LeptonVetoSelector");
