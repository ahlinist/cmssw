#ifndef TauAnalysis_RecoTools_ZllRecoilCorrectionProducerT1T2_h
#define TauAnalysis_RecoTools_ZllRecoilCorrectionProducerT1T2_h

/** \class ZllRecoilCorrectionProducerT1T2
 *
 * Produce collectiobs of MET plus diTau objects 
 * corrected for difference in hadronic recoil between Monte Carlo simulation and Data
 * in Z --> l+ l- events 
 * (cf. CMS AN-10-264 for a description of the method)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: ZllRecoilCorrectionProducerT1T2.h,v 1.5 2011/01/19 10:10:33 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionAlgorithm.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/Ref.h"

template <typename T1, typename T2>
class ZllRecoilCorrectionProducerT1T2 : public edm::EDProducer 
{
  typedef CompositePtrCandidateT1T2MEt<T1,T2> CompositePtrCandidate;
  typedef std::vector<CompositePtrCandidate> CompositePtrCandidateCollection;
  typedef edm::RefProd<CompositePtrCandidateCollection> CompositePtrCandidateRefProd;
  typedef std::vector<int> vint;
  typedef edm::AssociationVector<CompositePtrCandidateRefProd, vint> diTauToMEtAssociation;

 public:
  explicit ZllRecoilCorrectionProducerT1T2(const edm::ParameterSet&);
  ~ZllRecoilCorrectionProducerT1T2();

  void produce(edm::Event&, const edm::EventSetup&);

 private:

//--- configuration parameters

  std::string moduleLabel_;

  // collection of diTau objects representing Z --> l+ l- candidates
  edm::InputTag src_;

  // collection of gen. particles for determining Z/W/H Pt
  // used in parametrization of recoil correction
  edm::InputTag srcGenParticles_;
  vint genParticlePdgIds_;

  edm::InputTag srcGenMET_;

//--- algorithm doing the actual computations
  ZllRecoilCorrectionAlgorithm algorithm_;
};

#endif


