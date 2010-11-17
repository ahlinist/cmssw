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

  //
  // electrons
  //
  if ( ! eleHandle.isValid()) {
    edm::LogWarning("LeptonVetoSelector") << "No results for InputTag " << electronTag_;
    return false;
  }

  for (std::vector<pat::Electron>::const_iterator ie = (*eleHandle).begin(); ie != (*eleHandle).end(); ++ie) {

    //common for PF and Reco:
    if (ie->electronID("eidLoose") < 1)
      continue;

    if (ie->pt() < minPtEle_)
      continue;

    if (fabs(ie->eta()) > maxEtaEle_)
      continue;

    if (fabs(ie->superCluster()->eta()) > 1.4442 && fabs(ie->superCluster()->eta()) < 1.566)
      continue;

    if (fabs(ie->gsfTrack()->dxy(myPosition)) > eleDxy_)
      continue;

    if (abs(ie->vz() - myPosition.z()) > 1)     
      continue;


    //PF only:
    if(ie->pfCandidateRef().isNonnull() )
      {
	double Iso = 0;

	for(int i =4; i<7; ++i)
	  {
	    const pat::IsoDeposit* Idep = ie->isoDeposit((pat::IsolationKeys)(i));   //only 4,5 and 6 filled
	    if(Idep)                                                                 //charged,neutral and Photon candidates
	      Iso += Idep->depositWithin(0.3)/ie->pt();
	    else{
	      std::cout<<"Isolation Key: "<<i<<" not found"<<std::endl;
	      continue;
	    }
	  }
	if(Iso > eleIsoPF_) continue;


	LogDebug("LeptonVetoSelector") << "Isolated electron found";
	++nElectrons;
	continue;
      }
    
    //reco only:

    if((ie->isEB() &&(( ie->dr03TkSumPt() + std::max(0., ie->dr03EcalRecHitSumEt() - 1.) + ie->dr03HcalTowerSumEt() ) / ie->p4().Pt() > eleIso_)) || ( ! ie->isEB() && ( ie->dr03TkSumPt() + ie->dr03EcalRecHitSumEt() + ie->dr03HcalTowerSumEt() ) / ie->p4().Pt() > eleIso_))   
      continue;

    LogDebug("LeptonVetoSelector") << "Isolated electron found";
    ++nElectrons;
  }
      
  setVariable("nElectrons", nElectrons);

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

    //only PF
    if(im->pfCandidateRef().isNonnull())
      {
	double Iso = 0;

	for(int i =4; i<7; ++i)
	  {
	    const pat::IsoDeposit* Idep = im->isoDeposit((pat::IsolationKeys)(i));
	    if(Idep)
	      Iso += Idep->depositWithin(0.3)/im->pt();
	    else
	      {
		std::cout<<"Isolation Key: "<<i<<" not found"<<std::endl;
		continue;
	      }
	  }
	if(Iso > muonIsoPF_) continue;

	LogDebug("LeptonVetoSelector") << "Isolated muon found";
	++nMuons;

	continue;
      }
    
    //only Reco:
    if ((im->trackIso() + im->ecalIso() + im->hcalIso()) / im->pt() > muonIso_)
      continue;


    //       std::cout << "PAT muon"<<std::endl;
    //       std::cout << "eta: "<<      im->eta()<<std::endl;
    //       std::cout << "pt: "<< im->pt()<<std::endl;
    //       std::cout << "GlobalMuonPromptTight? " << im->isGood(reco::Muon::GlobalMuonPromptTight) <<std::endl;
    //       std::cout << "GlobalMuon? " << im->isGlobalMuon() <<std::endl;
    //       std::cout << "Chi2: "<< im->combinedMuon()->normalizedChi2()<<std::endl;
    //       std::cout <<"dxy: "<<im->innerTrack()->dxy()<<std::endl;
    //       std::cout <<"hits: "<<	 im->innerTrack()->numberOfValidHits()<<std::endl;
    //       std::cout << "trackIso: "<<im->trackIso()<<std::endl;
    //       std::cout << "ecalIso: "<<	im->ecalIso()<<std::endl;
    //       std::cout << "hcalIso: "<<	im->hcalIso()<<std::endl;
    //       std::cout << "relIso: "<<(im->trackIso()+im->ecalIso()+im->hcalIso())/im->pt()<<std::endl;
    //       std::cout << "-----------muon end"<<std::endl;


    LogDebug("LeptonVetoSelector") << "Isolated muon found";
    ++nMuons;

  }
  setVariable("nMuons", nMuons);

  //  //
  //  // taus
  //  //
  //  if ( tauHandle.isValid() )
  //    for ( std::vector<pat::Tau>::const_iterator it=(*tauHandle).begin();
  //	  it!=(*tauHandle).end(); ++it ) {
  //      // Only taus above Et cut
  //      if ( it->et() > minEtTau_
  //           && (it->trackIso()+it->et())/it->et() < tauIso_ ) {
  //        LogDebug("LeptonVetoSelector") << "Isolated tau found";
  //        ++nTaus;
  //      }
  //    }
  //  setVariable("nTaus",nTaus);
  //  if((nElectrons +  nMuons + nTaus) > 0)
  //  std::cout<<"nMuons: "<<nMuons<<"  nElecs: "<<nElectrons<<std::endl;

  // Selection
  if (!invertVeto_){
    if ( nElectrons>0 || nMuons>0 || nTaus>0 ) return false;

    //
    // all electrons, muons and taus are below Et cut or not isolated: event passes
    //
    return true;
  }
  else{
    if ( (nElectrons +  nMuons + nTaus) == 1 && nTaus ==0 ) return true;
  }
  return false;
}
//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, LeptonVetoSelector, "LeptonVetoSelector");
