#include "TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodMEt3.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "DataFormats/METReco/interface/PFMEtSignCovMatrix.h"

#include <TMath.h>
#include <TString.h>
#include <TFile.h>

using namespace SVfit_namespace;

NSVfitEventLikelihoodMEt3::NSVfitEventLikelihoodMEt3(const edm::ParameterSet& cfg)
  : NSVfitEventLikelihood(cfg),
    lut_(0),
    pfMEtSign_(0)
{
  power_ = ( cfg.exists("power") ) ?
    cfg.getParameter<double>("power") : 1.0;

  srcPFJets_ = cfg.getParameter<edm::InputTag>("srcPFJets");
  srcPFCandidates_ = cfg.getParameter<edm::InputTag>("srcPFCandidates");
  
  dRoverlapPFJet_ = cfg.getParameter<double>("dRoverlapPFJet");
  dRoverlapPFCandidate_ = cfg.getParameter<double>("dRoverlapPFCandidate");
  
  pfCandPtThreshold_ = cfg.getParameter<double>("pfCandPtThreshold");
  pfJetPtThreshold_ = cfg.getParameter<double>("pfJetPtThreshold");

  std::string lutName = std::string(pluginName_).append("_lut");
  lut_ = new TH2D(lutName.data(), lutName.data(), 1000, -250., +250, 1000, -250., +250);

  numToys_ = cfg.getParameter<unsigned>("numToys");

  pfMEtSign_ = new PFMEtSignInterfaceBase(cfg.getParameter<edm::ParameterSet>("resolution"));

  monitorMEtUncertainty_ = ( cfg.exists("monitorMEtUncertainty") ) ?
    cfg.getParameter<bool>("monitorMEtUncertainty") : false;
  if ( monitorMEtUncertainty_ ) monitorFilePath_ = cfg.getParameter<std::string>("monitorFilePath");

  verbosity_ = cfg.exists("verbosity") ?
    cfg.getParameter<int>("verbosity") : 0;
}

NSVfitEventLikelihoodMEt3::~NSVfitEventLikelihoodMEt3()
{
  delete lut_;
  delete pfMEtSign_;
}

void NSVfitEventLikelihoodMEt3::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter("allLeptons",   nSVfit_namespace::kLep_shiftEn,    pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_energy_lab,  pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_phi_lab,     pluginName_);
}

namespace
{
  template <typename T>
  void makeLists(const std::vector<T>& collection, std::list<const T*>& listForCovMatrix, std::list<const T*>& listForToys, double ptThreshold)
  {
    for ( typename std::vector<T>::const_iterator object = collection.begin();
	  object != collection.end(); ++object ) {
      if ( object->pt() > ptThreshold ) listForToys.push_back(&(*object));
      else listForCovMatrix.push_back(&(*object));
    }
  }
  
  template <typename T>
  void removePFCandidateOverlaps(std::list<const reco::PFCandidate*>& pfCandidates, 
				 const std::list<const T*>& objectsNotToBeFiltered, double dRoverlap)
  {
    std::list<const reco::PFCandidate*>::iterator pfCandidate = pfCandidates.begin();
    while ( pfCandidate != pfCandidates.end() ) {
      bool isOverlap = false;
      for ( typename std::list<const T*>::const_iterator objectNotToBeFiltered = objectsNotToBeFiltered.begin();
	    objectNotToBeFiltered != objectsNotToBeFiltered.end() && !isOverlap; ++objectNotToBeFiltered ) {
	if ( deltaR((*pfCandidate)->p4(), (*objectNotToBeFiltered)->p4()) < dRoverlap ) isOverlap = true;
      }
      
      if ( isOverlap ) pfCandidate = pfCandidates.erase(pfCandidate);
      else ++pfCandidate;
    }
  }

