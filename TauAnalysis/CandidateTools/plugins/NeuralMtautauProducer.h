#ifndef TauAnalysis_CandidateTools_NeuralMtautauProducer_h
#define TauAnalysis_CandidateTools_NeuralMtautauProducer_h

/** \class NeuralMtautauProducer
 *
 * Reconstruct tau+ tau- invariant mass using neural network
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NeuralMtautauProducer.h,v 1.1 2011/04/16 07:44:48 liis Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/AssociationVector.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Handle.h"

#include "TauAnalysis/CandidateTools/interface/NeuralMtautauAlgorithm.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include <TMatrixD.h>

template<typename T1, typename T2>
class NeuralMtautauProducer : public edm::EDProducer 
{
 public:

  typedef CompositePtrCandidateT1T2MEt<T1,T2> CompositePtrCandidate;
  typedef std::vector<CompositePtrCandidate> CompositePtrCandidateCollection;
  typedef edm::RefProd<CompositePtrCandidateCollection> CompositePtrCandidateRefProd;
  typedef std::vector<double> MtautauCollection;
  typedef edm::AssociationVector<CompositePtrCandidateRefProd, MtautauCollection> diTauToMtautauAssociation;

  explicit NeuralMtautauProducer(const edm::ParameterSet& cfg)
    : algorithm_(cfg)
  {
    src_ = cfg.getParameter<edm::InputTag>("src");

    produces<MtautauCollection>();
    produces<diTauToMtautauAssociation>();
  }
  ~NeuralMtautauProducer() {}

  void produce(edm::Event& evt, const edm::EventSetup& es)
  {
    edm::Handle<CompositePtrCandidateCollection> diTauCollection;
    evt.getByLabel(src_, diTauCollection);

    std::auto_ptr<MtautauCollection> mTauTauCollection(new MtautauCollection);

    // association between diTau candidates and computed mTauTau values
    std::auto_ptr<diTauToMtautauAssociation> mTauTauAssociations(new diTauToMtautauAssociation(CompositePtrCandidateRefProd(diTauCollection)));

    edm::RefProd<MtautauCollection> mTauTauRefProd = evt.getRefBeforePut<MtautauCollection>();
 
    size_t numDiTauCandidates = diTauCollection->size();
    for ( size_t iDiTauCandidate = 0; iDiTauCandidate < numDiTauCandidates; ++iDiTauCandidate ) {
      const CompositePtrCandidate& diTauCandidate = diTauCollection->at(iDiTauCandidate);

      if ( !diTauCandidate.hasMEtSignMatrix() )
	throw cms::Exception("NeuralMtautauProducer::produce") 
	  << "DiTauCandidate has no MET covariance matrix !!\n";
      
      double metPx = diTauCandidate.met()->px();
      double metPy = diTauCandidate.met()->py();
      double mTauTau = 
	algorithm_(diTauCandidate.leg1()->p4(), diTauCandidate.leg2()->p4(), 
		   metPx, metPy, diTauCandidate.metSignMatrix());

      mTauTauCollection->push_back(mTauTau);

      edm::Ref<MtautauCollection> mTauTauRef(mTauTauRefProd, iDiTauCandidate);
      int mTauTau_index = mTauTauRef.index();

      mTauTauAssociations->setValue(iDiTauCandidate, mTauTau_index);
    }
  
    evt.put(mTauTauCollection);
    evt.put(mTauTauAssociations);
  }

 private:

  edm::InputTag src_;

  NeuralMtautauAlgorithm algorithm_;

};

#endif

