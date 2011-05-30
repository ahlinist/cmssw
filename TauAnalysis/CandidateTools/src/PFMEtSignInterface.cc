#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "RecoMET/METAlgorithms/interface/significanceAlgo.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>
#include <TVectorD.h>

using namespace SVfit_namespace;

const double defaultPFMEtResolutionX = 10.;
const double defaultPFMEtResolutionY = 10.;

const double epsilon = 1.e-4;

PFMEtSignInterface::PFMEtSignInterface(const edm::ParameterSet& cfg)
  : pfMEtResolution_(0)
{
  srcPFJets_ = cfg.getParameter<edm::InputTag>("srcPFJets");
  srcPFCandidates_ = cfg.getParameter<edm::InputTag>("srcPFCandidates");

  edm::ParameterSet cfgResolution = cfg.getParameter<edm::ParameterSet>("resolution");
  pfMEtResolution_ = new metsig::SignAlgoResolutions(cfgResolution);

  dRoverlapPFJet_ = cfg.getParameter<double>("dRoverlapPFJet");
  dRoverlapPFCandidate_ = cfg.getParameter<double>("dRoverlapPFCandidate");

  verbosity_ = cfg.exists("verbosity") ?
    cfg.getParameter<int>("verbosity") : 0;
}

PFMEtSignInterface::~PFMEtSignInterface()
{
  delete pfMEtResolution_;
}