  template <typename T>
  void removePFJetOverlaps(std::list<const reco::PFJet*>& pfJets, 
			   const std::list<const T*>& objectsNotToBeFiltered, double dRoverlapPFJet, double dRoverlapPFCandidate)
  {
    std::list<const reco::PFJet*>::iterator pfJet = pfJets.begin();
    while ( pfJet != pfJets.end() ) {
      bool isOverlap = false;
      for ( typename std::list<const T*>::const_iterator objectNotToBeFiltered = objectsNotToBeFiltered.begin();
	    objectNotToBeFiltered != objectsNotToBeFiltered.end() && !isOverlap; ++objectNotToBeFiltered ) {
	if ( deltaR((*pfJet)->p4(), (*objectNotToBeFiltered)->p4()) < dRoverlapPFJet ) isOverlap = true;
      }
      
      const reco::Jet::Constituents pfJetConstituents = (*pfJet)->getJetConstituents();
      for ( reco::Jet::Constituents::const_iterator pfJetConstituent = pfJetConstituents.begin();
	    pfJetConstituent != pfJetConstituents.end() && !isOverlap; ++pfJetConstituent ) {
	for ( typename std::list<const T*>::const_iterator objectNotToBeFiltered = objectsNotToBeFiltered.begin();
	      objectNotToBeFiltered != objectsNotToBeFiltered.end(); ++objectNotToBeFiltered ) {
	  if ( deltaR((*pfJetConstituent)->p4(), (*objectNotToBeFiltered)->p4()) < dRoverlapPFCandidate ) isOverlap = true;
	}
      }
      
      if ( isOverlap ) pfJet = pfJets.erase(pfJet);
      else ++pfJet;
    }
  }
}

void NSVfitEventLikelihoodMEt3::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<reco::PFJetCollection> pfJets;
  evt.getByLabel(srcPFJets_, pfJets);
  makeLists<reco::PFJet>(*pfJets, pfJetListForCovMatrix_, pfJetListForToys_, pfJetPtThreshold_);  

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(srcPFCandidates_, pfCandidates);
  makeLists<reco::PFCandidate>(*pfCandidates, pfCandidateListForCovMatrix_, pfCandidateListForToys_, pfCandPtThreshold_); 

  std::list<const reco::PFCandidate*> pfJetConstituentList;
  for ( std::list<const reco::PFJet*>::const_iterator pfJet = pfJetListForCovMatrix_.begin();
	pfJet != pfJetListForCovMatrix_.end(); ++pfJet ) {
    const std::vector<reco::PFCandidatePtr> pfJetConstituents = (*pfJet)->getPFConstituents();
    for ( std::vector<reco::PFCandidatePtr>::const_iterator pfJetConstituent = pfJetConstituents.begin();
	  pfJetConstituent != pfJetConstituents.end(); ++pfJetConstituent ) {
      pfJetConstituentList.push_back(pfJetConstituent->get());
    }
  }
  for ( std::list<const reco::PFJet*>::const_iterator pfJet = pfJetListForToys_.begin();
	pfJet != pfJetListForToys_.end(); ++pfJet ) {
    const std::vector<reco::PFCandidatePtr> pfJetConstituents = (*pfJet)->getPFConstituents();
    for ( std::vector<reco::PFCandidatePtr>::const_iterator pfJetConstituent = pfJetConstituents.begin();
	  pfJetConstituent != pfJetConstituents.end(); ++pfJetConstituent ) {
      pfJetConstituentList.push_back(pfJetConstituent->get());
    }
  }

  removePFCandidateOverlaps(pfCandidateListForCovMatrix_, pfJetConstituentList, dRoverlapPFCandidate_);
  removePFCandidateOverlaps(pfCandidateListForToys_, pfJetConstituentList, dRoverlapPFCandidate_);

  if ( monitorMEtUncertainty_ ) {
    TString monitorFileName_tstring = monitorFilePath_.data();
    if ( monitorFileName_tstring.Length() > 0 && !monitorFileName_tstring.EndsWith("/") ) monitorFileName_tstring.Append("/");
    monitorFileName_tstring.Append(Form("metUncertainty_%s_run%i_ls%i_ev%i.root", pluginName_.data(), evt.id().run(), evt.luminosityBlock(), evt.id().event()));
    monitorFileName_ = monitorFileName_tstring.Data();
  }
}

