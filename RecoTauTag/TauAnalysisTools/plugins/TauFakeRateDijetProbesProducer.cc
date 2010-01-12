#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTauTag/TauAnalysisTools/interface/TauFunctionsImpl.h"
#include <algorithm>

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
      double minThresh_;
      TauFunction* func_;
};

TauFakeRateDijetProbesProducer::TauFakeRateDijetProbesProducer(const ParameterSet& pset)
{
   src_ = pset.getParameter<InputTag>("source");
   thresh_ = pset.getParameter<double>("triggerThreshold");
   minThresh_ = pset.getParameter<double>("minimumThreshold");
   string dataType = pset.getParameter<string>("dataType");
   string eval_func = pset.getParameter<string>("expression");

   if(thresh_ < minThresh_)
   {
      throw cms::Exception("Nonsensical Thresholds") << "Trigger threshold should be greater "
         << "than filter threshold!";
   }

   // Get our evaluation function
   func_ = TauHelpers::tauStringFuncFromType(dataType, eval_func);

   // register products
   produces<CandBaseRefVect>("tagObject");
   produces<CandBaseRefVect>("highestPtProbe");
   produces<CandBaseRefVect>("secondHighestPtProbe");
}

namespace {
   // Sorting predicate
   bool refBaseCandPtDescending(const RefToBase<Candidate> &a, const RefToBase<Candidate> &b)
   {
      return (a->pt() > b->pt());
   }
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

   vector<RefToBase<Candidate> > filteredJets;
   size_t nRawJets = sourceView->size();
   // Ensure we have at least two jets - we need a trigger and a probe.
   if( nRawJets > 1 )
   {
      cout << "Starting with " << nRawJets << " jets" << endl;
      // Count how many are possible 'trigger' jets
      size_t nTriggerCandidates = 0;
      for(size_t iJet = 0; iJet < nRawJets; ++iJet)
      {
         RefToBase<Candidate> jet = sourceView->refAt(iJet);
         double pt = (*func_)(jet);
         cout << "Found jet with pt " << pt << endl;
         if( pt > thresh_) 
         {
            // Count how many above threshold
            nTriggerCandidates++;
         }

         if( pt > minThresh_ )
         {
            filteredJets.push_back(jet);
         }
      }

      // sort by pt
      std::sort(filteredJets.begin(), filteredJets.end(), refBaseCandPtDescending);

      switch(nTriggerCandidates) {
         // No jet fired the trigger, do nothing
         case 0:
            break;
            // Only one jet fired, then the highest pt jet is our trigger jet
            //  the second highest pt jet is our probe
         case 1:
            secondHighestPt->push_back(filteredJets[1]);
            tagObject->push_back(filteredJets[0]);
            cout << "Case 1: " << filteredJets[0]->pt() << " " << filteredJets[1]->pt() << endl;
            break;
            // Two jets fired, so the highest Pt jet is our probe, and the 
            //  second highest is the trigger jet
         case 2:
            highestPt->push_back(filteredJets[0]);
            tagObject->push_back(filteredJets[1]);
            cout << "Case 2: " << filteredJets[0]->pt() << " " << filteredJets[1]->pt() << endl;
            break;

            // If 3 or more jets fired, then the third (or 4th etc) is our trigger
            //  jet and the 1st and 2nd are both probe jets
         default:
            highestPt->push_back(filteredJets[0]);
            secondHighestPt->push_back(filteredJets[1]);
            tagObject->push_back(filteredJets[2]);
            cout << "Case 3+: " << filteredJets[0]->pt() << " " << filteredJets[1]->pt() << 
               filteredJets[2]->pt() << endl;
            break;
      }
   } // end - at least two jets

   // store in event
   evt.put(tagObject, "tagObject");
   evt.put(highestPt, "highestPtProbe");
   evt.put(secondHighestPt, "secondHighestPtProbe");

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauFakeRateDijetProbesProducer);
