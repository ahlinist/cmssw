#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/angle.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>
#include <TFile.h>

const double epsilon = 0.01;
const double speedOfLight = 3.e-5; // speed of light [cm/fs]
const double tauLeptonMass = 1.78; // tau lepton mass [GeV]

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
CompositePtrCandidateT1T2MEtHistManager<T1,T2>::CompositePtrCandidateT1T2MEtHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg),
    lutSqrtX1X2VsDPhi12_(0)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtHistManager::CompositePtrCandidateT1T2MEtHistManager>:" << std::endl;

  diTauCandidateSrc_ = cfg.getParameter<edm::InputTag>("diTauCandidateSource");
  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_ << std::endl;

  genParticleSrc_ = cfg.getParameter<edm::InputTag>("genParticleSource");
  //std::cout << " genParticleSrc = " << genParticleSrc_ << std::endl;

  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;

  visMassHypothesisSrc_ = ( cfg.exists("visMassHypothesisSource") ) ?  
    cfg.getParameter<edm::InputTag>("visMassHypothesisSource") : edm::InputTag();
  //std::cout << " visMassHypothesisSrc = " << visMassHypothesisSrc_ << std::endl;

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");

  if ( cfg.exists("fileName_sqrtX1X2VsDPhi12") && cfg.exists("meName_sqrtX1X2VsDPhi12") ) {
    std::string fileName_sqrtX1X2VsDPhi12 = cfg.getParameter<std::string>("fileName_sqrtX1X2VsDPhi12");
    std::string meName_sqrtX1X2VsDPhi12 = cfg.getParameter<std::string>("meName_sqrtX1X2VsDPhi12");

    TFile* file_sqrtX1X2VsDPhi12 = TFile::Open(fileName_sqrtX1X2VsDPhi12.data());
    if ( !file_sqrtX1X2VsDPhi12->IsZombie() ) {
      TObject* obj = file_sqrtX1X2VsDPhi12->Get(meName_sqrtX1X2VsDPhi12.data());
      if ( obj ) lutSqrtX1X2VsDPhi12_ = dynamic_cast<TH1*>(obj->Clone());
    } else {
      edm::LogError ("CompositePtrCandidateT1T2MEtHistManager") 
	<< " Failed to open inputFile = " << fileName_sqrtX1X2VsDPhi12
	<< "--> sqrtX1X2VsDPhi12 LUT histogram will NOT be loaded !!";
    }
    
    delete file_sqrtX1X2VsDPhi12;
  }
  
  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtHistManager<T1,T2>::~CompositePtrCandidateT1T2MEtHistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T1>*>::iterator it = diTauLeg1WeightExtractors_.begin();
	it != diTauLeg1WeightExtractors_.end(); ++it ) {
    delete (*it);
  }

  for ( typename std::vector<FakeRateJetWeightExtractor<T2>*>::iterator it = diTauLeg2WeightExtractors_.begin();
	it != diTauLeg2WeightExtractors_.end(); ++it ) {
    delete (*it);
  }

  delete lutSqrtX1X2VsDPhi12_;
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtHistManager::bookHistogramsImp>:" << std::endl;

  hGenDiTauCandidatePt_ = book1D("GenDiTauCandidatePt", "gen. Composite P_{T}", 75, 0., 150.);

  hGenLeg1En_ = book1D("GenLeg1En", "gen. leg_{1} Energy", 125, 0., 250.);
  hGenLeg2En_ = book1D("GenLeg2En", "gen. leg_{2} Energy", 125, 0., 250.);
  hGenLeg1PtVsLeg2Pt_ = book2D("GenLeg1PtVsLeg2Pt", "gen. leg_{1} P_{T} vs. leg_{2} P_{T}", 20, 0., 100., 20, 0., 100.);
  hGenLeg1EtaVsLeg2Eta_ = book2D("GenLeg1EtaVsLeg2Eta", "gen. leg_{1} #eta vs. leg_{2} #eta", 20, -2.5, 2.5, 20, -2.5, 2.5);
  hGenDeltaRleg1VisNu_ = book1D("GenDeltaRleg1VisNu", "gen. leg_{1} dR(pVis, pNu)", 102, -0.01, 1.01);
  hGenDeltaRleg2VisNu_ = book1D("GenDeltaRleg2VisNu", "gen. leg_{2} dR(pVis, pNu)", 102, -0.01, 1.01);
  hGenLeg1DecayTime_ = book1D("GenLeg1DecayTime", "gen. leg_{1} Decay eigentime", 100, 0., 1000.);
  hGenLeg2DecayTime_ = book1D("GenLeg2DecayTime", "gen. leg_{2} Decay eigentime", 100, 0., 1000.);

  hGenSqrtX1X2VsDPhi12_ = bookProfile1D("GenSqrtX1X2VsDPhi12", 
					"gen. sqrt(X_{1} * X_{2}) vs. #Delta#phi_{1,2}", 18, -epsilon, TMath::Pi() + epsilon);
  hSqrtX1X2VsDPhi12Mass_ = book1D("SqrtX1X2VsDPhi12Mass", "sqrt(X_{1} * X_{2}) vs. #Delta#phi_{1,2} LUT Mass", 50, 0., 250.);

  hGenLeg1TauPlusDecayAngleLepton_ = book1D("GenLeg1TauPlusDecayAngleLepton", 
					    "gen. leg_{1} #theta(#tau, #ell) for lep. Tau+ decays", 36, 0., TMath::Pi());
  hGenLeg1TauPlusDecayAngleOneProng_ = book1D("GenLeg1TauPlusDecayAngleOneProng", 
					      "gen. leg_{1} #theta(#tau, #nu) for had. one-prong Tau+ decays", 36, 0., TMath::Pi());
  hGenLeg1TauPlusDecayAngleThreeProng_ = book1D("GenLeg1TauPlusDecayAngleThreeProng", 
					      "gen. leg_{1} #theta(#tau, #nu) for had. three-prong Tau+ decays", 36, 0., TMath::Pi());
  hGenLeg1TauMinusDecayAngleLepton_ = book1D("GenLeg1TauMinusDecayAngleLepton", 
					     "gen. leg_{1} #theta(#tau, #ell) for lep. Tau- decays", 36, 0., TMath::Pi());
  hGenLeg1TauMinusDecayAngleOneProng_ = book1D("GenLeg1TauMinusDecayAngleOneProng", 
					       "gen. leg_{1} #theta(#tau, #nu) for had. one-prong Tau- decays", 36, 0., TMath::Pi());
  hGenLeg1TauMinusDecayAngleThreeProng_ = book1D("GenLeg1TauMinusDecayAngleThreeProng", 
						 "gen. leg_{1} #theta(#tau, #nu) for had. three-prong Tau- decays", 36, 0., TMath::Pi());
  hGenLeg2TauPlusDecayAngleLepton_ = book1D("GenLeg2TauPlusDecayAngleLepton", 
					    "gen. leg_{2} #theta(#tau, #ell) for lep. Tau+ decays", 36, 0., TMath::Pi());
  hGenLeg2TauPlusDecayAngleOneProng_ = book1D("GenLeg2TauPlusDecayAngleOneProng", 
					      "gen. leg_{2} #theta(#tau, #nu) for had. one-prong Tau+ decays", 36, 0., TMath::Pi());
  hGenLeg2TauPlusDecayAngleThreeProng_ = book1D("GenLeg2TauPlusDecayAngleThreeProng", 
					      "gen. leg_{2} #theta(#tau, #nu) for had. three-prong Tau+ decays", 36, 0., TMath::Pi());
  hGenLeg2TauMinusDecayAngleLepton_ = book1D("GenLeg2TauMinusDecayAngleLepton", 
					     "gen. leg_{2} #theta(#tau, #ell) for lep. Tau- decays", 36, 0., TMath::Pi());
  hGenLeg2TauMinusDecayAngleOneProng_ = book1D("GenLeg2TauMinusDecayAngleOneProng", 
					       "gen. leg_{2} #theta(#tau, #nu) for had. one-prong Tau- decays", 36, 0., TMath::Pi());
  hGenLeg2TauMinusDecayAngleThreeProng_ = book1D("GenLeg2TauMinusDecayAngleThreeProng", 
						 "gen. leg_{2} #theta(#tau, #nu) for had. three-prong Tau- decays", 36, 0., TMath::Pi());
  
  hDiTauCandidatePt_ = book1D("DiTauCandidatePt", "Composite P_{T}", 75, 0., 150.);
  hDiTauCandidateEta_ = book1D("DiTauCandidateEta", "Composite #eta", 100, -5., +5.);
  hDiTauCandidatePhi_ = book1D("DiTauCandidatePhi", "Composite #phi", 36, -TMath::Pi(), +TMath::Pi());
  hDiTauCandidateCharge_ = book1D("DiTauCandidateCharge", "Composite Charge", 11, -5.5, +5.5);
  hDiTauCandidateMass_ = book1D("DiTauCandidateMass", "Composite Invariant Mass", 50, 0., 250.);

  hSVfitMass_ = book1D("SVfitMass", "SVfit Mass", 50, 0., 250.);
  hSVfitMass1stSolution_ = book1D("SVfitMass1stSolution", "SVfit Mass (1st Solution)", 50, 0., 250.);
  hSVfitMass2ndSolution_ = book1D("SVfitMass2ndSolution", "SVfit Mass (2nd Solution)", 50, 0., 250.);
  hSVfitMass3rdSolution_ = book1D("SVfitMass3rdSolution", "SVfit Mass (3rd Solution)", 50, 0., 250.);
  hSVfitMass4thSolution_ = book1D("SVfitMass4thSolution", "SVfit Mass (4th Solution)", 50, 0., 250.);  
  hSVfitMassBestMatch_ = book1D("SVfitMassBestMatch", "SVfit Mass best matching gen. Mass", 50, 0., 250.);
  hSVfitMassAverage_ = book1D("SVfitMassAverage", "SVfit Mass (Average of valid Solutions)", 50, 0., 250.);
  hSVfitMassNumSolutionsAveraged_ = book1D("SVfitMassNumSolutionsAveraged", "SVfit Num. Mass Solutions incl. in Average", 5, -0.5, 4.5);
  hSVfitMassVsLogLikelihood_ = book2D("SVfitMassVsLogLikelihood", "SVfit Mass vs. log-Likelihood", 50, 0., 250., 20, -35., 25.);
  hSVfitLogLikelihood_ = book1D("SVfitLogLikelihood", "SVfit log-Likelihood", 100, -50., 50.);
  hSVfitDecayTimeLeg1_ = book1D("SVfitDecayTimeLeg1", "SVfit leg_{1} Decay eigentime", 100, 0., 1000.);
  hSVfitDecayTimeLeg2_ = book1D("SVfitDecayTimeLeg2", "SVfit leg_{2} Decay eigentime", 100, 0., 1000.);
  hSVfitStatus_ = book1D("SVfitStatus", "SVfit Status", 10, -2.5, 7.5);
  
  hLeg1PtVsLeg2Pt_ = book2D("Leg1PtVsLeg2Pt", "leg_{1} P_{T} vs. leg_{2} P_{T}", 20, 0., 100., 20, 0., 100.);
  hLeg1EtaVsLeg2Eta_ = book2D("Leg1EtaVsLeg2Eta", "leg_{1} #eta vs. leg_{2} #eta", 20, -2.5, 2.5, 20, -2.5, 2.5);
  
  bookWeightHistograms(*dqmStore_, "DiTauCandidateWeight", "Composite Weight", 
		       hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_, 
		       hDiTauCandidateWeightLinear_);

  hDiTauCandidateImpParSig_ = book1D("DiTauCandidateImpParSig", "#sqrt{#frac{dxy_{1}}{#Delta dxy_{1}}^{2}+#frac{dxy_{2}}{#Delta dxy_{2}}^{2}}", 30, 0., 15.);
  
  hVisPt_ = book1D("VisPt", "Visible P_{T}", 50, 0., 100.);
  hVisPhi_ = book1D("VisPhi", "Visible #phi", 36, -TMath::Pi(), +TMath::Pi());
  hVisMass_ = book1D("VisMass", "Visible Mass", 40, 0., 200.);
  hVisMassZllCombinedHypothesis_ = ( visMassHypothesisSrc_.label() != "" ) ?
    book1D("VisMassZllCombinedHypothesis", "Visible Mass (combined Value of different Event Hypotheses)", 40, 0., 200.) : 0;
  
  hCollinearApproxEta_ = book1D("CollinearApproxEta", "Collinear Approximation #eta", 100, -5., +5.);
  hCollinearApproxMass_ = book1D("CollinearApproxMass", "Collinear Approximation Mass", 50, 0., 250.);
  hCollinearApproxX1_ = book1D("CollinearApproxX1", "Collinear Approximation X_{1}", 100, -2.5, +2.5);
  hCollinearApproxX2_ = book1D("CollinearApproxX2", "Collinear Approximation X_{2}", 100, -2.5, +2.5);

  hCDFmethodMass_ = book1D("CDFmethodMass", "CDF Method Mass", 50, 0., 250.);
  
  hMt12MET_ = book1D("Mt12MET", "Mass_{T 1,2,MET}", 50, 0., 250.);
  
  hMt1MET_ = book1D("Mt1MET", "Mass_{T 1,MET}", 40, 0., 200.);
  hMt2MET_ = book1D("Mt2MET", "Mass_{T 2,MET}", 40, 0., 200.);
  
  hDPhi12_ = book1D("DPhi12", "#Delta#phi_{1,2}", 36, -epsilon, TMath::Pi() + epsilon);
  hDR12_ = book1D("DR12", "#Delta R_{1,2}", 51, -0.1, 10.1);
  
  hVisEtaMin_ = book1D("VisEtaMin", "VisEtaMin", 60, -3., +3.);
  hVisEtaMax_ = book1D("VisEtaMax", "VisEtaMax", 60, -3., +3.);
  
  hDPhi1MET_ = book1D("DPhi1MET", "#Delta#phi_{1,MET}", 36, -epsilon, TMath::Pi() + epsilon);
  hDPhi2MET_ = book1D("DPhi2MET", "#Delta#phi_{2,MET}", 36, -epsilon, TMath::Pi() + epsilon);
  hDPhi1METvsDPhi2MET_ = book2D("DPhi1METvsDPhi2MET", "#Delta#phi_{1,MET} vs. #Delta#phi_{2,MET}", 18, -epsilon, TMath::Pi() + epsilon, 18, -epsilon, TMath::Pi() + epsilon); 

  hPzetaCorr_ = book2D("PzetaCorr", "P_{#zeta} vs. P_{#zeta}^{vis}", 10, 0., 50., 14, -20., 50.);
  hPzetaDiff_ = book1D("PzetaDiff", "P_{#zeta} - 1.5*P_{#zeta}^{vis}", 40, -100., +100.);
}

