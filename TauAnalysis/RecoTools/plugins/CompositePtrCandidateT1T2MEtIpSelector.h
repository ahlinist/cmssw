#ifndef TauAnalysis_RecoTools_CompositePtrCandidateT1T2MEtIpSelector_h
#define TauAnalysis_RecoTools_CompositePtrCandidateT1T2MEtIpSelector_h

/** \class CompositePtrCandidateT1T2MEtIpSelector
 *
 * Select di-tau pairs based on significance 
 * of transverse impact parameter with respect to primary event vertex
 * of tracks associated to tau decay products
 * 
 * \author Giuseppe Cerati;
 *         modified by Christian Veelken
 *
 * \version $Revision: 1.2 $
 *
 * $Id: CompositePtrCandidateT1T2MEtIpSelector.h,v 1.2 2010/09/28 11:23:35 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "TauAnalysis/RecoTools/interface/PATLeptonTrackExtractor.h"

#include <vector>

template <typename T1, typename T2>
class CompositePtrCandidateT1T2MEtIpSelector
{
  public:
    
    typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > collection;
  
    explicit CompositePtrCandidateT1T2MEtIpSelector(const edm::ParameterSet&);
    ~CompositePtrCandidateT1T2MEtIpSelector();

    typename std::vector<const CompositePtrCandidateT1T2MEt<T1,T2> *>::const_iterator begin() const { return selected_.begin(); }
    typename std::vector<const CompositePtrCandidateT1T2MEt<T1,T2> *>::const_iterator end() const { return selected_.end(); }

    void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);
    
    size_t size() const { return selected_.size(); }

  private:
    std::vector<const CompositePtrCandidateT1T2MEt<T1,T2> *> selected_;

    edm::InputTag vertexSrc_;

    double ipSigMin_;
    bool applyIpSigMin_;
    double ipSigMax_;
    bool applyIpSigMax_;

//--- "helper" class for accessing the track
//    of pat::Electrons and pat::Muons 
//    and the "leading" (i.e. highest Pt) track of pat::Taus
    PATLeptonTrackExtractor<T1> trackExtractorLeg1_;
    PATLeptonTrackExtractor<T2> trackExtractorLeg2_;
};

#endif
