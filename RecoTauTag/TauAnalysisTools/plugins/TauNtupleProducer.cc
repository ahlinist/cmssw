#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTauTag/TauAnalysisTools/interface/TauFunctionsImpl.h"
#include "RecoTauTag/TauAnalysisTools/interface/HandleManager.h"

/*
 * TauNtupleProducer
 *
 * Author: Evan K. Friis, UC Davis (friis@physics.ucdavis.edu)
 *
 * Based on NtpProducer
 *
 * Produces EDM-style ntuples, specialized for the tau use-case. The producer
 * builds a set of vector<float>, where each entry in the vector corresponds to
 * a source candidate (a 'row').  The 'columns', or quantities associated to
 * the tau is configurable. The producer has the following options:
 *
 *      o source: an InputTag specifying the collection.  Any collection
 *        compatibile with View<Candidate> is okay.
 *
 *      o alias: specify the basename used when building the branch aliases
 *
 *      o dataType: specify the C++ class type of the collection. Currently
 *        supported: Candidate, PFTau, CaloTau, GenJet, PATTau etc.
 *
 *      o expressions: A PSet containing strings with expressions to apply to
 *        the [source] objects. Example:
 *        expressions = cms.PSet(
 *                       pt = cms.string("pt()"),
 *                       eta = cms.string("eta()"),
 *        )
 *
 *      o discriminators: A PSet of tau discriminators to evaluate on the
 *        current 'row' candidate.  Only valid when [dataType] = PF or CaloTau
 *
 *      o matching: 
 *              > matchingOption: a string specifying none, matched, or
 *                unmatched.  If none, a row will be produced for each
 *                candidate in [source].  If (un)matched a row will be produced
 *                for each candidate that (does not) exists in the [matchingSource]
 *
 *              > matchingSource: Specifies the matching source, if desired.
 *                Should be of type CandViewMatchMap - such as that produced by
 *                TrivialDeltaRViewMatcher.  The src of the matcher should be
 *                [source], and the 'matched' collection should be the MC type
 *                taus.
 *
 *              > matchingType: Specify the C++ class type of the matched
 *                collection
 *
 *              > matched_expressions: Similar to expressions, but will be
 *                evaluated on the truth object matched to the current 'row'
 *                candidate.
 *
 */ 

using namespace edm;
using namespace reco;
using namespace std;

class TauNtupleProducer : public EDProducer {
   public:

      enum MatchingOption
      {
         kMatched,
         kUnmatched,
         kAll 
      };

      explicit TauNtupleProducer(const ParameterSet&);
      virtual ~TauNtupleProducer(){}

      void beginJob();
      void produce(Event&, const EventSetup&);

   private:
      typedef vector<TauFunction*> VTauFunction;
      typedef vector<HandleManager*> VHandles;

      InputTag source_;
      MatchingOption matchingOption_;
      InputTag matchingSrc_;
      string matchingName_;

      CandViewMatchMapHandler* matchViewManager_;

      VHandles handlesToFill_;
      vector<TauFunction*> tauFunctions_;
};


