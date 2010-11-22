#include "SusyAnalysis/EventSelector/interface/AddOtherVariables.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "TMath.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

#include <vector>
#include <numeric>
//__________________________________________________________________________________________________
AddOtherVariables::AddOtherVariables(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), 
   jetTag_(pset.getParameter<edm::InputTag> ("jetTag")), 
   metTag_(pset.getParameter<edm::InputTag> ("metTag")), 
   electronTag_(pset.getParameter<edm::InputTag> ("electronTag")), 
   muonTag_(pset.getParameter<edm::InputTag> ("muonTag")), 
   vertex_(pset.getParameter<edm::InputTag> ("vertex")),

   jetptmin_(pset.getParameter<double> ("JetPtMin")), 
   jetetamax_(pset.getParameter<double> ("JetEtaMax")), 
   useJetID_(pset.getParameter<bool> ("useJetID")),
   rejectEvtJetID_(pset.getParameter<bool> ("rejectEvtJetID"))
//  tauTag_    ( pset.getParameter<edm::InputTag>("tau")       ),
{
   // Store counter
   defineVariable("SET");
   defineVariable("MET");
   defineVariable("METsig");
   defineVariable("HT");
   defineVariable("MHT");
   defineVariable("MHTsig");
   defineVariable("alphaT");
   defineVariable("dPhiMin");
   defineVariable("dPhiBiased");
   defineVariable("JetMult");
   //.... other variables we don't cut on and are not produced in other selectors


   //defineVariable("nTaus");
   jetIDLooseCalo_ = new JetIDSelectionFunctor( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );
   jetIDLoosePF_   = new PFJetIDSelectionFunctor( PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE );
}

//
// member functions
//
// ------------ biased delta phi ----------------------------
double AddOtherVariables::dPhiBiased(const edm::View<reco::Candidate> * jets, 
         const edm::View<reco::Candidate>::const_iterator excljet) const {
   math::PtEtaPhiMLorentzVector mhtvec(0.0, 0.0, 0.0, 0.0);
   for (edm::View<reco::Candidate>::const_iterator jet = jets->begin(); jet != jets->end(); ++jet) {
      if (fabs(jet->eta()) > jetetamax_)
         continue;
      if (jet->pt() < jetptmin_)
         continue;
      if (jet == excljet)
         continue;
      mhtvec -= jet->p4();
   }
   return fabs(deltaPhi(mhtvec, *excljet));
}

// ------------ helper func for alphaT ----------------------------
std::vector<double> AddOtherVariables::deltaSumPt_permutations(const std::vector<LorentzV>& p4s) const {
   std::vector<std::vector<double> > ht(1 << (p4s.size() - 1), std::vector<double>(2, 0.));
   for (unsigned i = 0; i < ht.size(); i++) {
      for (unsigned j = 0; j < p4s.size(); j++) {
         ht[i][(i / (1 << j)) % 2] += p4s[j].pt();
      }
   }
   std::vector<double> deltaHT;
   for (unsigned i = 0; i < ht.size(); i++)
      deltaHT.push_back(fabs(ht[i][0] - ht[i][1]));
   return deltaHT;
}

// ------------ alphaT calculation ----------------------------
double AddOtherVariables::alphaT(const std::vector<LorentzV>& p4s) const {
   if (p4s.size() < 2)
      return 0;

   std::vector<double> pTs;
   for (unsigned i = 0; i < p4s.size(); i++)
      pTs.push_back(p4s[i].pt());
   for (unsigned i = 0; i < p4s.size(); i++)
      pTs.push_back(p4s[i].pt());
   const std::vector<double> DHTper(deltaSumPt_permutations(p4s));

   const double mDHT = *(std::min_element(DHTper.begin(), DHTper.end()));
   const double sumPT = accumulate(pTs.begin(), pTs.end(), double(0));
   const LorentzV sumP4 = accumulate(p4s.begin(), p4s.end(), LorentzV());

   return 0.5 * (sumPT - mDHT) / sqrt(sumPT * sumPT - sumP4.perp2());
}


bool AddOtherVariables::isLoose(const pat::Jet* jet) const {
  if (!useJetID_ || !jet) return true;
  else if( jet->isCaloJet() || jet->isJPTJet() ){
      pat::strbitset ret = jetIDLooseCalo_->getBitTemplate();
      ret.set(false);
      return (*jetIDLooseCalo_)( *jet, ret );
  }
  else if ( jet->isPFJet() ){
      pat::strbitset ret = jetIDLoosePF_->getBitTemplate();
      ret.set(false);
      return (*jetIDLoosePF_)( *jet, ret );
  }
  return false;
}


//__________________________________________________________________________________________________
bool AddOtherVariables::select(const edm::Event& event) const {

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
   //const math::XYZPointD & myPosition = myBeamSpot.position();
   //  edm::Handle< std::vector<pat::Tau> > tauHandle;
   //  event.getByLabel(tauTag_, tauHandle);

   //jet collection - can be either reco::jet or pat::jet
   edm::Handle<edm::View<reco::Candidate> > jet_hnd;
   event.getByLabel(jetTag_, jet_hnd);

   //MET collection - can be either reco::jet or pat::jet
   edm::Handle<edm::View<reco::MET> > met_hnd;
   event.getByLabel(metTag_, met_hnd);


   double ht = 0.0, dphimin = 999., dphibiased = 999., SEt=0;
   int jetmult = 0;
   math::PtEtaPhiMLorentzVector mhtvec(0.0, 0.0, 0.0, 0.0);
   std::vector<LorentzV> goodJets;

   //loop over all jets - use only those fulfilling the cuts defined in cfg file
   for (edm::View<reco::Candidate>::const_iterator jet = jet_hnd->begin(); jet != jet_hnd->end(); ++jet) {
      const pat::Jet * patjet = static_cast<const pat::Jet*>( &(*jet) );
      if (!isLoose(patjet)) continue;       //jet ID

      SEt += patjet->correctedJet(pat::JetCorrFactors::Raw).pt();

      if (fabs(jet->eta()) > jetetamax_) continue;
      if (jet->pt() < jetptmin_) continue;

      goodJets.push_back(jet->p4());
      mhtvec -= jet->p4();
      ht += jet->pt();
      ++jetmult;
   }

   math::PtEtaPhiMLorentzVector bias_mhtvec(0.0, 0.0, 0.0, 0.0);
   for (unsigned i = 0; i < goodJets.size(); i++) {
     if(i >= 3 ) break;

     if ( fabs(deltaPhi(goodJets[i], mhtvec)) < dphimin )
       dphimin = fabs(deltaPhi(goodJets[i], mhtvec));

     bias_mhtvec = mhtvec + goodJets[i];
     if ( fabs(deltaPhi(goodJets[i], bias_mhtvec)) < dphibiased )
       dphibiased = fabs(deltaPhi(goodJets[i], bias_mhtvec));
   }

   resetVariables();
   setVariable("HT",  ht); 
   setVariable("MHT", mhtvec.pt());
   if (ht) setVariable("MHTsig",mhtvec.pt() / sqrt(ht));
   setVariable("JetMult",jetmult);
   setVariable("alphaT", alphaT(goodJets));
   setVariable("dPhiMin",dphimin);
   setVariable("dPhiBiased",dphibiased);
   setVariable("SET", SEt); 
   setVariable("MET", met_hnd->front().et());
   if (SEt) setVariable("METsig",met_hnd->front().et() / sqrt(SEt));

   return true;
}
//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, AddOtherVariables, "AddOtherVariables");
