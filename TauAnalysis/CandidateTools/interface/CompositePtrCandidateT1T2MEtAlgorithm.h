#ifndef TauAnalysis_CandidateTools_CompositePtrCandidateT1T2MEtAlgorithm_h
#define TauAnalysis_CandidateTools_CompositePtrCandidateT1T2MEtAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/METReco/interface/MET.h"

#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include "TMath.h"
#include "TF1.h"

#include <string>
#include <vector>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtAlgorithm
{
  typedef edm::Ptr<T1> T1Ptr;
  typedef edm::Ptr<T2> T2Ptr;
  typedef edm::Ptr<reco::MET> MEtPtr;

 public:

  CompositePtrCandidateT1T2MEtAlgorithm(const edm::ParameterSet& cfg)
    : pfMEtSign_(0)
  {
    //std::cout << "<CompositePtrCandidateT1T2MEtAlgorithm::CompositePtrCandidateT1T2MEtAlgorithm>:" << std::endl;

    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);

    if ( cfg.exists("pfMEtSign") ) {
      edm::ParameterSet cfgPFMEtSign = cfg.getParameter<edm::ParameterSet>("pfMEtSign");
      pfMEtSign_ = new PFMEtSignInterface(cfgPFMEtSign);
    }

    if ( cfg.exists("nSVfit") ) {
      edm::ParameterSet cfgNSVfit = cfg.getParameter<edm::ParameterSet>("nSVfit");
      std::vector<std::string> nSVfitAlgorithmNames = cfgNSVfit.getParameterNamesForType<edm::ParameterSet>();
      for ( std::vector<std::string>::const_iterator nSVfitAlgorithmName = nSVfitAlgorithmNames.begin();
	    nSVfitAlgorithmName != nSVfitAlgorithmNames.end(); ++nSVfitAlgorithmName ) {
	edm::ParameterSet cfgNSVfitAlgorithm = cfgNSVfit.getParameter<edm::ParameterSet>(*nSVfitAlgorithmName);
	edm::ParameterSet cfg_config = cfgNSVfitAlgorithm.getParameter<edm::ParameterSet>("config");
	edm::ParameterSet cfg_event = cfg_config.getParameter<edm::ParameterSet>("event");
	edm::ParameterSet cfg_algorithm = cfgNSVfitAlgorithm.getParameter<edm::ParameterSet>("algorithm");
	cfg_algorithm.addParameter<edm::ParameterSet>("event", cfg_event);
	cfg_algorithm.addParameter<std::string>("pluginName", *nSVfitAlgorithmName);
	std::string pluginType = cfg_algorithm.getParameter<std::string>("pluginType");
	NSVfitAlgorithmBase* nSVfitAlgorithm = NSVfitAlgorithmPluginFactory::get()->create(pluginType, cfg_algorithm);
        try {
          nSVfitAlgorithms_.insert(std::pair<std::string, NSVfitAlgorithmBase*>(*nSVfitAlgorithmName, nSVfitAlgorithm));
        } catch (...) {
          edm::LogError("NSVFitConfigurationError") <<
            "Caught exception when building NSVfit algorithm: "
            << *nSVfitAlgorithmName  << std::endl;
          throw;
        }
	//std::cout << "--> adding nSVfit algorithm: name = " << (*nSVfitAlgorithmName) << std::endl;
      }
    }

    scaleFuncImprovedCollinearApprox_ = cfg.exists("scaleFuncImprovedCollinearApprox") ?
      cfg.getParameter<std::string>("scaleFuncImprovedCollinearApprox") : "1";
    /// compute the scale factor to weight the diTau mass
    /// computed in the improved collinear approximation;
    /// NO re-scaling of the p4 is made at this stage.
    scaleFunc_ = new TF1("scaleFunc_", scaleFuncImprovedCollinearApprox_.c_str(), 10, 300);

    if ( cfg.exists("genParticleMatchPdgId") ) {
      genParticleMatchPdgId_ = cfg.getParameter<vint>("genParticleMatchPdgId");
    } else {
      // per default, match to gen. tau leptons
      genParticleMatchPdgId_.push_back(+15);
      genParticleMatchPdgId_.push_back(-15);
    }
  }

  ~CompositePtrCandidateT1T2MEtAlgorithm()
  {
    delete pfMEtSign_;

    for ( typename std::map<std::string, NSVfitAlgorithmBase*>::iterator it = nSVfitAlgorithms_.begin();
	  it != nSVfitAlgorithms_.end(); ++it ) {
      delete it->second;
    }

    delete scaleFunc_;
  }

  void beginJob(bool doSVreco)
  {
    if ( doSVreco ) {
      for ( typename std::map<std::string, NSVfitAlgorithmBase*>::iterator nSVfitAlgorithm = nSVfitAlgorithms_.begin();
	    nSVfitAlgorithm != nSVfitAlgorithms_.end(); ++nSVfitAlgorithm ) {
	nSVfitAlgorithm->second->beginJob();
      }
    }
  }

  void beginEvent(edm::Event& evt, const edm::EventSetup& es, bool doSVreco, bool doPFMEtSign)
  {
    if ( doPFMEtSign && pfMEtSign_ ) pfMEtSign_->beginEvent(evt, es);

    if ( doSVreco ) {
      for ( typename std::map<std::string, NSVfitAlgorithmBase*>::iterator nSVfitAlgorithm = nSVfitAlgorithms_.begin();
	    nSVfitAlgorithm != nSVfitAlgorithms_.end(); ++nSVfitAlgorithm ) {
	nSVfitAlgorithm->second->beginEvent(evt, es);
      }
    }
  }

  CompositePtrCandidateT1T2MEt<T1,T2> buildCompositePtrCandidate(const T1Ptr leg1,
								 const T2Ptr leg2,
								 const MEtPtr met,
								 const reco::GenParticleCollection* genParticles,
                                                                 const reco::Vertex* pv,
                                                                 const reco::BeamSpot* beamSpot,
                                                                 const TransientTrackBuilder* trackBuilder,
								 const std::string& recoMode,
								 bool doSVreco, bool doPFMEtSign)
  {
    //std::cout << "<CompositePtrCandidateT1T2MEtAlgorithm::buildCompositePtrCandidate>:"<< std::endl;
    //if ( !met.isNull() ) std::cout << " MET: pt = " << met->pt() << std::endl;
    //else                 std::cout << " MET: none." << std::endl;

    CompositePtrCandidateT1T2MEt<T1,T2> compositePtrCandidate(leg1, leg2, met);

    if ( leg1.isNull() ||
         leg2.isNull() ) {
      edm::LogError ("CompositePtrCandidateT1T2MEtAlgorithm") << " Pointers to visible Decay products invalid !!";
      return compositePtrCandidate;
    }

//--- compute quantities that are independent of MET
    compositePtrCandidate.setCharge(leg1->charge() + leg2->charge());
    compositePtrCandidate.setP4Vis(leg1->p4() + leg2->p4());
    compositePtrCandidate.setDR12(reco::deltaR(leg1->p4(), leg2->p4()));
    compositePtrCandidate.setDPhi12(TMath::Abs(normalizedPhi(leg1->phi() - leg2->phi())));
    compositePtrCandidate.setVisEtaMin(TMath::Min(leg1->eta(), leg2->eta()));
    compositePtrCandidate.setVisEtaMax(TMath::Max(leg1->eta(), leg2->eta()));

//--- compute quantities that do dependent on MET
    if ( met.isNonnull() ) {
      compCollinearApprox(compositePtrCandidate, leg1->p4(), leg2->p4(), met->px(), met->py());
      compImprovedCollinearApprox(compositePtrCandidate, leg1->p4(), leg2->p4(), met->px(), met->py());
      compositePtrCandidate.setP4CDFmethod(compP4CDFmethod(leg1->p4(), leg2->p4(), met->px(), met->py()));
      compositePtrCandidate.setMt12MET(compMt(leg1->p4(), leg2->p4(), met->px(), met->py()));
      compositePtrCandidate.setMt1MET(compMt(leg1->p4(), met->px(), met->py()));
      compositePtrCandidate.setMt2MET(compMt(leg2->p4(), met->px(), met->py()));
      compositePtrCandidate.setDPhi1MET(TMath::Abs(normalizedPhi(leg1->phi() - met->phi())));
      compositePtrCandidate.setDPhi2MET(TMath::Abs(normalizedPhi(leg2->phi() - met->phi())));

      compZeta(compositePtrCandidate, leg1->p4(), leg2->p4(), met->px(), met->py());

//--- compute (PF)MEt significance matrix
      if ( doPFMEtSign && pfMEtSign_ ) {
	std::list<const reco::Candidate*> daughterHypothesesList;
	daughterHypothesesList.push_back(leg1.get());
	daughterHypothesesList.push_back(leg2.get());
	compositePtrCandidate.setMEtSignMatrix((*pfMEtSign_)(daughterHypothesesList));
      }

//--- SV method computation (if we have the PV and beamspot)
      if ( doSVreco ) {
	if ( pv ) {
	  for ( typename std::map<std::string, NSVfitAlgorithmBase*>::const_iterator nSVfitAlgorithm = nSVfitAlgorithms_.begin();
		nSVfitAlgorithm != nSVfitAlgorithms_.end(); ++nSVfitAlgorithm ) {
	    //std::cout << "--> running nSVfit algorithm: name = " << nSVfitAlgorithm->first << std::endl;
	    typedef edm::Ptr<reco::Candidate> CandidatePtr;
	    typedef std::map<std::string, CandidatePtr> inputParticleMap;
	    inputParticleMap inputParticles;
	    inputParticles.insert(std::pair<std::string, CandidatePtr>("leg1", leg1));
	    inputParticles.insert(std::pair<std::string, CandidatePtr>("leg2", leg2));
	    inputParticles.insert(std::pair<std::string, CandidatePtr>("met",  met));
	    std::auto_ptr<NSVfitEventHypothesisBase> nSVfitHypothesis(nSVfitAlgorithm->second->fit(inputParticles, pv));
	    compositePtrCandidate.addNSVfitSolution(nSVfitHypothesis);
	    //std::cout << " done." << std::endl;
	  }
	}
      }
    } else {
      compositePtrCandidate.setCollinearApproxQuantities(reco::Candidate::LorentzVector(0,0,0,0), -1, -1, false, 0);
    }

//--- compute gen. level quantities
    if ( genParticles ) {
      compGenQuantities(compositePtrCandidate, genParticles);
    }

//--- set compositePtr four-momentum
//    (depending on recoMode configuration parameter)
    if ( recoMode == "collinearApprox" ) {
      if ( met.isNonnull() ) {
        compositePtrCandidate.setP4(compositePtrCandidate.p4CollinearApprox());
      } else {
        edm::LogError ("buildCompositePtrCandidate")
	  << " Failed to set four-momentum:"
	  << " recoMode = " << recoMode << " requires MET pointer to be valid !!";
      }
    } else if ( recoMode == "ImprovedCollinearApprox" ) {
      if ( met.isNonnull() ) {
	compositePtrCandidate.setP4(compositePtrCandidate.p4ImprovedCollinearApprox());
      } else {
	edm::LogError ("buildCompositePtrCandidate")
	  << " Failed to set four-momentum:"
	  << " recoMode = " << recoMode << " requires MET pointer to be valid !!";
      }
    }  else if ( recoMode == "cdfMethod" ) {
      if ( met.isNonnull() ) {
	compositePtrCandidate.setP4(compositePtrCandidate.p4CDFmethod());
      } else {
	edm::LogError ("buildCompositePtrCandidate")
	  << " Failed to set four-momentum:"
	  << " recoMode = " << recoMode << " requires MET pointer to be valid !!";
      }
    } else if ( recoMode == "" ) {
      compositePtrCandidate.setP4(compositePtrCandidate.p4Vis());
    } else {
      edm::LogError ("buildCompositePtrCandidate")
	<< " Failed to set four-momentum:"
	<< " recoMode = " << recoMode << " undefined !!";
    }

    compositePtrCandidate.recoMode_ = recoMode;

    return compositePtrCandidate;
  }

 private:

  void compGenQuantities(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate, const reco::GenParticleCollection* genParticles)
  {
    const reco::GenParticle* genLeg1 = findGenParticle(compositePtrCandidate.leg1()->p4(), *genParticles, 0.5, -1,
						       &genParticleMatchPdgId_, false);
    if ( genLeg1 ) {
      compositePtrCandidate.setPrimaryVertexPosGen(genLeg1->vertex());
      compositePtrCandidate.setDecayVertexPosLeg1gen(getDecayVertex(genLeg1));
      compositePtrCandidate.setP4Leg1gen(genLeg1->p4());
      compositePtrCandidate.setP4VisLeg1gen(getVisMomentum(genLeg1));
    }

    const reco::GenParticle* genLeg2 = findGenParticle(compositePtrCandidate.leg2()->p4(), *genParticles, 0.5, -1,
						       &genParticleMatchPdgId_, false);
    if ( genLeg2 ) {
      compositePtrCandidate.setDecayVertexPosLeg2gen(getDecayVertex(genLeg2));
      compositePtrCandidate.setP4Leg2gen(genLeg2->p4());
      compositePtrCandidate.setP4VisLeg2gen(getVisMomentum(genLeg2));
    }
  }
  void compCollinearApprox(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,
			   const reco::Candidate::LorentzVector& leg1,
			   const reco::Candidate::LorentzVector& leg2,
			   double metPx, double metPy)
  {
    double x1, x2;
    compX1X2byCollinearApprox(x1, x2, leg1.px(), leg1.py(), leg2.px(), leg2.py(), metPx, metPy);

    bool isX1withinPhysRange, isX2withinPhysRange;
    double x1phys = getPhysX(x1, isX1withinPhysRange);
    double x2phys = getPhysX(x2, isX2withinPhysRange);

    if ( x1phys != 0. && x2phys != 0. ) {
      reco::Candidate::LorentzVector p4 = leg1/x1phys + leg2/x2phys;
      compositePtrCandidate.setCollinearApproxQuantities(p4, x1, x2, isX1withinPhysRange && isX2withinPhysRange,1);
    } else {
      compositePtrCandidate.setCollinearApproxQuantities(reco::Candidate::LorentzVector(0,0,0,0), x1, x2, false, 0);
    }
  }

  /// compute the diTau mass in improved collinear approximation;
  /// this new algorithm allows to recover events for which
  /// the collinear approximation fails
  void compImprovedCollinearApprox(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,
				   const reco::Candidate::LorentzVector& leg1,
				   const reco::Candidate::LorentzVector& leg2,
				   double metPx, double metPy)
    {
      double x1, x2;
      compX1X2byCollinearApprox(x1, x2, leg1.px(), leg1.py(), leg2.px(), leg2.py(), metPx, metPy);

      /// define scalar products useful for later computations
      double sp1 = (leg1.px()*metPx + leg1.py()*metPy);
      double sp2 = (leg2.px()*metPx + leg2.py()*metPy);
      double module_leg1 = leg1.pt();
      double module_leg2 = leg2.pt();
      double module_Met = sqrt(metPx*metPx + metPy*metPy);
      double cos1 = sp1/(module_leg1*module_Met);
      double cos2 = sp2/(module_leg2*module_Met);
      double Enu1 = 0.;
      double Enu2 = 0.;
      /// define the angular windows to accept the events
      bool collinear_leg1 = (cos1 >= 0.940);
      bool collinear_leg2 = (cos2 >= 0.940);
      bool backToback = (cos( leg1.phi()-leg2.phi() ) < -0.90 );
      /// collinear approximation
      if ( (x1 > 0. && x1 < 1.) &&  (x2 > 0. && x2 < 1.) ) {
	reco::Candidate::LorentzVector p4 = leg1/x1 + leg2/x2;
	compositePtrCandidate.setImprovedCollinearApproxQuantities(p4, x1, x2, 1.0, true, 1);
      }
      /// rescue not-back-to-back events close to leg1
      else if ( (x1 > 0. && x1 < 1.) && !(x2 > 0. && x2 < 1.) && collinear_leg1 && !backToback){
	x1 = x1;
	x2 = 1.;
	reco::Candidate::LorentzVector p4Vis = leg1+leg2;
	reco::Candidate::LorentzVector p4 = leg1/x1 + leg2/x2;
	if(p4Vis.pt()>10.0) compositePtrCandidate.setImprovedCollinearApproxQuantities(p4, x1, -99, scaleFunc_->Eval(p4Vis.pt()), true, 2);
	else compositePtrCandidate.setImprovedCollinearApproxQuantities(p4, x1, -99, 1.0, true, 2);
      }
      /// rescue not-back-to-back events close to leg2
      else if ( !(x1 > 0. && x1 < 1.) && (x2 > 0. && x2 < 1.) && collinear_leg2 && !backToback ){
	x1 = 1.;
	x2 = x2;
	reco::Candidate::LorentzVector p4Vis = leg1+leg2;
	reco::Candidate::LorentzVector p4 = leg1/x1 + leg2/x2;
	if(p4Vis.pt()>10.0) compositePtrCandidate.setImprovedCollinearApproxQuantities(p4, -99, x2, scaleFunc_->Eval(p4Vis.pt()) , true, 2);
	else compositePtrCandidate.setImprovedCollinearApproxQuantities(p4, -99, x2, 1.0 , true, 2);
      }
      /// rescue back-to-back events using the known mean tau-energy fraction carried away
      /// by the neutrino in the tau -> l + nu_l + nu_tau decay;
      /// the first condition is a XOR between A=(x1 > 0. && x1 < 1.) and B=(x2 > 0. && x2 < 1.)
      else if ( ( (!(x1 > 0. && x1 < 1.) && (x2 > 0. && x2 < 1.)) || ((x1 > 0. && x1 < 1.) && !(x2 > 0. && x2 < 1.))  ) &&
		(collinear_leg1 || collinear_leg2) &&
		backToback ){

	if(collinear_leg1){
	  Enu1= 13./7.*leg1.pt();
	  Enu2 = Enu1-module_Met;
	  x1 = (Enu2 > 0) ? 7./20. : x1;
	  x2 = (Enu2 > 0) ? leg2.pt()/(leg2.pt()+Enu2) : 1;
	  reco::Candidate::LorentzVector p4 = leg1/x1 + leg2/x2;
	  compositePtrCandidate.setImprovedCollinearApproxQuantities(p4, 99, -99, 1.0, true, 3);
	}
	else {
	  Enu1= 13./7.*leg1.pt();
	  Enu2 = Enu1+module_Met;
	  x1 = 7./20.;
	  x2 = leg2.pt()/(leg2.pt()+Enu2);
	  reco::Candidate::LorentzVector p4 = leg1/x1 + leg2/x2;
	  compositePtrCandidate.setImprovedCollinearApproxQuantities(p4, -99, 99, 1.0, true, 3);
	}

      }
      /// if none of the previous conditions has been fulfilled,
      /// don't reconstruct the diTau
      else {
	compositePtrCandidate.setImprovedCollinearApproxQuantities(reco::Candidate::LorentzVector(0,0,0,0), -1, -1, 1.0, false, 0);
      }

    }
  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  void compZeta(CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate,
		const reco::Candidate::LorentzVector& leg1,
		const reco::Candidate::LorentzVector& leg2,
		double metPx, double metPy)
  {
    //std::cout << "<CompositePtrCandidateT1T2MEtAlgorithm::compZeta>:" << std::endl;

    double leg1x = cos(leg1.phi());
    double leg1y = sin(leg1.phi());
    double leg2x = cos(leg2.phi());
    double leg2y = sin(leg2.phi());
    double zetaX = leg1x + leg2x;
    double zetaY = leg1y + leg2y;
    double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
    if ( zetaR > 0. ) {
      zetaX /= zetaR;
      zetaY /= zetaR;
    }

    //std::cout << " leg1Phi = " << leg1.phi()*180./TMath::Pi() << std::endl;
    //std::cout << " leg2Phi = " << leg2.phi()*180./TMath::Pi() << std::endl;

    //std::cout << " zetaX = " << zetaX << std::endl;
    //std::cout << " zetaY = " << zetaY << std::endl;

    //std::cout << " zetaPhi = " << normalizedPhi(atan2(zetaY, zetaX))*180./TMath::Pi() << std::endl;

    double visPx = leg1.px() + leg2.px();
    double visPy = leg1.py() + leg2.py();
    double pZetaVis = visPx*zetaX + visPy*zetaY;

    //std::cout << " visPx = " << visPx << std::endl;
    //std::cout << " visPy = " << visPy << std::endl;

    double px = visPx + metPx;
    double py = visPy + metPy;
    double pZeta = px*zetaX + py*zetaY;

    //std::cout << " metPhi = " << normalizedPhi(atan2(metPy, metPx))*180./TMath::Pi() << std::endl;

    if ( verbosity_ ) {
      std::cout << "<CompositePtrCandidateT1T2MEtAlgorithm::compZeta>:" << std::endl;
      std::cout << " pZetaVis = " << pZetaVis << std::endl;
      std::cout << " pZeta = " << pZeta << std::endl;
    }

    compositePtrCandidate.setPzeta(pZeta);
    compositePtrCandidate.setPzetaVis(pZetaVis);
  }
  reco::Candidate::LorentzVector compP4CDFmethod(const reco::Candidate::LorentzVector& leg1,
						 const reco::Candidate::LorentzVector& leg2,
						 double metPx, double metPy)
  {
    double px = leg1.px() + leg2.px() + metPx;
    double py = leg1.py() + leg2.py() + metPy;
    double pz = leg1.pz() + leg2.pz();
    double e = leg1.energy() + leg2.energy() + TMath::Sqrt(metPx*metPx + metPy*metPy);
    reco::Candidate::LorentzVector p4(px, py, pz, e);
    return p4;
  }
  double compMt(const reco::Candidate::LorentzVector& leg1,
		const reco::Candidate::LorentzVector& leg2,
		double metPx, double metPy)
  {
    double px = leg1.px() + leg2.px() + metPx;
    double py = leg1.py() + leg2.py() + metPy;
    double et = TMath::Max(leg1.Et(), leg1.pt()) + TMath::Max(leg2.Et(), leg2.pt()) + TMath::Sqrt(metPx*metPx + metPy*metPy);
    double mt2 = et*et - (px*px + py*py);
    if ( mt2 < 0 ) {
      edm::LogWarning ("compMt(3-body)") << " mt2 = " << mt2 << " must not be negative !!";
      return 0.;
    }
    return TMath::Sqrt(mt2);
  }
  double compMt(const reco::Candidate::LorentzVector& visParticle,
		double metPx, double metPy)
  {
    double px = visParticle.px() + metPx;
    double py = visParticle.py() + metPy;
    double et = TMath::Max(visParticle.Et(), visParticle.pt()) + TMath::Sqrt(metPx*metPx + metPy*metPy);
    double mt2 = et*et - (px*px + py*py);
    if ( mt2 < 0 ) {
      edm::LogWarning ("compMt(2-body)") << " mt2 = " << mt2 << " must not be negative !!";
      return 0.;
    }
    return TMath::Sqrt(mt2);
  }

  int verbosity_;
  std::string scaleFuncImprovedCollinearApprox_;
  PFMEtSignInterface* pfMEtSign_;
  std::map<std::string, NSVfitAlgorithmBase*> nSVfitAlgorithms_;
  TF1* scaleFunc_;
  typedef std::vector<int> vint;
  vint genParticleMatchPdgId_;
};

#endif

