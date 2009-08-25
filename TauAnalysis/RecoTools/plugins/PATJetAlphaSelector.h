#ifndef TauAnalysis_RecoTools_PATJetAlphaSelector_h
#define TauAnalysis_RecoTools_PATJetAlphaSelector_h

/** \class PATJetAlphaSelector
 *
 * Select jets based on the quantity alpha,
 * defined as ratio of sum of charged particle transverse momenta 
 * to sum of charged plus neutral particle transverse momenta
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: PATJetAlphaSelector.h,v 1.4 2009/04/09 14:51:21 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "TauAnalysis/RecoTools/interface/PATJetAlphaExtractor.h"

#include <vector>

class PATJetAlphaSelectorImp
{
  public:
    typedef pat::JetCollection collection;

    explicit PATJetAlphaSelectorImp(const edm::ParameterSet&);
    ~PATJetAlphaSelectorImp();

    std::vector<const pat::Jet*>::const_iterator begin() const { return selected_.begin(); }
    std::vector<const pat::Jet*>::const_iterator end() const { return selected_.end(); }

    void select(const edm::Handle<collection>&, edm::Event&, const edm::EventSetup&);
    
    size_t size() const { return selected_.size(); }

  private:
    std::vector<const pat::Jet*> selected_;

//--- configuration parameters
    double alphaMin_;
    bool applyAlphaMin_;
    double alphaMax_;
    bool applyAlphaMax_;

//--- auxiliary class to compute quantity alpha,
// defined as ratio of sum of charged particle transverse momenta 
// to sum of charged plus neutral particle transverse momenta,
// for a jet
    PATJetAlphaExtractor jetAlphaExtractor_;
};

#endif
