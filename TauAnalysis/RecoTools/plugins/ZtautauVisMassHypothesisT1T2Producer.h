#ifndef TauAnalysis_RecoTools_ZtautauVisMassHypothesisT1T2Producer_h
#define TauAnalysis_RecoTools_ZtautauVisMassHypothesisT1T2Producer_h

/** \class ZtautauVisMassHypothesisT1T2Producer
 *
 * Produce hypothesis for visible invariant mass 
 * reconstructed from visible decay products of tau-lepton pair
 * (combined value estimated from CompositePtrCandidateT1T2MEt and ZllHypothesisT1T2 data-formats)
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ZtautauVisMassHypothesisT1T2Producer.h,v 1.1 2009/10/10 12:32:16 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/AssociationVector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

template<typename T1, typename T2>
class ZtautauVisMassHypothesisT1T2Producer : public edm::EDProducer 
{
  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > DiCandidatePairCollection;
  typedef edm::Ref<DiCandidatePairCollection> DiCandidatePairRef;
  typedef edm::RefProd<DiCandidatePairCollection> DiCandidatePairRefProd;
  typedef std::vector<ZllHypothesisT1T2<T1,T2> > ZllHypothesisCollection;
  
  typedef std::vector<float> vfloat;
  typedef edm::AssociationVector<DiCandidatePairRefProd, vfloat> ZtautauVisMassHypothesisCollection;

  struct ZllHypothesisEntry
  {
    ZllHypothesisEntry(const edm::ParameterSet&);
    edm::InputTag src_;
    double minZllMass_;
    double maxZllMass_;
  };

  typedef std::vector<ZllHypothesisEntry> vZllHypothesisEntry;

 public:

  explicit ZtautauVisMassHypothesisT1T2Producer(const edm::ParameterSet&);
  ~ZtautauVisMassHypothesisT1T2Producer();

  void produce(edm::Event&, const edm::EventSetup&);

 private:
  
  edm::InputTag srcDiCandidatePairs_;
  vZllHypothesisEntry ZllHypothesisEntries_;
};

#endif

