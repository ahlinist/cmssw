#include "RecoTauTag/TauAnalysisTools/interface/PFTauEfficiencyAssociator.h"
#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
using namespace edm;
using namespace reco;

PFTauEfficiencyAssociator::PFTauEfficiencyAssociator(const ParameterSet& iConfig)
{
   effSourcesPSet_ = iConfig.getParameter<ParameterSet>("efficiencySources");

   // get list of efficiency parameter sets names
   efficiencySources_= effSourcesPSet_.getParameterNamesForType<ParameterSet>();

   PFTauProducer_    = iConfig.getParameter<InputTag>("PFTauProducer");

   size_t nSources = efficiencySources_.size();
   for(size_t iSource = 0; iSource < nSources; ++iSource)
   {
      produces<EfficiencyMap>(efficiencySources_[iSource]);
   }
}

void
PFTauEfficiencyAssociator::beginJob(const edm::EventSetup& es)
{
   // 22X compatability layer
   beginJob();
}
   

void
PFTauEfficiencyAssociator::beginJob()
{
   // Pass pointers to the kinematic variables
   KineVarPtrs temp_ptrs;
   temp_ptrs.pt = &pt_;
   temp_ptrs.eta = &eta_;
   temp_ptrs.width = &jetwidth_;
   
   setupEfficiencySources(effSourcesPSet_, temp_ptrs);
}

void 
PFTauEfficiencyAssociator::produce(Event& event, const EventSetup& iSetup)
{
   typedef std::vector<pat::LookupTableRecord> lookupRecordVector;

   // list of efficiency lookups aligned w/ the tau collection
   size_t nEfficiencySources = efficiencySources_.size();
   std::vector<lookupRecordVector> efficiencyCollections(nEfficiencySources);

   // get taus & primary vertices
   Handle<reco::PFTauCollection> taus;
   event.getByLabel(PFTauProducer_, taus);

   size_t nTaus = taus->size();

   for( size_t iTau = 0; iTau < nTaus; ++iTau )
   {
      // get the efficiency for this tau
      reco::PFTauRef tauRef(taus, iTau);

      if( tauRef.isNull() )
      {
         throw cms::Exception("PFTauEfficiencyAssociator") 
            << " found null PFTauRef in collection " << PFTauProducer_ << " at index " << iTau;
      }

      // PFTaus have their vertex set to the associated PV
      const PrimaryVertex& tauPV = tauRef->vertex();

      // set the current tau as active and cache relevant quantities
      loadTau(tauRef, tauPV); 

      for( size_t iEff = 0; iEff < nEfficiencySources; ++iEff )
      {
         // get the efficiency info for this tau, this source
         efficiencyCollections[iEff].push_back(getEfficiency(iEff));
      }
   }

   for( size_t iEff = 0; iEff < nEfficiencySources; ++iEff )
   {
      const lookupRecordVector& efficiencies = efficiencyCollections[iEff];
      const std::string& name                = efficiencySources_[iEff];

      std::auto_ptr<EfficiencyMap> output(new EfficiencyMap());
      // associate the Refs to the LookupTableRecords we have added
      EfficiencyMap::Filler filler(*output);

      filler.insert(taus, efficiencies.begin(), efficiencies.end());
      filler.fill();

      // store the output in the event
      event.put(output, name);
   }
}

const reco::PFJetRef& pfJetFromPFTau(const reco::PFTauRef& tauRef)
{
   if( tauRef->pfTauTagInfoRef().isNull() )
   {
      throw cms::Exception("PFTauEfficiencyAssociator") << " attempting to get the PFJet from a PFTau that has a null PFTauTagInfo reference!";
   }

   return tauRef->pfTauTagInfoRef()->pfjetRef();
}

double PFJetWidthExtractor(const reco::PFJetRef& pfJetRef, const reco::Candidate::Point& pv)
{
   // get constituents
   typedef Candidate::LorentzVector FourVector;
   typedef vector<const Candidate*> daughterCollection;
   daughterCollection daughters     = pfJetRef->getJetConstituentsQuick();
   // in 31X plus move to PV corrected physicsP4(pv).pt()
   FourVector jetAxis = pfJetRef->p4();

   double etWeightedDeltaRSum = 0.;
   double sumEt               = 0.;

   for(daughterCollection::const_iterator daughter = daughters.begin(); daughter != daughters.end(); ++daughter)
   {
      etWeightedDeltaRSum += deltaR<FourVector, FourVector>( (*daughter)->p4(), jetAxis );
      sumEt += (*daughter)->et();
   }

   return (sumEt > 0) ? etWeightedDeltaRSum/sumEt : -1;
}

void
PFTauEfficiencyAssociator::loadTau(const reco::PFTauRef& tauRef, const PrimaryVertex& pv)
{
   // get underlying PFJet
   const reco::PFJetRef& jet = pfJetFromPFTau(tauRef);

   pt_ = jet->pt();
   eta_ = fabs(reco::PFJet::physicsEta(pv.Z(), jet->eta())); // correct for DZ
   jetwidth_ = PFJetWidthExtractor(jet, pv);
}





