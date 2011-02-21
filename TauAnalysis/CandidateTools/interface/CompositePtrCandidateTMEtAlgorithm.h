#ifndef TauAnalysis_CandidateTools_CompositePtrCandidateTMEtAlgorithm_h
#define TauAnalysis_CandidateTools_CompositePtrCandidateTMEtAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "TauAnalysis/CandidateTools/interface/SVfitAlgorithmWtauNu.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>

template<typename T>
class CompositePtrCandidateTMEtAlgorithm 
{
  typedef edm::Ptr<T> TPtr;
  typedef edm::Ptr<reco::MET> MEtPtr;

 public:

  CompositePtrCandidateTMEtAlgorithm(const edm::ParameterSet& cfg)
  {
    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
    if ( cfg.exists("svFit") ) {
      edm::ParameterSet cfgSVfit = cfg.getParameter<edm::ParameterSet>("svFit");	
      std::vector<std::string> svFitAlgorithmNames = cfgSVfit.getParameterNamesForType<edm::ParameterSet>();
      for ( std::vector<std::string>::const_iterator svFitAlgorithmName = svFitAlgorithmNames.begin();
	    svFitAlgorithmName != svFitAlgorithmNames.end(); ++svFitAlgorithmName ) {
	edm::ParameterSet cfgSVfitAlgorithm = cfgSVfit.getParameter<edm::ParameterSet>(*svFitAlgorithmName);
	cfgSVfitAlgorithm.addParameter<std::string>("name", *svFitAlgorithmName);
	copyCfgParameter<edm::InputTag>(cfg, "srcPrimaryVertex", cfgSVfitAlgorithm);
	copyCfgParameter<edm::InputTag>(cfg, "srcBeamSpot", cfgSVfitAlgorithm);
	SVfitAlgorithmWtauNu<T>* svFitAlgorithm = new SVfitAlgorithmWtauNu<T>(cfgSVfitAlgorithm);
	svFitAlgorithms_.insert(std::pair<std::string, SVfitAlgorithmWtauNu<T>*>(*svFitAlgorithmName, svFitAlgorithm));
	//std::cout << "--> adding SVfit algorithm: name = " << (*svFitAlgorithmName) << std::endl;
      }
    }
  }
  ~CompositePtrCandidateTMEtAlgorithm() 
  {
    for ( typename std::map<std::string, SVfitAlgorithmWtauNu<T>*>::iterator it = svFitAlgorithms_.begin();
	  it != svFitAlgorithms_.end(); ++it ) {
      delete it->second;
    }
  }

  void beginJob()
  {
    for ( typename std::map<std::string, SVfitAlgorithmWtauNu<T>*>::iterator svFitAlgorithm = svFitAlgorithms_.begin();
	  svFitAlgorithm != svFitAlgorithms_.end(); ++svFitAlgorithm ) {
      svFitAlgorithm->second->beginJob();
    }
  }

  void beginEvent(edm::Event& evt, const edm::EventSetup& es)
  {
    for ( typename std::map<std::string, SVfitAlgorithmWtauNu<T>*>::iterator svFitAlgorithm = svFitAlgorithms_.begin();
	  svFitAlgorithm != svFitAlgorithms_.end(); ++svFitAlgorithm ) {
      svFitAlgorithm->second->beginEvent(evt, es);
    }
  }

  CompositePtrCandidateTMEt<T> buildCompositePtrCandidate(const TPtr visDecayProducts, 
							  const MEtPtr met,
							  const reco::Vertex* pv,
							  const reco::BeamSpot* beamSpot,
							  const TransientTrackBuilder* trackBuilder,
							  bool doSVreco)
  {
    CompositePtrCandidateTMEt<T> compositePtrCandidate(visDecayProducts, met);
  
    if ( visDecayProducts.isNull() ) {
      edm::LogError ("CompositePtrCandidateTMEtAlgorithm") << " Pointer to visible Decay products invalid !!";
      return compositePtrCandidate;
    }

    if ( met.isNull() ) {
      edm::LogError ("CompositePtrCandidateTMEtAlgorithm") << " Pointer to missing transverse momentum invalid !!";
      return compositePtrCandidate;
    }

    compositePtrCandidate.setCharge(visDecayProducts->charge());
    compositePtrCandidate.setMt(compMt(visDecayProducts->p4(), met->px(), met->py()));
    compositePtrCandidate.setDPhi(TMath::Abs(normalizedPhi(visDecayProducts->phi() - met->phi())));
    
//--- SV method computation (if we have the PV and beamspot)
    if ( pv && beamSpot && trackBuilder && doSVreco ) {
      for ( typename std::map<std::string, SVfitAlgorithmWtauNu<T>*>::const_iterator svFitAlgorithm = svFitAlgorithms_.begin();
	    svFitAlgorithm != svFitAlgorithms_.end(); ++svFitAlgorithm ) {
	std::vector<SVfitWtauNuSolution> svFitSolutions = svFitAlgorithm->second->fit(compositePtrCandidate);
	for ( std::vector<SVfitWtauNuSolution>::const_iterator svFitSolution = svFitSolutions.begin();
	      svFitSolution != svFitSolutions.end(); ++svFitSolution ) {
	  compositePtrCandidate.addSVfitSolution(svFitAlgorithm->first, *svFitSolution);
	}
      }
    }

    return compositePtrCandidate;
  }

 private: 

  double compMt(const reco::Candidate::LorentzVector& visParticle, 
		double metPx, double metPy)
  {
    double px = visParticle.px() + metPx;
    double py = visParticle.py() + metPy;
    double et = visParticle.Et() + TMath::Sqrt(metPx*metPx + metPy*metPy);
    double mt2 = et*et - (px*px + py*py);
    if ( mt2 < 0 ) {
      edm::LogWarning ("compMt") << " mt2 = " << mt2 << " must not be negative !!";
      return 0.;
    }
    return TMath::Sqrt(mt2);
  }

  int verbosity_;
  std::map<std::string, SVfitAlgorithmWtauNu<T>*> svFitAlgorithms_;
};

#endif 

