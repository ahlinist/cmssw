#ifndef ElectroWeakAnalysis_MultiBosons_LazyHistoAnalyzer_h
#define ElectroWeakAnalysis_MultiBosons_LazyHistoAnalyzer_h
/** \class LazyHistoAnalyzer
 *
 * Creates histograms defined in config file - lazy-evaluation
 * version of the analyzer in CommonTools/UtilAlgos
 *
 * \author: Benedikt Hegner, DESY
 * 
 * Template parameters:
 * - C : Concrete candidate collection type
 *
 */
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/LazyExpressionHisto.h"

template<typename C>
class LazyHistoAnalyzer : public edm::EDAnalyzer {
public:
  /// constructor from parameter set
  LazyHistoAnalyzer( const edm::ParameterSet& );
  /// destructor
  ~LazyHistoAnalyzer();
  
protected:
  /// process an event
  virtual void analyze( const edm::Event&, const edm::EventSetup& );

private:
  /// label of the collection to be read in
  edm::InputTag src_;
  /// Do we weight events?
  bool usingWeights_;
  /// label of the weight collection (can be null for weights = 1)
  edm::InputTag weights_;
  /// vector of the histograms
  std::vector<LazyExpressionHisto<typename C::value_type>* > vhistograms;
};

template<typename C>
LazyHistoAnalyzer<C>::LazyHistoAnalyzer( const edm::ParameterSet& par ) : 
  src_(par.template getParameter<edm::InputTag>("src")),
  usingWeights_(par.exists("weights")),
  weights_(par.template getUntrackedParameter<edm::InputTag>("weights", edm::InputTag("fake")))
{
   edm::Service<TFileService> fs;
   std::vector<edm::ParameterSet> histograms = 
                                   par.template getParameter<std::vector<edm::ParameterSet> >("histograms");
   std::vector<edm::ParameterSet>::const_iterator it = histograms.begin();
   std::vector<edm::ParameterSet>::const_iterator end = histograms.end();

   // create the histograms from the given parameter sets 
   for (; it!=end; ++it)
   {
      LazyExpressionHisto<typename C::value_type>* hist = new LazyExpressionHisto<typename C::value_type>(*it);
      hist->initialize(fs);
      vhistograms.push_back(hist);
   }   

}

template<typename C>
LazyHistoAnalyzer<C>::~LazyHistoAnalyzer() 
{
   // delete all histograms and clear the vector of pointers
   typename std::vector<LazyExpressionHisto<typename C::value_type>* >::iterator it = vhistograms.begin(); 
   typename std::vector<LazyExpressionHisto<typename C::value_type>* >::iterator end = vhistograms.end();
   for (;it!=end; ++it){
     (*it)->~LazyExpressionHisto<typename C::value_type>();
   }
   vhistograms.clear(); 
}

template<typename C>
void LazyHistoAnalyzer<C>::analyze( const edm::Event& iEvent, const edm::EventSetup& ) 
{
   edm::Handle<C> coll;
   iEvent.getByLabel( src_, coll);
   double weight = 1.0;
   if (usingWeights_) { 
       edm::Handle<double> weightColl;
       iEvent.getByLabel( weights_, weightColl ); 
       weight = *weightColl;
   }

   typename std::vector<LazyExpressionHisto<typename C::value_type>* >::iterator it = vhistograms.begin();
   typename std::vector<LazyExpressionHisto<typename C::value_type>* >::iterator end = vhistograms.end(); 

   for (;it!=end; ++it){
      uint32_t i = 0;
      for( typename C::const_iterator elem=coll->begin(); elem!=coll->end(); ++elem, ++i ) {
         if (!(*it)->fill( *elem, weight, i )) {
            break;
         }
      }
   }
}

#endif