template<typename T1, typename T2>
double CompositePtrCandidateT1T2MEtHistManager<T1,T2>::getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

void fillDeltaRvisNuHistogram(MonitorElement* h, 
			      const reco::Candidate::LorentzVector& p4, const reco::Candidate::LorentzVector& p4Vis, double weight)
{
  reco::Candidate::LorentzVector p4Nu = p4 - p4Vis;
  double dR = deltaR(p4Vis, p4Nu);
  h->Fill(TMath::Min(dR, 1.), weight);
}

double compDecayEigenTime(const reco::Candidate::Point& primaryVertexPos, const reco::Candidate::Point& decayVertexPos, 
			  double tauLeptonEnergy)
{
  double decayDistance = TMath::Sqrt((decayVertexPos - primaryVertexPos).Mag2());
  double gamma = tauLeptonEnergy/tauLeptonMass;
  return decayDistance/(speedOfLight*gamma);
}

std::string getGenTauDecayMode(const std::vector<const reco::GenParticle*>& genTauDecayProducts) 
{  
//--- determine generator level tau decay mode
//
//    NOTE: function implements logic defined in PhysicsTools/JetMCUtils/src/JetMCTag::genTauDecayMode
//          for different type of argument
//

  int numElectrons = 0;
  int numMuons = 0;
  int numChargedHadrons = 0;
  int numNeutralHadrons = 0;
  int numPhotons = 0;
  
  for ( std::vector<const reco::GenParticle*>::const_iterator genTauDecayProduct = genTauDecayProducts.begin();
	genTauDecayProduct != genTauDecayProducts.end(); ++genTauDecayProduct ) {
    
    int pdgId = abs((*genTauDecayProduct)->pdgId());
    
    switch ( pdgId ) {
    case 22: 
      numPhotons++;
      break;
    case 11:
      numElectrons++;
      break;
    case 13:
      numMuons++;
      break;
    default : 
      if ( (*genTauDecayProduct)->charge() != 0 ) {
	numChargedHadrons++; 
      } else {
	numNeutralHadrons++; 
      }
    }
  }
  
  if ( numElectrons == 1 ) {
    return std::string("electron");
  } else if ( numMuons == 1 ) {
    return std::string("muon");
  } else {
    switch ( numChargedHadrons ) {
    case 1 : 
      if ( numNeutralHadrons != 0 ) return std::string("oneProngOther");
      switch ( numPhotons ) {
      case 0:
	return std::string("oneProng0Pi0");
      case 2:
	return std::string("oneProng1Pi0");
      case 4:
	return std::string("oneProng2Pi0");
      default:
	return std::string("oneProngOther");
      }
    case 3 : 
      if ( numNeutralHadrons != 0 ) return std::string("threeProngOther");
      switch ( numPhotons ) {
      case 0:
	return std::string("threeProng0Pi0");
      case 2:
	return std::string("threeProng1Pi0");
      default:
	return std::string("threeProngOther");
      }
    default:
      return std::string("rare");
    }
  }
}