TauNtupleProducer::TauNtupleProducer(const ParameterSet& pset)
{
   // get the source of candidates to make a ntuple out of
   source_ = pset.getParameter<InputTag>("source");
   
   string matchType = pset.getParameter<string>("matchingOption");

   if(matchType == "none")
      matchingOption_ = kAll;
   else if(matchType == "matched")
      matchingOption_ = kMatched;
   else if(matchType == "unmatched")
      matchingOption_ = kUnmatched;
   else
      throw cms::Exception("Bad matchingOption");

   // The base name of all the branches produced.  If not provided, use the input collection name
   string alias;
   if (pset.existsAs<string>("alias"))
      alias = pset.getParameter<string>("alias");
   else
      alias = source_.label();
   alias += "#";

   // Get the matching source if we are using it
   if( matchingOption_ != kAll )
   {
      matchingSrc_ = pset.getParameter<InputTag>("matchingSource");
      matchViewManager_ = new CandViewMatchMapHandler(matchingSrc_);
      // Register the match view in our handle updater
      handlesToFill_.push_back(matchViewManager_);
   }

   // If we are requiring matching, build the expressions to evaluate on the
   // matched objects
   if( matchingOption_ == kMatched )
   {
      alias += "matched#";
      // register the matching index producer - this allows access to the original
      // collection
      matchingName_ = alias + "matchedIndex";
      produces<vector<unsigned short> >(matchingName_).setBranchAlias(matchingName_);
      // Check if we are producing expressions on the matched objects
      if( pset.existsAs<ParameterSet>("matched_expressions") )
      {
         const ParameterSet& matched_expressionsConfig = 
            pset.getParameter<ParameterSet>("matched_expressions");

         vector<string> matched_expressions = 
            matched_expressionsConfig.getParameterNamesForType<string>();

         string matchedType = pset.getParameter<string>("matchedType");

         for(vector<string>::const_iterator matched_expression = matched_expressions.begin();
               matched_expression != matched_expressions.end(); ++matched_expression)
         {
            const Handle<CandViewMatchMap> *matchView = matchViewManager_->get();
            string func_expr = matched_expressionsConfig.getParameter<string>(*matched_expression);

            TauFunction *matched_func = 
               TauHelpers::tauMatchStringFuncFromType(matchedType, func_expr, matchView);

            // Set name
            string function_name = alias + "ref#";
            function_name += *matched_expression;
            matched_func->setName(function_name);
            // Register this in our list of function
            tauFunctions_.push_back(matched_func);
         }
      }
   } else if (matchingOption_ == kUnmatched)
      alias += "unmatched#";

   // Get the C++ class name
   std::string dataType = pset.getParameter<string>("dataType");

   // Parse and build string expressions
   if (pset.existsAs<ParameterSet>("expressions"))
   {
      const ParameterSet& expressionsConfig = pset.getParameter<ParameterSet>("expressions");
      vector<string> expressions = expressionsConfig.getParameterNamesForType<string>();

      for(vector<string>::const_iterator expression = expressions.begin();
            expression != expressions.end(); ++expression)
      {
         // The expression to be evaluated (ie 'pt()')
         string func_expr = expressionsConfig.getParameter<string>(*expression);
         // The generalized abstract base class
         TauFunction* func = TauHelpers::tauStringFuncFromType(dataType, func_expr);
         // Get the specific type of Tau Function for the given data type
         func->setName(alias + *expression);
         tauFunctions_.push_back(func);
      }
   }

   if (pset.existsAs<ParameterSet>("decayModeExpressions"))
   {
      ParameterSet dm_exprs = pset.getParameter<ParameterSet>("decayModeExpressions");
      if (dm_exprs.existsAs<InputTag>("source"))
      {
         // Get DM source
         InputTag dm_source = dm_exprs.getParameter<InputTag>("source");
         // Register this handle in our handle manager so it is loaded on each event
         PFTauDecayModeAssociationHandleManager* dmHandleManager = 
            new PFTauDecayModeAssociationHandleManager(dm_source);
         handlesToFill_.push_back(dmHandleManager);
         // Get DM expressions
         vector<string> dm_expressions = 
            dm_exprs.getParameterNamesForType<string>();
         // Loop over the DM expressions
         for(vector<string>::const_iterator dm_expression = dm_expressions.begin();
               dm_expression != dm_expressions.end(); ++dm_expression)
         {
            const Handle<PFTauDecayModeAssociation> *dmHandle = dmHandleManager->get();
            string expr = dm_exprs.getParameter<string>(*dm_expression);
            TauFunction *dm_func = new TauDecayModeFunction(dmHandle, expr);
            string func_name = alias + "dm#";
            func_name += *dm_expression;
            dm_func->setName(func_name);
            tauFunctions_.push_back(dm_func);
         }
      }
   }

   // Build PFTauDiscriminator Sources
   if (pset.existsAs<ParameterSet>("discriminators"))
   {
      const ParameterSet& discriminatorsConfig = pset.getParameter<ParameterSet>("discriminators");
      vector<string> discriminators = discriminatorsConfig.getParameterNamesForType<InputTag>();

      for(vector<string>::const_iterator discriminator = discriminators.begin();
            discriminator != discriminators.end(); ++discriminator)
      {
         // The source of the discriminator
         InputTag disc_source = discriminatorsConfig.getParameter<InputTag>(*discriminator);
         // Abstract base
         TauFunction *func;
         // Helper class that fetches the Handle from the event at the
         // appropriate time
         HandleManager *handler;
         if (dataType == "PFTau")
         {
            PFTauDiscriminatorHandleManager* temp_handler = new PFTauDiscriminatorHandleManager(disc_source);
            handler = temp_handler; // store persistently as a pointer to the base
            // Build the TauFunction implementation, pointing to the
            // edm::Handle managed by the handler
            func = new PFTauDiscriminatorFunction(temp_handler->get());
         } else if (dataType == "CaloTau")
         {
            CaloTauDiscriminatorHandleManager* temp_handler = new CaloTauDiscriminatorHandleManager(disc_source);
            handler = temp_handler; // store persistently as a pointer to the base
            func = new CaloTauDiscriminatorFunction(temp_handler->get());
         } else
            throw cms::Exception("BadDiscriminatorDataType") << dataType << " unsupported for"
               << " discriminators" << endl;
         func->setName(alias + *discriminator);
         tauFunctions_.push_back(func);
         handlesToFill_.push_back(handler);
      }
   }

   for(VTauFunction::const_iterator func = tauFunctions_.begin();
         func != tauFunctions_.end(); ++ func)
   {
      const string& name = (*func)->name();
      // Register the products
      produces<vector<float> >(name).setBranchAlias(name);
   }
}

