#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtCollinearApproxHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

const double epsilon = 0.01;

template<typename T1, typename T2>
bool matchesGenCandidatePair(const CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::CompositePtrCandidateT1T2MEtCollinearApproxHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtCollinearApproxHistManager::CompositePtrCandidateT1T2MEtCollinearApproxHistManager>:" << std::endl;

  diTauCandidateSrc_ = cfg.getParameter<edm::InputTag>("diTauCandidateSource");
  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_ << std::endl;

  genParticleSrc_ = cfg.getParameter<edm::InputTag>("genParticleSource");
  genJetSrc_ = cfg.getParameter<edm::InputTag>("genJetSource");
  recoJetSrc_ = cfg.getParameter<edm::InputTag>("recoJetSource");

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");
  
  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");

  makeCollinearApproxX1X2histograms_ = cfg.getParameter<bool>("makeCollinearApproxX1X2histograms");

  makeCollinearApproxMassDepHistograms_ = cfg.getParameter<bool>("makeCollinearApproxMassDepHistograms");
  
  collinearApproxMassDPhi12Incr_ = TMath::Pi()/18.;
  numCollinearApproxMassDiTauPtBins_ = 25;
  collinearApproxMassDiTauPtIncr_ = 2.;
  numCollinearApproxMassMEtPtBins_ = 25;
  collinearApproxMassMEtPtIncr_ = 2.;
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::~CompositePtrCandidateT1T2MEtCollinearApproxHistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T1>*>::iterator it = diTauLeg1WeightExtractors_.begin();
	it != diTauLeg1WeightExtractors_.end(); ++it ) {
    delete (*it);
  }

  for ( typename std::vector<FakeRateJetWeightExtractor<T2>*>::iterator it = diTauLeg2WeightExtractors_.begin();
	it != diTauLeg2WeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtCollinearApproxHistManager::bookHistogramsImp>:" << std::endl;

  hGenX1_ = book1D("GenX1", "gen. X_{1}", 51, -0.01, 1.01);
  hGenX2_ = book1D("GenX2", "gen. X_{2}", 51, -0.01, 1.01);
  hX1_ = book1D("X1", "X_{1}", 51, -0.01, 1.01);
  hX2_ = book1D("X2", "X_{2}", 51, -0.01, 1.01);
  if ( makeCollinearApproxX1X2histograms_ ) {
    hGenX1vsX2_ = book2D("GenX1vsX2", "gen. X_{1} vs. X_{2}", 51, -0.01, 1.01, 51, -0.01, 1.01);
  
    hX1vsGenX1_ = book2D("X1vsGenX1", "X_{1} vs. gen. X_{1}", 21, -0.01, 1.01, 100, -2.5, +2.5);
    hX1vsGenX1Profile_ = bookProfile1D("X1vsGenX1Profile", "X_{1} vs. gen. X_{1}", 51, -0.01, 1.01);
    hX2vsGenX2_ = book2D("X2vsGenX2", "X_{2} vs. gen. X_{2}", 21, -0.01, 1.01, 100, -2.5, +2.5);
    hX2vsGenX2Profile_ = bookProfile1D("X2vsGenX2Profile", "X_{2} vs. gen. X_{2}", 51, -0.01, 1.01);
  }

  hGenLeg1ProjGenMEt_ = book1D("GenLeg1ProjGenMEt", "gen. leg_{1} P_{T} || missing E_{T}", 75, 0., 150.);
  hGenLeg2ProjGenMEt_ = book1D("GenLeg2ProjGenMEt", "gen. leg_{2} P_{T} || missing E_{T}", 75, 0., 150.);
  
  bookWeightHistograms(*dqmStore_, "DiTauCandidateWeight", "Composite Weight", 
		       hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_,
		       hDiTauCandidateWeightLinear_);
  
  hCollinearApproxEffDPhi12dep_ = book1D("CollinearApproxEffDPhi12dep", "Eff. of collinear Approximation as function of #Delta#phi_{1,2}", 36, -epsilon, TMath::Pi() + epsilon);
  hCollinearApproxEffDiTauPtDep_ = book1D("CollinearApproxEffDiTauPtDep", "Eff. of collinear Approximation as function of Visible P_{T}", 50, 0., 100.);
  hCollinearApproxEffMEtDep_ = book1D("CollinearApproxEffMEtDep", "Eff. of collinear Approximation as function of MEt", 75, 0., 150.);

  hCollinearApproxEta_ = book1D("CollinearApproxEta", "Collinear Approximation #eta", 100, -5., +5.);
  hCollinearApproxMass_ = book1D("CollinearApproxMass", "Collinear Approximation Mass", 50, 0., 250.);
  hCollinearApproxMassRes_ = book1D("CollinearApproxMassRes", "Collinear Approximation Mass Resolution", 100, -2.5, +2.5);
  hCollinearApproxMassVsPt_ = book2D("CollinearApproxMassVsPt", "Collinear Approximation Mass vs. P_{T}", 30, 0., 150., 25, 0., 250.);
  hCollinearApproxMassVsDPhi12_ = book2D("CollinearApproxMassVsDPhi12", "Collinear Approximation Mass vs. #Delta#phi_{1,2}", 18, -epsilon, TMath::Pi() + epsilon, 25, 0., 250.);
  hCollinearApproxX1_ = book1D("CollinearApproxX1", "Collinear Approximation X_{1}", 100, -2.5, +2.5);
  hCollinearApproxX2_ = book1D("CollinearApproxX2", "Collinear Approximation X_{2}", 100, -2.5, +2.5);
  if ( makeCollinearApproxX1X2histograms_ ) {
    hCollinearApproxX1vsX2_ = book2D("CollinearApproxX1vsX2", "Collinear Approximation X_{1} vs. X_{2}", 50, -2.5, +2.5, 50, -2.5, +2.5);
  }

  if ( makeCollinearApproxMassDepHistograms_ ) {
    int numDPhi12bins = TMath::CeilNint(TMath::Pi()/collinearApproxMassDPhi12Incr_);
    for ( int iDPhi12bin = 0; iDPhi12bin < numDPhi12bins; ++iDPhi12bin ) {
      std::ostringstream iDPhi12binString;
      iDPhi12binString << std::setfill('0') << std::setw(2) << iDPhi12bin;
    
      std::string hCollinearApproxMassDPhi12depName_i 
	= std::string("CollinearApproxMassDPhi12dep").append("_").append(iDPhi12binString.str());
      hCollinearApproxMassDPhi12dep_.push_back(book1D(hCollinearApproxMassDPhi12depName_i,
						      hCollinearApproxMassDPhi12depName_i, 50, 0., 250.));
    }
    
    for ( unsigned iDiTauPtBin = 0; iDiTauPtBin < numCollinearApproxMassDiTauPtBins_; ++iDiTauPtBin ) {
      std::ostringstream iDiTauPtBinString;
      iDiTauPtBinString << std::setfill('0') << std::setw(2) << iDiTauPtBin;
    
      std::string hCollinearApproxMassDiTauPtDepName_i 
	= std::string("CollinearApproxMassDiTauPtDep").append("_").append(iDiTauPtBinString.str());
      hCollinearApproxMassDiTauPtDep_.push_back(book1D(hCollinearApproxMassDiTauPtDepName_i,
						       hCollinearApproxMassDiTauPtDepName_i, 50, 0., 250.));
    }

    for ( unsigned iMEtPtBin = 0; iMEtPtBin < numCollinearApproxMassMEtPtBins_; ++iMEtPtBin ) {
      std::ostringstream iMEtPtBinString;
      iMEtPtBinString << std::setfill('0') << std::setw(2) << iMEtPtBin;
    
      std::string hCollinearApproxMassMEtPtDepName_i 
	= std::string("CollinearApproxMassMEtPtDep").append("_").append(iMEtPtBinString.str());
      hCollinearApproxMassMEtPtDep_.push_back(book1D(hCollinearApproxMassMEtPtDepName_i,
						     hCollinearApproxMassMEtPtDepName_i, 50, 0., 250.));
    }
  }

  bookMEtHistograms();
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::bookMEtHistograms()
{
  hMEtTauNeutrinosParallelZ_ 
    = book1D("MEtTauNeutrinosParallelZ", "P_{T} of Neutrinos produced in #tau-Decays || Z^{0}", 200, -50., +50.);
  hMEtTauNeutrinosPerpendicularZ_ 
    = book1D("MEtTauNeutrinosPerpendicularZ", "P_{T} of Neutrinos produced in #tau-Decays perp. Z^{0}", 200, -50., +50.);
  hMEtNonTauNeutrinosParallelZ_ 
    = book1D("MEtNonTauNeutrinosParallelZ", "P_{T} of Neutrinos not produced in #tau-Decays || Z^{0}", 200, -50., +50.);
  hMEtNonTauNeutrinosPerpendicularZ_ 
    = book1D("MEtNonTauNeutrinosPerpendicularZ", "P_{T} of Neutrinos not produced in #tau-Decays perp. Z^{0}", 200, -50., +50.);
  hMEtLeg1MisMeasParallelZ_ 
    = book1D("MEtLeg1MisMeasParallelZ", "P_{T} due to Mismeasurement of first #tau-Decay product || Z^{0}", 200, -50., +50.);
  hMEtLeg1MisMeasPerpendicularZ_ 
    = book1D("MEtLeg1MisMeasPerpendicularZ", "P_{T} due to Mismeasurement of first #tau-Decay product perp. Z^{0}", 200, -50., +50.);
  hMEtLeg2MisMeasParallelZ_ 
    = book1D("MEtLeg2MisMeasParallelZ", "P_{T} due to Mismeasurement of second #tau-Decay product || Z^{0}", 200, -50., +50.);
  hMEtLeg2MisMeasPerpendicularZ_ 
    = book1D("MEtLeg2MisMeasPerpendicularZ", "P_{T}} due to Mismeasurement of second #tau-Decay product perp. Z^{0}", 200, -50., +50.);
  hMEtJetMisMeasParallelZ_ 
    = book1D("MEtJetMisMeasParallelZ", "P_{T} due to Mismeasurement of quark/gluon Jets || Z^{0}", 200, -50., +50.);
  hMEtJetMisMeasPerpendicularZ_ 
    = book1D("MEtJetMisMeasPerpendicularZ", "P_{T} due to Mismeasurement of quark/gluon Jets perp. Z^{0}", 200, -50., +50.);
  hMEtHighEtaJetsParallelZ_ 
    = book1D("MEtHighEtaJetsParallelZ", "P_{T} due to high |#eta| Jets", 200, -50., +50.);
  hMEtHighEtaJetsPerpendicularZ_ 
    = book1D("MEtHighEtaJetsPerpendicularZ", "P_{T} due to high |#eta| Jets", 200, -50., +50.);
  hMEtUnaccountedParallelZ_ 
    = book1D("MEtUnaccountedParallelZ", "unaccounted missing P_{T} || Z^{0} (should be zero)", 200, -50., +50.);
  hMEtUnaccountedPerpendicularZ_ 
    = book1D("MEtUnaccountedPerpendicularZ", "unaccounted missing P_{T} perp. Z^{0} (should be zero)", 200, -50., +50.);
}

template<typename T1, typename T2>
double CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CompositePtrCandidateT1T2MEtCollinearApproxHistManager::fillHistogramsImp>:" << std::endl; 

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  getCollection(evt, diTauCandidateSrc_, diTauCandidates);

  //std::cout << " diTauCandidates.size = " << diTauCandidates->size() << std::endl;

  double diTauCandidateWeightSum = 0.;
  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {
    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    diTauCandidateWeightSum += getDiTauCandidateWeight(*diTauCandidate);
  }

  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {

    //bool isGenMatched = matchesGenCandidatePair(*diTauCandidate);
    //std::cout << " Pt = " << diTauCandidate->pt() << ", phi = " << diTauCandidate->phi() << ", visMass = " << diTauCandidate->p4Vis().mass() << std::endl;
    //std::cout << " isGenMatched = " << isGenMatched << std::endl;

    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    double diTauCandidateWeight = getDiTauCandidateWeight(*diTauCandidate);
    double weight = getWeight(evtWeight, diTauCandidateWeight, diTauCandidateWeightSum);

    if ( diTauCandidate->p4Leg1gen().energy() > epsilon && 
	 diTauCandidate->p4Leg2gen().energy() > epsilon ) {
      hGenX1_->Fill(diTauCandidate->x1gen(), weight);
      hGenX2_->Fill(diTauCandidate->x2gen(), weight);

      hX1_->Fill(diTauCandidate->x1CollinearApprox(), weight);
      hX2_->Fill(diTauCandidate->x2CollinearApprox(), weight);

      if ( makeCollinearApproxX1X2histograms_ ) {
	hGenX1vsX2_->Fill(diTauCandidate->x1gen(), diTauCandidate->x2gen(), weight);
	
	hX1vsGenX1_->Fill(diTauCandidate->x1gen(), diTauCandidate->x1CollinearApprox(), weight);
	hX1vsGenX1Profile_->getTProfile()->Fill(diTauCandidate->x1gen(), diTauCandidate->x1CollinearApprox(), weight);
	hX2vsGenX2_->Fill(diTauCandidate->x2gen(), diTauCandidate->x2CollinearApprox(), weight);
	hX2vsGenX2Profile_->getTProfile()->Fill(diTauCandidate->x2gen(), diTauCandidate->x2CollinearApprox(), weight);
      }

      if ( diTauCandidate->met()->pt() > 0. && dynamic_cast<const pat::MET*>(diTauCandidate->met().get()) ) {
	const pat::MET* recoMEt = dynamic_cast<const pat::MET*>(diTauCandidate->met().get());
        if ( recoMEt->genMET() ) {
	  double genMEtPt = recoMEt->genMET()->pt();
	  double genMEtPx = recoMEt->genMET()->px();
	  double genMEtPy = recoMEt->genMET()->py();

	  double genLeg1ProjGenMEt = (diTauCandidate->p4VisLeg1gen().px()*genMEtPx 
                                    + diTauCandidate->p4VisLeg1gen().py()*genMEtPy)/genMEtPt;
	  double genLeg2ProjGenMEt = (diTauCandidate->p4VisLeg2gen().px()*genMEtPx 
                                    + diTauCandidate->p4VisLeg2gen().py()*genMEtPy)/genMEtPt;

	  hGenLeg1ProjGenMEt_->Fill(genLeg1ProjGenMEt, weight);
	  hGenLeg2ProjGenMEt_->Fill(genLeg2ProjGenMEt, weight);
	}
      }
    }

    fillWeightHistograms(hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_, 
			 hDiTauCandidateWeightLinear_, diTauCandidateWeight);

    hCollinearApproxEffDPhi12dep_->Fill(diTauCandidate->dPhi12(), weight);
    hCollinearApproxEffDiTauPtDep_->Fill(diTauCandidate->p4Vis().pt(), weight);
    hCollinearApproxEffMEtDep_->Fill(diTauCandidate->met()->pt(), weight);

    double genMass = diTauCandidate->p4gen().mass();
    double recMass = diTauCandidate->p4CollinearApprox().mass();
    hCollinearApproxEta_->Fill(diTauCandidate->p4CollinearApprox().eta(), weight);
    hCollinearApproxMass_->Fill(recMass, weight);
    if ( genMass > 0. ) hCollinearApproxMassRes_->Fill((recMass - genMass)/genMass, weight);
    hCollinearApproxMassVsPt_->Fill(diTauCandidate->p4Vis().pt(), recMass, weight);
    hCollinearApproxMassVsDPhi12_->Fill(diTauCandidate->dPhi12(), recMass, weight);
    hCollinearApproxX1_->Fill(diTauCandidate->x1CollinearApprox(), weight);
    hCollinearApproxX2_->Fill(diTauCandidate->x2CollinearApprox(), weight);
    if ( makeCollinearApproxX1X2histograms_ ) {
      hCollinearApproxX1vsX2_->Fill(diTauCandidate->x1CollinearApprox(), diTauCandidate->x2CollinearApprox(), weight);
    }

    if ( makeCollinearApproxMassDepHistograms_  ) {
      int iDPhi12bin = TMath::FloorNint(diTauCandidate->dPhi12()/collinearApproxMassDPhi12Incr_);
      if ( iDPhi12bin >= 0 && iDPhi12bin < (int)hCollinearApproxMassDPhi12dep_.size() )
	hCollinearApproxMassDPhi12dep_[iDPhi12bin]->Fill(recMass, weight);
      
      int iDiTauPtBin = TMath::FloorNint(diTauCandidate->p4Vis().pt()/collinearApproxMassDiTauPtIncr_);
      if ( iDiTauPtBin >= 0 && iDiTauPtBin < (int)hCollinearApproxMassDiTauPtDep_.size() )
	hCollinearApproxMassDiTauPtDep_[iDiTauPtBin]->Fill(recMass, weight);
      
      int iMEtPtBin = TMath::FloorNint(diTauCandidate->met()->pt()/collinearApproxMassMEtPtIncr_);
      if ( iMEtPtBin >= 0 && iMEtPtBin < (int)hCollinearApproxMassMEtPtDep_.size() )
	hCollinearApproxMassMEtPtDep_[iMEtPtBin]->Fill(recMass, weight);
    }

    fillMEtHistograms(evt, *diTauCandidate, weight);
  }
}

