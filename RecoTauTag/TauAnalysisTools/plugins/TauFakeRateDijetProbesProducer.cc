#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTauTag/TauAnalysisTools/interface/TauFunctionsImpl.h"

/*
 * TauFakeRateDijetProbesProducer
 *
 * Author: Evan K. Friis (UC Davis), Christian Veelken (UC Davis)
 *
 * Takes View<Candidate> as input and a pt threshold.  Produces two output
 * collections, both of RefToBase<Candidate>:
 *
 *      tagObject
 *      highestPtProbe
 *      secondHighestPtProbe
 *
 *  Where the both are selected from the input collection such that neither is
 *  solely responsible (uncorrelated) for meeting the trigger conditions.  
 *
 *  Examples:
 *
 *  N object(s) in [input] above [threshold]
 *
 *  1  : put [input][1] into secondHighestPtProbe collection
 *
 *  2  : put [input][0] into highestPtProbe collection
 *
 *  3+ : put [input][1] into secondHighestPtProbe
 *       put [input][0] into highestPtProbe
 */

using namespace edm;
using namespace reco;
using namespace std;

class TauFakeRateDijetProbesProducer : public EDProducer {

   public:
      typedef edm::RefToBaseVector<reco::Candidate>  CandBaseRefVect;
      explicit TauFakeRateDijetProbesProducer(const ParameterSet& pset);
      virtual ~TauFakeRateDijetProbesProducer(){}
      void produce(Event&, const EventSetup&);

   private:
      InputTag src_;
      double thresh_;
      TauFunction* func_;
};

TauFakeRateDijetProbesProducer::TauFakeRateDijetProbesProducer(const ParameterSet& pset)
{
   src_ = pset.getParameter<InputTag>("source");
   thresh_ = pset.getParameter<double>("threshold");
   string dataType = pset.getParameter<string>("dataType");
   string eval_func = pset.getParameter<string>("expression");

   // Get our evaluation function
   func_ = TauHelpers::tauStringFuncFromType(dataType, eval_func);

   // register products
   produces<CandBaseRefVect>("tagObject");
   produces<CandBaseRefVect>("highestPtProbe");
   produces<CandBaseRefVect>("secondHighestPtProbe");
}

void
TauFakeRateDijetProbesProducer::produce(Event &evt, const EventSetup &es)
{
   // output products
   auto_ptr<CandBaseRefVect> tagObject(new CandBaseRefVect());
   auto_ptr<CandBaseRefVect> highestPt(new CandBaseRefVect());
   auto_ptr<CandBaseRefVect> secondHighestPt(new CandBaseRefVect());

   Handle<View<Candidate> > sourceView;
   evt.getByLabel(src_, sourceView);

   size_t nRawJets = sourceView->size();
   // Ensure we have at least two jets - we need a trigger and a probe.
   if( nRawJets > 1 )
   {
      // Count how many are possible 'trigger' jets
      size_t nTriggerCandidates = 0;
      RefToBase<Candidate> highestPtJet;
      RefToBase<Candidate> secondPtJet;
      RefToBase<Candidate> thirdPtJet;
      for(size_t iJet = 0; iJet < nRawJets; ++iJet)
      {
         RefToBase<Candidate> jet = sourceView->refAt(iJet);
         double value = (*func_)(jet);
         if( value > thresh_) 
         {
            // Count how many above threshold
            nTriggerCandidates++;
         }

         // Keep track of the highest and second highest jets
         if ( highestPtJet.isNull() || highestPtJet->pt() < value )
         {
            thirdPtJet = secondPtJet;
            secondPtJet = highestPtJet;
            highestPtJet = jet;
         }
      }

      // Make sure at least one jet fired the trigger
      if( nTriggerCandidates > 0 )
      {
         switch(nTriggerCandidates) {
            // Only one jet fired, then the highest pt jet is our trigger jet
            //  the second highest pt jet is our probe
            case 1:
               assert(secondPtJet.isNonnull());
               secondHighestPt->push_back(secondPtJet);
               tagObject->push_back(highestPtJet);
               break;
            // Two jets fired, so the highest Pt jet is our probe, and the 
            //  second highest is the trigger jet
            case 2:
               assert(highestPtJet.isNonnull());
               highestPt->push_back(highestPtJet);
               tagObject->push_back(secondPtJet);
               break;

            // If 3 or more jets fired, then the third (or 4th etc) is our trigger
            //  jet and the 1st and 2nd are both probe jets
            default:
               assert(highestPtJet.isNonnull());
               highestPt->push_back(highestPtJet);
               assert(secondPtJet.isNonnull());
               secondHighestPt->push_back(secondPtJet);
               assert(thirdPtJet.isNonnull());
               tagObject->push_back(thirdPtJet);
               break;
         }
      } // end - At least one trigger candidate
   } // end - at least two jets

   // store in event
   evt.put(tagObject, "tagObject");
   evt.put(highestPt, "highestPtProbe");
   evt.put(secondHighestPt, "secondHighestPtProbe");

}
