#ifndef TauAnalysis_BgEstimation_PFTauEfficiencyAssociator_H_
#define TauAnalysis_BgEstimation_PFTauEfficiencyAssociator_H_

/* 
 * \class PFTauEfficiencyAssociator created : Wed Aug  5 11:34:55 PDT 2009
 * revised : , \author Evan K. Friis, (UC Davis)
 *
 * \version $Revision: 1.1.2.1 $
 *
 * Set of classes to produce a set of ValueMaps mapping pat::LookupTableRecords
 * containing estimated efficiency/fake rate to PFTaus for a collection of
 * efficiency sources. The efficiencies must be parameterized by pt, eta, and
 * jet width.  The methods to access the efficiencySource are abstract, and
 * must be implemented in the subclass. 
 *
 * $Id: PFTauEfficiencyAssociator.h,v 1.1.2.1 2009/09/23 03:41:47 friis Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h" 
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/PatCandidates/interface/LookupTableRecord.h"
#include <memory>

using namespace edm;

class PFTauEfficiencyAssociator : public EDProducer {
   public:

      typedef reco::Candidate::Point PrimaryVertex;
      typedef ValueMap<pat::LookupTableRecord> EfficiencyMap;

      // struct to hold the current state of the tau
      struct KineVarPtrs {
         const double* pt;
         const double* eta;
         const double* width;
      };

      explicit PFTauEfficiencyAssociator(const ParameterSet&);
      virtual ~PFTauEfficiencyAssociator(){}

      void beginJob();
      void beginJob(const EventSetup&); //22X compability 
      void produce(Event&, const EventSetup&);

      // Abstract functions to access the desired efficiency source.
      virtual void setupEfficiencySources(const ParameterSet& effSources, const KineVarPtrs& vars) = 0;
      // Retrieve the expected efficiency for source _i given the kinematic variables.
      virtual pat::LookupTableRecord getEfficiency(size_t iSource) = 0;            

      const std::vector<std::string>& efficiencySourceNames() const { return efficiencySources_; };

   private:
      // need to cache this, as we can't call virtual functions from the constructor
      ParameterSet effSourcesPSet_;

      void loadTau(const reco::PFTauRef&, const PrimaryVertex&);
      // Kinematic variables that paremterize eff. for current tau
      double pt_;
      double eta_;
      double jetwidth_;

      InputTag PFTauProducer_;
      InputTag PVProducer_;
      std::vector<std::string> efficiencySources_;
};



#endif