void fillGenTauHistograms(MonitorElement* hGenTauPlusDecayAngleLepton, 
			  MonitorElement* hGenTauPlusDecayAngleOneProng, 
			  MonitorElement* hGenTauPlusDecayAngleThreeProng,
			  MonitorElement* hGenTauMinusDecayAngleLepton, 
			  MonitorElement* hGenTauMinusDecayAngleOneProng, 
			  MonitorElement* hGenTauMinusDecayAngleThreeProng,
			  const reco::Candidate::LorentzVector& genTauMomentum, 
			  const reco::GenParticleCollection& genParticles,
			  double weight)
{
  //std::cout << "<fillGenTauHistograms>:" << std::endl;

  const reco::GenParticle* genTau = findGenParticle(genTauMomentum, genParticles);
  //std::cout << " genTau = " << genTau << std::endl;

  if ( genTau ) {
    std::vector<const reco::GenParticle*> genTauDecayProducts;
    findDaughters(genTau, genTauDecayProducts);

    reco::Candidate::LorentzVector genTauVisMomentum = getVisMomentum(genTauDecayProducts);
    reco::Candidate::LorentzVector genTauInvisMomentum = getInvisMomentum(genTauDecayProducts);
    
    reco::Candidate::LorentzVector genTauVisMomentum_restframe = boostToRestFrame(genTauVisMomentum, genTauMomentum);
    reco::Candidate::LorentzVector genTauInvisMomentum_restframe = boostToRestFrame(genTauInvisMomentum, genTauMomentum);

    std::string genTauDecayMode = getGenTauDecayMode(genTauDecayProducts);
    //std::cout << " genTauDecayMode = " << genTauDecayMode << std::endl;

    if ( genTauDecayMode == "electron" || 
	 genTauDecayMode == "muon" ) {	  
      double genTauVisDecayAngle = angle(genTauVisMomentum_restframe, genTau->p4());
      if ( genTau->pdgId() == -15 ) hGenTauPlusDecayAngleLepton->Fill(genTauVisDecayAngle, weight);
      if ( genTau->pdgId() == +15 ) hGenTauMinusDecayAngleLepton->Fill(genTauVisDecayAngle, weight);
    }
	
    if ( genTauDecayMode == "oneProng0Pi0" ||
	 genTauDecayMode == "oneProng1Pi0" ||
	 genTauDecayMode == "oneProng2Pi0" ) {
      double genTauInvisDecayAngle = angle(genTauInvisMomentum_restframe, genTau->p4());
      if ( genTau->pdgId() == -15 ) hGenTauPlusDecayAngleOneProng->Fill(genTauInvisDecayAngle, weight);
      if ( genTau->pdgId() == +15 ) hGenTauMinusDecayAngleOneProng->Fill(genTauInvisDecayAngle, weight);
    }
	
    if ( genTauDecayMode == "threeProng0Pi0" ||
	 genTauDecayMode == "threeProng1Pi0" ) {
      double genTauInvisDecayAngle = angle(genTauInvisMomentum_restframe, genTau->p4());
      if ( genTau->pdgId() == -15 ) hGenTauPlusDecayAngleThreeProng->Fill(genTauInvisDecayAngle, weight);
      if ( genTau->pdgId() == +15 ) hGenTauMinusDecayAngleThreeProng->Fill(genTauInvisDecayAngle, weight);
    }
  }
}

