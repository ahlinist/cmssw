#ifndef TauAnalysis_CandidateTools_NeuralMtautauNtupleProducer_h  
#define TauAnalysis_CandidateTools_NeuralMtautauNtupleProducer_h

/** \class NeuralMtautauNtupleProducer
 *
 * Produce an Ntuple of various quantities useful 
 * to train neural network for reconstructing the "full" tau+ tau- invariant mass
 * (for cross-check of SVfit performance)
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.3 $
 *
 * $Id: NeuralMtautauNtupleProducer.h,v 1.3 2012/03/08 10:31:49 veelken Exp $
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

class NeuralMtautauNtupleProducer : public edm::EDProducer 
{
 public:
  
  NeuralMtautauNtupleProducer(const edm::ParameterSet&);
  ~NeuralMtautauNtupleProducer();

  void produce(edm::Event&, const edm::EventSetup&);
  void beginJob();

 private:

  void addBranch(const std::string&);
  void addBranch_long(const std::string&); 
  void addBranch_EnPxPyPz(const std::string&);
  void addBranch_PxPy(const std::string&);
  void addBranch_Cov(const std::string&); 

  void printBranches(std::ostream&);

  void setValue(const std::string&, double);
  void setValue_long(const std::string&, unsigned long);
  void setValue_EnPxPyPz(const std::string&, const reco::Candidate::LorentzVector&);
  void setValue_EnPxPyPz(const std::string&, const reco::Candidate::LorentzVector&, double);
  void setValue_PxPy(const std::string&, const reco::Candidate::LorentzVector&);
  void setValue_PxPy(const std::string&, const reco::Candidate::LorentzVector&, double);
  void setValue_Cov(const std::string&, const TMatrixD&);
  void setValue_Cov(const std::string&, const TMatrixD&, double);

  std::string moduleLabel_;

  edm::InputTag srcGenTauPair_;
  edm::InputTag srcGenMEt_;
  edm::InputTag srcGenTaus_;
  edm::InputTag srcGenParticles_;

  edm::InputTag srcRecPFJets_;
  edm::InputTag srcRecPFCandidatesNotWithinJets_;
  edm::InputTag srcRecLeg1_;
  edm::InputTag srcRecLeg2_;
  edm::InputTag srcRecMEt_;
  
  edm::InputTag srcPFMEtCovMatrix_;

  struct branchEntryType
  {
    std::string name_;
    Float_t value_float_;
    ULong64_t value_long_;
  };

  typedef std::map<std::string, branchEntryType> branchMap; // key = branch name
  branchMap branches_;

  TTree* ntuple_;

  static int verbosity_;
};

#endif


