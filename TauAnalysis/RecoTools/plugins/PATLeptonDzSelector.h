#ifndef TauAnalysis_RecoTools_PATLeptonDzSelector_h
#define TauAnalysis_RecoTools_PATLeptonDzSelector_h

/** \class PATLeptonDzSelector
 *
 * Select electrons, muons and tau-jets compatible
 * with originating from primary event vertex
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATLeptonDzSelector.h,v 1.1 2011/04/24 13:20:19 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "TauAnalysis/RecoTools/interface/PATLeptonTrackExtractor.h"

#include <vector>
#include <string>

template <typename T>
class PATLeptonDzSelector
{
  public:
    
    typedef std::vector<T> collection;
  
    explicit PATLeptonDzSelector(const edm::ParameterSet&);
    ~PATLeptonDzSelector();

    typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
    typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }

    void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);
    
    size_t size() const { return selected_.size(); }

  private:
    std::vector<const T*> selected_;

    std::string moduleLabel_;

    edm::InputTag vertexSrc_;

    double dzMax_;

//--- "helper" class for accessing the track
//    of pat::Electrons and pat::Muons 
//    and the "leading" (i.e. highest Pt) track of pat::Taus
    PATLeptonTrackExtractor<T> trackExtractor_;
};

#endif
