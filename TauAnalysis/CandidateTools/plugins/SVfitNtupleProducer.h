#ifndef TauAnalysis_CandidateTools_SVfitNtupleProducer_h  
#define TauAnalysis_CandidateTools_SVfitNtupleProducer_h

/** \class SVfitNtupleProducer
 *
 * Produce an Ntuple of various quantities useful 
 * to study tau+ tau- mass reconstruction
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.2 $
 *
 * $Id: SVfitNtupleProducer.h,v 1.2 2012/02/22 09:46:03 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <TMatrixD.h>
#include <TTree.h>

#include <map>
#include <string>
#include <vector>
#include <ostream>

class SVfitNtupleProducer : public edm::EDProducer 
{
 public:
  
  SVfitNtupleProducer(const edm::ParameterSet&);
  ~SVfitNtupleProducer();

  void produce(edm::Event&, const edm::EventSetup&);
  void beginJob();

 private:

  void addBranchF(const std::string&);
  void addBranchI(const std::string&);
  void addBranch_EnPxPyPz(const std::string&);
  void addBranch_PxPy(const std::string&);
  void addBranch_Cov(const std::string&); 

  void printBranches(std::ostream&);

  void setValueF(const std::string&, double);
  void setValueI(const std::string&, int);
  void setValue_EnPxPyPz(const std::string&, const reco::Candidate::LorentzVector&);
  void setValue_PxPy(const std::string&, const reco::Candidate::LorentzVector&);
  void setValue_Cov(const std::string&, const TMatrixD&);

  std::string moduleLabel_;

  edm::InputTag srcGenTauPair_;
  edm::InputTag srcGenMEt_;
  edm::InputTag srcGenTaus_;
  edm::InputTag srcGenParticles_;
  
  edm::InputTag srcGenLeg1_;
  edm::InputTag srcGenLeg2_;
  
  edm::InputTag srcRecLeg1_;
  edm::InputTag srcRecLeg2_;

  edm::InputTag srcRecMEt_;

  edm::InputTag srcPFMEtCovMatrix_;

  struct branchEntryType
  {
    Float_t valueF_;
    Int_t valueI_;
  };

  typedef std::map<std::string, branchEntryType> branchMap; // key = branch name
  branchMap branches_;

  TTree* ntuple_;

  static int verbosity_;
};

#endif