void fillHistogram(MonitorElement* hParallelZ, MonitorElement* hPerpendicularZ, 
		   double px, double py, reco::Candidate::LorentzVector& p4Zgen, double weight)
{
  if ( p4Zgen.pt() != 0. ) {
    double ptParallelZ = (px*p4Zgen.px() + py*p4Zgen.py())/p4Zgen.pt();
    hParallelZ->Fill(ptParallelZ, weight);

    double ptPerpendicularZ = TMath::Abs(px*p4Zgen.py() + py*p4Zgen.px())/p4Zgen.pt();
    hPerpendicularZ->Fill(ptPerpendicularZ, weight);
  }
}

void fillHistogram(MonitorElement* hParallelZ, MonitorElement* hPerpendicularZ, 
		   reco::Candidate::LorentzVector& p4, reco::Candidate::LorentzVector& p4Zgen, double weight)
{
  fillHistogram(hParallelZ, hPerpendicularZ, p4.px(), p4.py(), p4Zgen, weight);
}
		   
template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtCollinearApproxHistManager<T1,T2>::fillMEtHistograms(const edm::Event& evt, const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate, double weight)
{
  edm::Handle<reco::GenParticleCollection> genParticles;
  if ( genParticleSrc_.label() != "" ) evt.getByLabel(genParticleSrc_, genParticles);

  edm::Handle<edm::View<reco::Jet> > recoJets;
  evt.getByLabel(recoJetSrc_, recoJets);
      
  edm::Handle<reco::GenJetCollection> genJets;
  if ( genJetSrc_.label() != "" ) evt.getByLabel(genJetSrc_, genJets);

  if ( genParticles.isValid() && genJets.isValid() ) {
    const reco::GenParticle* genLeg1 = findGenParticle(diTauCandidate.leg1()->p4(), *genParticles, 0.5, -1);
    const reco::GenParticle* genLeg2 = findGenParticle(diTauCandidate.leg2()->p4(), *genParticles, 0.5, -1);
    
    if ( genLeg1 && genLeg2 ) {
      reco::Candidate::LorentzVector p4Zgen = genLeg1->p4() + genLeg2->p4();
      
      reco::Candidate::LorentzVector p4InvisLeg1gen = getInvisMomentum(genLeg1);
      reco::Candidate::LorentzVector p4InvisLeg2gen = getInvisMomentum(genLeg2);
      
      reco::Candidate::LorentzVector p4GenTauNeutrinos = p4InvisLeg1gen + p4InvisLeg2gen;
      
      fillHistogram(hMEtTauNeutrinosParallelZ_, hMEtTauNeutrinosPerpendicularZ_, p4GenTauNeutrinos, p4Zgen, weight);
      
      reco::Candidate::LorentzVector p4GenNeutrinos(0,0,0,0);
      for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin(); 
	    genParticle != genParticles->end(); ++genParticle ) {
	
	if ( genParticle->status() == 1 && isNeutrino(&(*genParticle)) )p4GenNeutrinos += genParticle->p4();
      }
      
      reco::Candidate::LorentzVector p4GenNonTauNeutrinos = p4GenNeutrinos - p4GenTauNeutrinos;
      
      fillHistogram(hMEtNonTauNeutrinosParallelZ_, hMEtNonTauNeutrinosPerpendicularZ_, p4GenNonTauNeutrinos, p4Zgen, weight);
      
      reco::Candidate::LorentzVector p4VisLeg1gen = getVisMomentum(genLeg1);
      reco::Candidate::LorentzVector p4MisMeasLeg1 = p4VisLeg1gen - diTauCandidate.leg1()->p4();
      fillHistogram(hMEtLeg1MisMeasParallelZ_, hMEtLeg1MisMeasPerpendicularZ_, p4MisMeasLeg1, p4Zgen, weight);
      
      reco::Candidate::LorentzVector p4VisLeg2gen = getVisMomentum(genLeg2);
      reco::Candidate::LorentzVector p4MisMeasLeg2 = p4VisLeg2gen - diTauCandidate.leg2()->p4();
      fillHistogram(hMEtLeg2MisMeasParallelZ_, hMEtLeg2MisMeasPerpendicularZ_, p4MisMeasLeg2, p4Zgen, weight);

      std::vector<const reco::Jet*> recoJets_matched;    

      reco::Candidate::LorentzVector p4GenHighEtaJets(0,0,0,0);
      
      double pxMisMeasJets = 0;
      double pyMisMeasJets = 0;
      
      for ( reco::GenJetCollection::const_iterator genJet = genJets->begin();
	    genJet != genJets->end(); ++genJet ) {
//--- CV: genJet includes neutrinos,
//        so cannot simply take genJet four-vector, but need to compute visible momentum
	reco::Candidate::LorentzVector genJetVisMomentum = getVisMomentum(genJet->getGenConstituents(), 1);

//--- skip jets outside of geometric acceptance of the ECAL/HCAL (barrel, endcap or HF) calorimeters
	const double maxEtaAcceptance = 5.0;
	if ( TMath::Abs(genJetVisMomentum.eta()) > maxEtaAcceptance ) {
	  p4GenHighEtaJets += genJet->p4();
	  continue;
	}
    
//--- skip jets coinciding with tau decay products
	if ( reco::deltaR(diTauCandidate.leg1()->p4(), genJetVisMomentum) < 0.5 || 
	     reco::deltaR(diTauCandidate.leg2()->p4(), genJetVisMomentum) ) continue;

	const reco::Jet* recoJet_matched = 0;
	double dRmin = 1.e+3;
	
	for ( edm::View<reco::Jet>::const_iterator recoJet = recoJets->begin(); 
	      recoJet != recoJets->end(); ++recoJet ) {
	  double dR = reco::deltaR(genJetVisMomentum, recoJet->p4());
	  if ( dR < 0.5 && dR < dRmin ) {
	    recoJet_matched = &(*recoJet);
	    dRmin = dR;
	  }
	}
	
	double dPx = ( recoJet_matched ) ? genJetVisMomentum.px() - recoJet_matched->px() : genJetVisMomentum.px();
	double dPy = ( recoJet_matched ) ? genJetVisMomentum.py() - recoJet_matched->py() : genJetVisMomentum.py();
	
	pxMisMeasJets += dPx;
	pyMisMeasJets += dPy;
	
	recoJets_matched.push_back(recoJet_matched);
      }
      
      for ( edm::View<reco::Jet>::const_iterator recoJet = recoJets->begin(); 
	    recoJet != recoJets->end(); ++recoJet ) {
	
	bool isMatched = false;
	
	for ( std::vector<const reco::Jet*>::const_iterator recoJet_matched = recoJets_matched.begin(); 
	      recoJet_matched != recoJets_matched.end(); ++recoJet_matched ) {
	  if ( &(*recoJet) == (*recoJet_matched) ) isMatched = true;
	}
	
	if ( !isMatched ) {
	  pxMisMeasJets -= recoJet->px();
	  pyMisMeasJets -= recoJet->py();
	}
      }
      
      fillHistogram(hMEtJetMisMeasParallelZ_, hMEtJetMisMeasPerpendicularZ_, pxMisMeasJets, pyMisMeasJets, p4Zgen, weight);
      
      fillHistogram(hMEtHighEtaJetsParallelZ_, hMEtHighEtaJetsPerpendicularZ_, p4GenHighEtaJets, p4Zgen, weight);
     	  
      reco::Candidate::LorentzVector recoMEt = diTauCandidate.met()->p4();
      
      double pxUnaccountedMEt = recoMEt.px() - (p4GenTauNeutrinos.px() + p4GenNonTauNeutrinos.px() 
					      + p4MisMeasLeg1.px() + p4MisMeasLeg2.px() + pxMisMeasJets + p4GenHighEtaJets.px());
      double pyUnaccountedMEt = recoMEt.py() - (p4GenTauNeutrinos.py() + p4GenNonTauNeutrinos.py() 
					      + p4MisMeasLeg1.py() + p4MisMeasLeg2.py() + pyMisMeasJets + p4GenHighEtaJets.py());
      
      fillHistogram(hMEtUnaccountedParallelZ_, hMEtUnaccountedPerpendicularZ_, pxUnaccountedMEt, pyUnaccountedMEt, p4Zgen, weight);
    } else {
      edm::LogWarning ("fillMEtHistograms") << " Failed to find generated tau-leptons !!";
    }
  }
}