void TauNtupleProducer::beginJob()
{
   // do nothging
}

static int isMatched(const RefToBase<Candidate> &recoKey, const CandViewMatchMap& matchingToTruth)
{
   // Check if this reco is a key in the matching
   CandViewMatchMap::const_iterator truthObject = matchingToTruth.find(recoKey);
   if (truthObject != matchingToTruth.end())
   {
      const RefToBase<Candidate> &truthRef = truthObject->val;
      // Return the integer index to the truth collection
      return truthRef.key();
   }
   // Otherwise unmatched, return -1
   return -1;
}

void TauNtupleProducer::produce(Event& evt, const EventSetup& es)
{
   // Get the tau source
   Handle<View<Candidate> > sourceView;
   evt.getByLabel(source_, sourceView);

   // Make the matching output, if necessary
   auto_ptr<vector<unsigned short> > matchingIndexOutput(new vector<unsigned short>());

   // Load all the other handles we need filled (such as discrimiantors etc)
   for(VHandles::iterator handle = handlesToFill_.begin(); 
         handle != handlesToFill_.end(); ++handle)
   {
      (*handle)->fill(evt);
   }

   // Compute the list of taus fufilling the matching requirement
   //  Store the indices of the taus that pass along w/ the index of the 
   //  matched tau (if applicable)
   vector<size_t> matchingResult;
   for(size_t tau_index = 0; tau_index < sourceView->size(); ++tau_index)
   {
      RefToBase<Candidate> my_cand = sourceView->refAt(tau_index);
      // Check if we have any matching requirement
      if (matchingOption_ != kAll)
      {
         int match_result = isMatched(my_cand, **(matchViewManager_->get()));
         if (matchingOption_ == kMatched && match_result >= 0)
         {
            matchingResult.push_back(tau_index);
            // We store the index to the matched object
            matchingIndexOutput->push_back(match_result);
         } 
         else if (matchingOption_ == kUnmatched && match_result == -1)
         {
            matchingResult.push_back(tau_index);
         }
      } else // pass all
         matchingResult.push_back(tau_index);
   }

   if (matchingOption_ == kMatched)
      evt.put(matchingIndexOutput, matchingName_);

   // Loop over each function we are computing and build the output product
   for(VTauFunction::const_iterator iFunc = tauFunctions_.begin();
         iFunc != tauFunctions_.end(); ++ iFunc)
   {
      // Get our function
      assert(*iFunc);
      const TauFunction& func = **iFunc;
      const string& name = func.name();

      // Register our product
      auto_ptr<vector<float> > functionResultToPut(new vector<float>());
      size_t nMatchedTaus = matchingResult.size();
      functionResultToPut->reserve(nMatchedTaus);

      // Loop over the indexs of that taus that passed
      for(size_t matched_index = 0; matched_index < nMatchedTaus; ++matched_index)
      {
         size_t tau_index = matchingResult[matched_index];
         // Get the ref from the original container
         RefToBase<Candidate> my_cand = sourceView->refAt(tau_index);
         // Compute the result
         functionResultToPut->push_back( func(my_cand) );
      }

      // Put the result in the event
      evt.put(functionResultToPut, name);
   }

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauNtupleProducer);

