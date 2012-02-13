#ifndef TauAnalysis_RecoTools_PATLeptonIpSelector_h
#define TauAnalysis_RecoTools_PATLeptonIpSelector_h

/** \class PATLeptonIpSelector
 *
 * Select electrons, muons and tau-jets based
 * on track transverse impact parameter
 * with respect to primary event vertex
 * 
 * \author Konstantinos A. Petridis, Imperial College;
 *  modified by Christian Veelken
 *
 * \version $Revision: 1.7 $
 *
 * $Id: PATLeptonIpSelector.h,v 1.7 2011/10/20 18:15:22 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "TauAnalysis/RecoTools/interface/PATLeptonTrackExtractor.h"

#include <vector>

template <typename T>
class PATLeptonIpSelector
{
  public:
    
    typedef std::vector<T> collection;
  
    explicit PATLeptonIpSelector(const edm::ParameterSet&);
    ~PATLeptonIpSelector();



    typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
    typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }

    void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);
    
    size_t size() const { return selected_.size(); }

  private:
    std::vector<const T*> selected_;

    edm::InputTag vertexSrc_;

    double ipMin_;
    double ipMax_;
    double ipZmax_;
    double ipMinEB_;
    double ipMaxEB_;
    double ipZmaxEB_;
    double ipMinEE_;
    double ipMaxEE_;
    double ipZmaxEE_;

//--- "helper" class for accessing the track
//    of pat::Electrons and pat::Muons 
//    and the "leading" (i.e. highest Pt) track of pat::Taus
    PATLeptonTrackExtractor<T> trackExtractor_;
};

#endif