void fillPullHistogram(MonitorElement* h, double x, double xGen, double xErr, double weight)
{
  if ( xErr > 0. ) {
    double xPull = (x - xGen)/xErr;
    h->Fill(xPull, weight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<reco::Candidate, reco::Candidate> DiCandidatePairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Electron, pat::Tau> PATElecTauPairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Muon, pat::Tau> PATMuTauPairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Tau, pat::Tau> PATDiTauPairCollinearApproxHistManager;
typedef CompositePtrCandidateT1T2MEtCollinearApproxHistManager<pat::Electron, pat::Muon> PATElecMuPairCollinearApproxHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, DiCandidatePairCollinearApproxHistManager, "DiCandidatePairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, DiCandidatePairCollinearApproxHistManager, "DiCandidatePairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairCollinearApproxHistManager, "PATElecTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairCollinearApproxHistManager, "PATElecTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairCollinearApproxHistManager, "PATMuTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairCollinearApproxHistManager, "PATMuTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairCollinearApproxHistManager, "PATDiTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairCollinearApproxHistManager, "PATDiTauPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairCollinearApproxHistManager, "PATElecMuPairCollinearApproxHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairCollinearApproxHistManager, "PATElecMuPairCollinearApproxHistManager");
  
#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<DiCandidatePairCollinearApproxHistManager> DiCandidatePairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATElecTauPairCollinearApproxHistManager> PATElecTauPairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATMuTauPairCollinearApproxHistManager> PATMuTauPairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATDiTauPairCollinearApproxHistManager> PATDiTauPairCollinearApproxAnalyzer;
typedef HistManagerAdapter<PATElecMuPairCollinearApproxHistManager> PATElecMuPairCollinearApproxAnalyzer;

DEFINE_FWK_MODULE(DiCandidatePairCollinearApproxAnalyzer);
DEFINE_FWK_MODULE(PATElecTauPairCollinearApproxAnalyzer);
DEFINE_FWK_MODULE(PATMuTauPairCollinearApproxAnalyzer);
DEFINE_FWK_MODULE(PATDiTauPairCollinearApproxAnalyzer);
DEFINE_FWK_MODULE(PATElecMuPairCollinearApproxAnalyzer);