namespace
{
  template<typename T>
  void addPFMEtSignObjects(const PFMEtSignInterfaceBase* pfMEtSign,
			   std::vector<metsig::SigInputObj>& metSignObjects, 
			   const std::list<T*>& particles) 
  {
    for ( typename std::list<T*>::const_iterator particle = particles.begin();
	  particle != particles.end(); ++particle ) {
      metSignObjects.push_back(pfMEtSign->compResolution(*particle));
    }
  }
}

void NSVfitEventLikelihoodMEt3::beginCandidate(const NSVfitEventHypothesis* hypothesis) const
{
  //if ( this->verbosity_ ) {
  //  std::cout << "<NSVfitEventLikelihoodMEt3::beginCandidate>:" << std::endl;
  //  std::cout << " hypothesis = " << hypothesis << std::endl;
  //}

  std::list<const reco::Candidate*> daughterHypothesesList;
    
  size_t numResonances = hypothesis->numResonances();
  for ( size_t iResonance = 0; iResonance < numResonances; ++iResonance ) {
    const NSVfitResonanceHypothesis* resonance = hypothesis->resonance(iResonance);
    size_t numDaughters = resonance->numDaughters();
    for ( size_t iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
      const NSVfitSingleParticleHypothesis* daughter = resonance->daughter(iDaughter);
      if ( daughter->particle().isNonnull() ) daughterHypothesesList.push_back(daughter->particle().get());
    }
  }
  
  if ( this->verbosity_ ) {
    std::cout << " daughterHypothesesList: #entries = " << daughterHypothesesList.size() << std::endl;
    std::cout << " pfJetList: #entries(covMatrix) = " << pfJetListForCovMatrix_.size() << ", #entries(toys) = " << pfJetListForToys_.size() << std::endl;
    std::cout << " pfCandidateList: #entries(covMatrix) = " << pfCandidateListForCovMatrix_.size() << ", #entries(toys) = " << pfCandidateListForToys_.size() << std::endl;
  }

  std::list<const reco::PFJet*> pfJetListForCovMatrix_hypothesis = pfJetListForCovMatrix_;
  std::list<const reco::PFJet*> pfJetListForToys_hypothesis = pfJetListForToys_;
  removePFJetOverlaps(pfJetListForCovMatrix_hypothesis, daughterHypothesesList, dRoverlapPFJet_, dRoverlapPFCandidate_);  
  removePFJetOverlaps(pfJetListForToys_hypothesis, daughterHypothesesList, dRoverlapPFJet_, dRoverlapPFCandidate_); 

  std::list<const reco::PFCandidate*> pfCandidateListForCovMatrix_hypothesis = pfCandidateListForCovMatrix_;
  std::list<const reco::PFCandidate*> pfCandidateListForToys_hypothesis = pfCandidateListForToys_;
  removePFCandidateOverlaps(pfCandidateListForCovMatrix_hypothesis, daughterHypothesesList, dRoverlapPFCandidate_);
  removePFCandidateOverlaps(pfCandidateListForToys_hypothesis, daughterHypothesesList, dRoverlapPFCandidate_);

  std::vector<metsig::SigInputObj> signInputObjectsForCovMatrix;
  addPFMEtSignObjects(pfMEtSign_, signInputObjectsForCovMatrix, pfJetListForCovMatrix_hypothesis);
  addPFMEtSignObjects(pfMEtSign_, signInputObjectsForCovMatrix, pfCandidateListForCovMatrix_hypothesis);
  TMatrixD pfMEtCov = (*pfMEtSign_)(signInputObjectsForCovMatrix);
  double sigma1 = TMath::Sqrt(pfMEtCov(0,0));
  double sigma2 = TMath::Sqrt(pfMEtCov(1,1));
  double rho = pfMEtCov(0,1)/(sigma1*sigma2);
  double sqrt_1_minus_rho2 = TMath::Sqrt(1. - square(rho));
  if ( verbosity_ ) {
    std::cout << "pfMEtCov:" << std::endl;
    pfMEtCov.Print();
  }

  std::vector<metsig::SigInputObj> signInputObjectsForToys;
  addPFMEtSignObjects(pfMEtSign_, signInputObjectsForCovMatrix, pfJetListForToys_hypothesis);
  addPFMEtSignObjects(pfMEtSign_, signInputObjectsForCovMatrix, pfCandidateListForToys_hypothesis);
  for ( std::vector<metsig::SigInputObj>::iterator signInputObject = signInputObjectsForToys.begin();
	signInputObject != signInputObjectsForToys.end(); ++signInputObject ) {
    signInputObject->set(
      signInputObject->get_type(),	 
      signInputObject->get_energy(),	 
      signInputObject->get_phi(),	 
      signInputObject->get_sigma_e(),	 
      signInputObject->get_sigma_tan()/signInputObject->get_energy());
  }
  
  lut_->Reset();

  for ( unsigned iToy = 0; iToy < numToys_; ++iToy ) {
    double sumPx = 0.;
    double sumPy = 0.;
    for ( std::vector<metsig::SigInputObj>::const_iterator signInputObject = signInputObjectsForToys.begin();
	  signInputObject != signInputObjectsForToys.end(); ++signInputObject ) {
      double toyObjectPt = rnd_.Gaus(signInputObject->get_energy(), signInputObject->get_sigma_e());
      double toyObjectPhi = rnd_.Gaus(signInputObject->get_phi(), signInputObject->get_sigma_tan());
      double toyObjectPx = toyObjectPt*TMath::Cos(toyObjectPhi);
      double toyObjectPy = toyObjectPt*TMath::Sin(toyObjectPhi);
      //std::cout << "toyObject #" << idx << ": Px = " << toyObjectPx << ", Py = " << toyObjectPy 
      //	  << " (Pt = " << toyObjectPt << ", phi = " <<toyObjectPhi << ")" << std::endl;
      sumPx += toyObjectPx;
      sumPy += toyObjectPy;
    }
    double eta1 = rnd_.Gaus(0., 1.);
    double eta2 = rnd_.Gaus(0., 1.);
    sumPx += (sigma1*eta1);
    sumPy += (sigma2*(rho*eta1 + sqrt_1_minus_rho2*eta2));
    double metPx = -sumPx;
    double metPy = -sumPy;
    lut_->Fill(metPx, metPy);
  }
  lut_->Scale(1./numToys_);

  xAxis_ = lut_->GetXaxis();
  numBinsX_ = xAxis_->GetNbins();
  yAxis_ = lut_->GetYaxis();  
  numBinsY_ = yAxis_->GetNbins();

  if ( monitorMEtUncertainty_ ) {
    TFile* monitorFile = new TFile(monitorFileName_.data(), "RECREATE");
    lut_->Write();
    delete monitorFile;
  }
}

double NSVfitEventLikelihoodMEt3::operator()(const NSVfitEventHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for neutrinos produced in tau lepton decays
//    to match missing transverse momentum reconstructed in the event
//
//    NB: MET likelihood is split into perp/par components along (leptonic) leg1 of the diTau object
//
  
  double residualPx = hypothesis->dp4MEt_fitted().px();
  double residualPy = hypothesis->dp4MEt_fitted().py();

  if ( this->verbosity_ ) {
    std::cout << "<NSVfitEventLikelihoodMEt3::operator()>:" << std::endl;
    std::cout << " residualPx = " << residualPx << std::endl;
    std::cout << " residualPy = " << residualPy << std::endl;
  }

  double prob = 0.;

  int idxBinX = xAxis_->FindBin(residualPx);
  int idxBinY = yAxis_->FindBin(residualPy);
  if ( idxBinX >= 1 && idxBinX <= numBinsX_ &&
       idxBinY >= 1 && idxBinY <= numBinsY_ ) {
    prob = lut_->GetBinContent(idxBinX, idxBinY);
  } else {
    prob = 0.;
  }

  if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventLikelihoodPluginFactory, NSVfitEventLikelihoodMEt3, "NSVfitEventLikelihoodMEt3");