void fillSVmassRecoSolutionHistogram(unsigned iSolution, MonitorElement* h, 
				     const std::vector<SVmassRecoSolution>& svFitSolutions, double weight)
{
  if ( iSolution >= 0 && iSolution < svFitSolutions.size() ) {
    if ( svFitSolutions[iSolution].isValidSolution() ) h->Fill(svFitSolutions[iSolution].p4().mass(), weight);
  }
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CompositePtrCandidateT1T2MEtHistManager::fillHistogramsImp>:" << std::endl; 

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  getCollection(evt, diTauCandidateSrc_, diTauCandidates);

  //std::cout << " diTauCandidates.size = " << diTauCandidates->size() << std::endl;

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(genParticleSrc_, genParticles);

  edm::Handle<std::vector<reco::Vertex> > recoVertices;
  evt.getByLabel(vertexSrc_, recoVertices);

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
      hGenDiTauCandidatePt_->Fill(diTauCandidate->p4VisGen().pt(), weight);

      hGenLeg1En_->Fill(diTauCandidate->p4Leg1gen().energy(), weight);
      hGenLeg2En_->Fill(diTauCandidate->p4Leg2gen().energy(), weight);

      hGenLeg1PtVsLeg2Pt_->Fill(diTauCandidate->p4VisLeg1gen().pt(), diTauCandidate->p4VisLeg2gen().pt(), weight);
      hGenLeg1EtaVsLeg2Eta_->Fill(diTauCandidate->p4VisLeg1gen().eta(), diTauCandidate->p4VisLeg2gen().eta(), weight);

      fillDeltaRvisNuHistogram(hGenDeltaRleg1VisNu_, diTauCandidate->p4Leg1gen(), diTauCandidate->p4VisLeg1gen(), weight);
      fillDeltaRvisNuHistogram(hGenDeltaRleg2VisNu_, diTauCandidate->p4Leg2gen(), diTauCandidate->p4VisLeg2gen(), weight);

      double dPhi12gen = TMath::Abs(normalizedPhi(diTauCandidate->p4VisLeg1gen().phi() - diTauCandidate->p4VisLeg2gen().phi()));
/*
  
  CV: temporary work-around until MonitorElement::Fill(double, double, double) is fixed for TProfiles

      hGenSqrtX1X2VsDPhi12_->Fill(dPhi12gen, TMath::Sqrt(diTauCandidate->x1gen()*diTauCandidate->x2gen()), weight);
 */
      hGenSqrtX1X2VsDPhi12_->getTProfile()->Fill(dPhi12gen, TMath::Sqrt(diTauCandidate->x1gen()*diTauCandidate->x2gen()), weight);
      if ( lutSqrtX1X2VsDPhi12_ ) {
	int binIndex = lutSqrtX1X2VsDPhi12_->FindBin(diTauCandidate->dPhi12());
	double averageSqrtX1X2VsDPhi12 = lutSqrtX1X2VsDPhi12_->GetBinContent(binIndex);
	if ( averageSqrtX1X2VsDPhi12 > 0. ) hSqrtX1X2VsDPhi12Mass_->Fill(diTauCandidate->p4Vis().mass()/averageSqrtX1X2VsDPhi12, weight);
      }

      hGenLeg1DecayTime_->Fill(compDecayEigenTime(diTauCandidate->decayVertexPosLeg1gen(), 
						  diTauCandidate->primaryVertexPosGen(), diTauCandidate->p4Leg1gen().energy()), weight);
      hGenLeg2DecayTime_->Fill(compDecayEigenTime(diTauCandidate->decayVertexPosLeg2gen(), 
						  diTauCandidate->primaryVertexPosGen(), diTauCandidate->p4Leg2gen().energy()), weight);

      fillGenTauHistograms(hGenLeg1TauPlusDecayAngleLepton_, hGenLeg1TauPlusDecayAngleOneProng_, hGenLeg1TauPlusDecayAngleThreeProng_,
			   hGenLeg1TauMinusDecayAngleLepton_, hGenLeg1TauMinusDecayAngleOneProng_, hGenLeg1TauMinusDecayAngleThreeProng_,
			   diTauCandidate->p4Leg1gen(), *genParticles, weight);
      fillGenTauHistograms(hGenLeg2TauPlusDecayAngleLepton_, hGenLeg2TauPlusDecayAngleOneProng_, hGenLeg2TauPlusDecayAngleThreeProng_,
			   hGenLeg2TauMinusDecayAngleLepton_, hGenLeg2TauMinusDecayAngleOneProng_, hGenLeg2TauMinusDecayAngleThreeProng_,
			   diTauCandidate->p4Leg2gen(), *genParticles, weight);
    }

    hDiTauCandidatePt_->Fill(diTauCandidate->pt(), weight);
    hDiTauCandidateEta_->Fill(diTauCandidate->eta(), weight);
    hDiTauCandidatePhi_->Fill(diTauCandidate->phi(), weight);
    hDiTauCandidateCharge_->Fill(diTauCandidate->charge(), weight);
    hDiTauCandidateMass_->Fill(diTauCandidate->mass(), weight);

    double minLogLikelihood = 1.e+6;

    const std::vector<SVmassRecoSolution>& svFitSolutions = diTauCandidate->svFitSolutions();
    for ( std::vector<SVmassRecoSolution>::const_iterator svFitSolution = svFitSolutions.begin();
	  svFitSolution != svFitSolutions.end(); ++svFitSolution ) {
      if ( svFitSolution->isValidSolution() && svFitSolution->svFitStatus() == 0 ) {
	hSVfitMass_->Fill(svFitSolution->p4().mass(), weight);
	hSVfitMassVsLogLikelihood_->Fill(svFitSolution->p4().mass(), svFitSolution->logLikelihood(), weight);
	hSVfitLogLikelihood_->Fill(svFitSolution->logLikelihood(), weight);
	double leg1TotEnergy = ( svFitSolution->x1() > 0 && svFitSolution->x1() <= 1 ) ?
	  svFitSolution->p4VisLeg1().energy()/svFitSolution->x1() : svFitSolution->p4VisLeg1().energy();
	hSVfitDecayTimeLeg1_->Fill(compDecayEigenTime(svFitSolution->decayVertexPosLeg1(), 
						      svFitSolution->primaryVertexPosSVrefitted(), leg1TotEnergy), weight);
	double leg2TotEnergy = ( svFitSolution->x2() > 0 && svFitSolution->x2() <= 1 ) ?
	  svFitSolution->p4VisLeg2().energy()/svFitSolution->x2() : svFitSolution->p4VisLeg2().energy();
	hSVfitDecayTimeLeg2_->Fill(compDecayEigenTime(svFitSolution->decayVertexPosLeg2(), 
						      svFitSolution->primaryVertexPosSVrefitted(), leg2TotEnergy), weight);
	
	if ( svFitSolution->logLikelihood() < minLogLikelihood ) minLogLikelihood = svFitSolution->logLikelihood();
      }
      
      hSVfitStatus_->Fill(svFitSolution->svFitStatus(), weight);
    }

    fillSVmassRecoSolutionHistogram(0, hSVfitMass1stSolution_, svFitSolutions, weight);
    fillSVmassRecoSolutionHistogram(1, hSVfitMass2ndSolution_, svFitSolutions, weight);
    fillSVmassRecoSolutionHistogram(2, hSVfitMass3rdSolution_, svFitSolutions, weight);
    fillSVmassRecoSolutionHistogram(3, hSVfitMass4thSolution_, svFitSolutions, weight);
    
    double genDiTauMass = diTauCandidate->p4gen().mass();
    double svFitMassBestMatch = 1.e+6;
    double svFitMassAverage = 0.;
    unsigned svFitMassNumSolutionsAveraged = 0;
    double svFitMassAverageNumSigmaCut = 2.;

    for ( std::vector<SVmassRecoSolution>::const_iterator svFitSolution = svFitSolutions.begin();
	  svFitSolution != svFitSolutions.end(); ++svFitSolution ) {
      if ( svFitSolution->isValidSolution() && svFitSolution->svFitStatus() == 0 ) {
	if ( TMath::Abs(svFitSolution->p4().mass() - genDiTauMass) < TMath::Abs(svFitMassBestMatch - genDiTauMass) ) {
	  svFitMassBestMatch = svFitSolution->p4().mass();
	}
	
	if ( svFitSolution->logLikelihood() < (0.5*svFitMassAverageNumSigmaCut*svFitMassAverageNumSigmaCut) ) {
	  svFitMassAverage += svFitSolution->p4().mass();
	  ++svFitMassNumSolutionsAveraged;
	}
      }
    }

    if ( svFitMassBestMatch != 1.e+6 ) hSVfitMassBestMatch_->Fill(svFitMassBestMatch, weight);

    if ( svFitMassNumSolutionsAveraged > 0 ) hSVfitMassAverage_->Fill(svFitMassAverage/svFitMassNumSolutionsAveraged, weight);
    hSVfitMassNumSolutionsAveraged_->Fill(svFitMassNumSolutionsAveraged, weight);

    hLeg1PtVsLeg2Pt_->Fill(diTauCandidate->leg1()->pt(), diTauCandidate->leg2()->pt(), weight);
    hLeg1EtaVsLeg2Eta_->Fill(diTauCandidate->leg1()->eta(), diTauCandidate->leg2()->eta(), weight);

    fillWeightHistograms(hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_, 
			 hDiTauCandidateWeightLinear_, diTauCandidateWeight);

    const reco::Track* trackLeg1 = trackExtractorLeg1_(*diTauCandidate->leg1());
    const reco::Track* trackLeg2 = trackExtractorLeg2_(*diTauCandidate->leg2());
    if ( trackLeg1 && trackLeg2 &&
	 recoVertices->size() >= 1 ) {
      const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());

      double trackLeg1IpSig = trackLeg1->dxy(thePrimaryEventVertex.position())/trackLeg1->dxyError();
      double trackLeg2IpSig = trackLeg2->dxy(thePrimaryEventVertex.position())/trackLeg2->dxyError();

      hDiTauCandidateImpParSig_->Fill(TMath::Sqrt(trackLeg1IpSig*trackLeg1IpSig + trackLeg2IpSig*trackLeg2IpSig), weight);
    }

    hVisPt_->Fill(diTauCandidate->p4Vis().pt(), weight);
    hVisPhi_->Fill(diTauCandidate->p4Vis().phi(), weight);
    hVisMass_->Fill(diTauCandidate->p4Vis().mass(), weight);
    if ( visMassHypothesisSrc_.label() != "" ) {
      typedef edm::RefProd<CompositePtrCandidateCollection> CompositePtrCandidateRefProd;
      typedef std::vector<float> vfloat;
      typedef edm::AssociationVector<CompositePtrCandidateRefProd, vfloat> ZtautauVisMassHypothesisCollection;
      edm::Handle<ZtautauVisMassHypothesisCollection> visMassHypotheses;
      evt.getByLabel(visMassHypothesisSrc_, visMassHypotheses);

      unsigned numDiTauCandidates = diTauCandidates->size();
      for ( unsigned idxDiTauCandidate = 0; idxDiTauCandidate < numDiTauCandidates; ++idxDiTauCandidate ) {
	edm::Ref<CompositePtrCandidateCollection> diTauCandidateRef(diTauCandidates, idxDiTauCandidate);

	if ( diTauCandidateRef.get() != &(*diTauCandidate) ) continue;

	if ( visMassHypotheses->keyProduct().id() == diTauCandidateRef.id() ) {
	  hVisMassZllCombinedHypothesis_->Fill((*visMassHypotheses)[diTauCandidateRef], weight);
	} else {
	  edm::LogWarning("CompositePtrCandidateT1T2MEtHistManager::fillHistogramsImp") 
	    << " Mismatch between Product ids. of ZtautauVisMassHypothesisCollection  = " << visMassHypotheses->keyProduct().id()
	    << " and CompositePtrCandidateCollection = " << diTauCandidateRef.id() << ", "
	    << " presumably CompositePtrCandidateCollection has been filtered without re-keying ZtautauVisMassHypothesisCollection !!";
	}
      }
    }

    if ( diTauCandidate->collinearApproxIsValid() ) {  
      hCollinearApproxEta_->Fill(diTauCandidate->p4CollinearApprox().eta(), weight);
      hCollinearApproxMass_->Fill(diTauCandidate->p4CollinearApprox().mass(), weight);
      hCollinearApproxX1_->Fill(diTauCandidate->x1CollinearApprox(), weight);
      hCollinearApproxX2_->Fill(diTauCandidate->x2CollinearApprox(), weight);
    }

    hCDFmethodMass_->Fill(diTauCandidate->p4CDFmethod().mass(), weight);

    hMt12MET_->Fill(diTauCandidate->mt12MET(), weight);

    hMt1MET_->Fill(diTauCandidate->mt1MET(), weight);
    hMt2MET_->Fill(diTauCandidate->mt2MET(), weight);

    hDPhi12_->Fill(diTauCandidate->dPhi12(), weight);
    hDR12_->Fill(diTauCandidate->dR12(), weight);

    hVisEtaMin_->Fill(diTauCandidate->visEtaMin(), weight);
    hVisEtaMax_->Fill(diTauCandidate->visEtaMax(), weight);

    hDPhi1MET_->Fill(diTauCandidate->dPhi1MET(), weight);
    hDPhi2MET_->Fill(diTauCandidate->dPhi2MET(), weight);
    hDPhi1METvsDPhi2MET_->Fill(diTauCandidate->dPhi1MET(), diTauCandidate->dPhi2MET(), weight);

    hPzetaCorr_->Fill(diTauCandidate->pZetaVis(), diTauCandidate->pZeta(), weight);
    hPzetaDiff_->Fill(diTauCandidate->pZeta() - 1.5*diTauCandidate->pZetaVis(), weight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEtHistManager<reco::Candidate, reco::Candidate> DiCandidatePairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Electron, pat::Tau> PATElecTauPairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Muon, pat::Tau> PATMuTauPairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Tau, pat::Tau> PATDiTauPairHistManager;
typedef CompositePtrCandidateT1T2MEtHistManager<pat::Electron, pat::Muon> PATElecMuPairHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, DiCandidatePairHistManager, "DiCandidatePairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, DiCandidatePairHistManager, "DiCandidatePairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairHistManager, "PATElecTauPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairHistManager, "PATElecTauPairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairHistManager, "PATMuTauPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairHistManager, "PATMuTauPairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairHistManager, "PATDiTauPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairHistManager, "PATDiTauPairHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairHistManager, "PATElecMuPairHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairHistManager, "PATElecMuPairHistManager");
  
#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<DiCandidatePairHistManager> DiCandidatePairAnalyzer;
typedef HistManagerAdapter<PATElecTauPairHistManager> PATElecTauPairAnalyzer;
typedef HistManagerAdapter<PATMuTauPairHistManager> PATMuTauPairAnalyzer;
typedef HistManagerAdapter<PATDiTauPairHistManager> PATDiTauPairAnalyzer;
typedef HistManagerAdapter<PATElecMuPairHistManager> PATElecMuPairAnalyzer;

DEFINE_FWK_MODULE(DiCandidatePairAnalyzer);
DEFINE_FWK_MODULE(PATElecTauPairAnalyzer);
DEFINE_FWK_MODULE(PATMuTauPairAnalyzer);
DEFINE_FWK_MODULE(PATDiTauPairAnalyzer);
DEFINE_FWK_MODULE(PATElecMuPairAnalyzer);