template <typename T>
std::list<const T*> makeList(const std::vector<T>& collection)
{
  std::list<const T*> retVal;
  
  for ( typename std::vector<T>::const_iterator object = collection.begin();
	object != collection.end(); ++object ) {
    retVal.push_back(&(*object));
  }
  
  return retVal;
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

void PFMEtSignInterface::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<reco::PFJetCollection> pfJets;
  evt.getByLabel(srcPFJets_, pfJets);
  pfJetList_ = makeList<reco::PFJet>(*pfJets);  

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(srcPFCandidates_, pfCandidates);
  pfCandidateList_ = makeList<reco::PFCandidate>(*pfCandidates); 

  std::list<const reco::PFCandidate*> pfJetConstituentList;
  for ( std::list<const reco::PFJet*>::const_iterator pfJet = pfJetList_.begin();
	pfJet != pfJetList_.end(); ++pfJet ) {
    const std::vector<reco::PFCandidatePtr> pfJetConstituents = (*pfJet)->getPFConstituents();
    for ( std::vector<reco::PFCandidatePtr>::const_iterator pfJetConstituent = pfJetConstituents.begin();
	  pfJetConstituent != pfJetConstituents.end(); ++pfJetConstituent ) {
      pfJetConstituentList.push_back(pfJetConstituent->get());
    }
  }

  removePFCandidateOverlaps(pfCandidateList_, pfJetConstituentList, dRoverlapPFCandidate_);
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

TMatrixD PFMEtSignInterface::operator()(const std::list<const reco::Candidate*>& patLeptonList) const
{
  if ( this->verbosity_ ) std::cout << "<PFMEtSignInterface::operator()>:" << std::endl;

  std::list<const reco::PFJet*> pfJetList_hypothesis = pfJetList_;
  removePFJetOverlaps(pfJetList_hypothesis, patLeptonList, dRoverlapPFJet_, dRoverlapPFCandidate_);  

  std::list<const reco::PFCandidate*> pfCandidateList_hypothesis = pfCandidateList_;
  removePFCandidateOverlaps(pfCandidateList_hypothesis, patLeptonList, dRoverlapPFCandidate_);
 
  std::vector<metsig::SigInputObj> pfMEtSignObjects;
  addPFMEtSignObjects(pfMEtSignObjects, patLeptonList);
  addPFMEtSignObjects(pfMEtSignObjects, pfJetList_hypothesis);
  addPFMEtSignObjects(pfMEtSignObjects, pfCandidateList_hypothesis);

  if ( this->verbosity_ ) {
    double dpt2Sum = 0.;
    for ( std::vector<metsig::SigInputObj>::iterator pfMEtSignObject = pfMEtSignObjects.begin();
	  pfMEtSignObject != pfMEtSignObjects.end(); ++pfMEtSignObject ) {
      std::cout << pfMEtSignObject->get_type() << ": pt = " << pfMEtSignObject->get_energy() << "," 
		<< " phi = " << pfMEtSignObject->get_phi() << " --> dpt = " << pfMEtSignObject->get_sigma_e() << std::endl;
      dpt2Sum += pfMEtSignObject->get_sigma_e();
    }
    std::cout << "--> sqrt(sum(dpt^2)) = " << TMath::Sqrt(dpt2Sum) << std::endl;
  }

  metsig::significanceAlgo pfMEtSignAlgorithm;
  pfMEtSignAlgorithm.addObjects(pfMEtSignObjects);
  TMatrixD pfMEtCov = pfMEtSignAlgorithm.getSignifMatrix();

  if ( this->verbosity_ ) {
    TVectorD eigenValues(2);
    TMatrixD eigenVectors = pfMEtCov.EigenVectors(eigenValues);
    // CV: eigenvectors are stored in columns 
    //     and are sorted such that the one corresponding to the highest eigenvalue is in the **first** column
    for ( unsigned iEigenVector = 0; iEigenVector < 2; ++iEigenVector ) {
      std::cout << "eigenVector #" << iEigenVector << " (eigenValue = " << eigenValues(iEigenVector) << "):" 
		<< " x = " << eigenVectors(0, iEigenVector) << ", y = " << eigenVectors(1, iEigenVector) << std::endl;
    }
  }

//--- substitute (PF)MEt resolution matrix by default values 
//    in case resolution matrix cannot be inverted
  if ( TMath::Abs(pfMEtCov.Determinant()) < epsilon ) {
    edm::LogWarning("NSVfitEventLikelihoodMEt2::beginCandidate") 
      << "Inversion of PFMEt covariance matrix failed, det = " << pfMEtCov.Determinant()
      << " --> replacing covariance matrix by resolution defaults !!";
    pfMEtCov(0,0) = square(defaultPFMEtResolutionX);
    pfMEtCov(0,1) = 0.;
    pfMEtCov(1,0) = 0.;
    pfMEtCov(1,1) = square(defaultPFMEtResolutionY);
  }

  return pfMEtCov;
}

template <typename T>
void PFMEtSignInterface::addPFMEtSignObjects(std::vector<metsig::SigInputObj>& metSignObjects, 
					     const std::list<const T*>& particles) const
{
  if ( this->verbosity_ ) std::cout << "<NSVfitEventLikelihoodMEt2::addPFMEtSignObjects>:" << std::endl;

  for ( typename std::list<const T*>::const_iterator particle = particles.begin();
	particle != particles.end(); ++particle ) {
    double pt   = (*particle)->pt();
    double eta  = (*particle)->eta();
    double phi  = (*particle)->phi();

    if ( dynamic_cast<const pat::Electron*>(*particle) != 0 ) {
      std::string particleType = "electron";
      // WARNING: SignAlgoResolutions::PFtype2 needs to be kept in sync with reco::PFCandidate::e !!
      double dpt  = pfMEtResolution_->eval(metsig::PFtype2, metsig::ET,  pt, phi, eta);
      double dphi = pfMEtResolution_->eval(metsig::PFtype2, metsig::PHI, pt, phi, eta);
      //std::cout << "electron: pt = " << pt << ", eta = " << eta << ", phi = " << phi 
      //          << " --> dpt = " << dpt << ", dphi = " << dphi << std::endl;
      metSignObjects.push_back(metsig::SigInputObj(particleType, pt, phi, dpt, dphi));
    } else if ( dynamic_cast<const pat::Muon*>(*particle) != 0 ) {
      std::string particleType = "muon";
      double dpt, dphi;
      const pat::Muon* patMuon = dynamic_cast<const pat::Muon*>(*particle);
      if ( patMuon->track().isNonnull() && patMuon->track().isAvailable() ) {
        dpt  = patMuon->track()->ptError();
        dphi = pt*patMuon->track()->phiError(); // CV: pt*dphi is indeed correct
      } else {
	// WARNING: SignAlgoResolutions::PFtype3 needs to be kept in sync with reco::PFCandidate::mu !!
	dpt  = pfMEtResolution_->eval(metsig::PFtype3, metsig::ET,  pt, phi, eta);
	dphi = pfMEtResolution_->eval(metsig::PFtype3, metsig::PHI, pt, phi, eta);
      }
      //std::cout << "muon: pt = " << pt << ", eta = " << eta << ", phi = " << phi 
      //	  << " --> dpt = " << dpt << ", dphi = " << dphi << std::endl;
      metSignObjects.push_back(metsig::SigInputObj(particleType, pt, phi, dpt, dphi));
    } else if ( dynamic_cast<const pat::Tau*>(*particle) != 0 ) {
      // CV: use PFJet resolutions for PFTaus for now...
      //    (until PFTau specific resolutions are available)
      const pat::Tau* patTau = dynamic_cast<const pat::Tau*>(*particle);
      //std::cout << "tau: pt = " << pt << ", eta = " << eta << ", phi = " << phi << std::endl;
      metSignObjects.push_back(pfMEtResolution_->evalPFJet(patTau->pfJetRef().get()));
    } else if ( dynamic_cast<const reco::PFJet*>(*particle) != 0 ) {
      const reco::PFJet* pfJet = dynamic_cast<const reco::PFJet*>(*particle);
      //std::cout << "pfJet: pt = " << pt << ", eta = " << eta << ", phi = " << phi << std::endl;
      metSignObjects.push_back(pfMEtResolution_->evalPFJet(pfJet));
    } else if ( dynamic_cast<const reco::PFCandidate*>(*particle) != 0 ) {
      const reco::PFCandidate* pfCandidate = dynamic_cast<const reco::PFCandidate*>(*particle);
      //std::cout << "pfCandidate: pt = " << pt << ", eta = " << eta << ", phi = " << phi << std::endl;
      metSignObjects.push_back(pfMEtResolution_->evalPF(pfCandidate));
    } else throw cms::Exception("addPFMEtSignObjects")
	<< "Invalid type of particle:"
	<< " valid types = { pat::Electron, pat::Muon, pat::Tau, reco::PFJet, reco::PFCandidate } !!\n";
  }
}
