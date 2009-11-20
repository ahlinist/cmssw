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

   xAxisFn_ = PFTauStringFuncPtr(new PFTauStringFunc(
            iConfig.getParameter<string>("xAxisFunction")));
   yAxisFn_ = PFTauStringFuncPtr(new PFTauStringFunc(
            iConfig.getParameter<string>("yAxisFunction")));
   zAxisFn_ = PFTauStringFuncPtr(new PFTauStringFunc(
            iConfig.getParameter<string>("zAxisFunction")));

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

void
PFTauEfficiencyAssociator::loadTau(const reco::PFTauRef& tauRef, const PrimaryVertex& pv)
{
   xAxisValue_ = (*xAxisFn_)(*tauRef);
   yAxisValue_ = (*yAxisFn_)(*tauRef);
   zAxisValue_ = (*zAxisFn_)(*tauRef);
}





