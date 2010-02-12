#ifndef TauAnalysis_Core_SysUncertaintyService_h
#define TauAnalysis_Core_SysUncertaintyService_h

 /** \class SysUncertaintyService
  *
  * Book-keeping of current systematic uncertainty
  * (string set by GenericAnalyzer module; read by SystematicsHistManager and SystematicsBinner plugins)
  * and handling of event (re)weights for estimation of systematic uncertainties
  * (currently used for handling of PDF, ISR and FSR uncertainties;
  *  update and getWeight functions to be called by GenericAnalyzer module)
  * 
  * \author Christian Veelken, UC Davis
  *
  * \version $Revision: 1.4 $
  *
  * $Id: SysUncertaintyService.h,v 1.4 2010/01/18 14:22:06 veelken Exp $
  *
  */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <string>
#include <vector>

struct SysUncertaintyService_namespace
{
  static const double defaultEvtReweight_error;
  static const char* regexpParserExpr_array_entry;
  static const char* regexpParserExpr_array_elements;
};

class SysUncertaintyService
{
 public:
  /// constructor 
  explicit SysUncertaintyService(const edm::ParameterSet& cfg, edm::ActivityRegistry&)
  {
    //std::cout << "<SysUncertaintyService::SysUncertaintyService>:" << std::endl;
  
//--- ensure that SysUncertaintyService is singleton
    assert(!gSysUncertaintyService);
    gSysUncertaintyService = this;

    currentSystematic_ = "undefined";
    currentEventReweight_ = -1.;

    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgEvtReweightEntries = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgEvtReweightEntry = cfgEvtReweightEntries.begin();
	  cfgEvtReweightEntry != cfgEvtReweightEntries.end(); ++cfgEvtReweightEntry ) {
      std::string name = cfgEvtReweightEntry->getParameter<std::string>("name");
      //std::cout << " name = " << name << std::endl;
      
      TPRegexp regexpParser_array_entry(SysUncertaintyService_namespace::regexpParserExpr_array_entry);
      TPRegexp regexpParser_array_elements(SysUncertaintyService_namespace::regexpParserExpr_array_elements);

      TString name_tstring = name.data();
      
      edm::InputTag src = cfgEvtReweightEntry->getParameter<edm::InputTag>("src");
      //std::cout << " src = " << src.label() << ", instance = " << src.instance() << std::endl;
      
      std::string sysName = "undefined";
      WeightEntryBase* evtWeightEntry = 0;
      if ( regexpParser_array_entry.Match(name_tstring) == 1 ) {
	TObjArray* subStrings = regexpParser_array_elements.MatchS(name_tstring);
	
	if ( subStrings->GetEntries() == 3 ) {
	  sysName = ((TObjString*)subStrings->At(1))->GetString().Data();
	  unsigned numValues = (unsigned)atoi(((TObjString*)subStrings->At(2))->GetString().Data());
	  evtWeightEntry = new WeightVectorEntry(src, numValues);
	} else {
	  edm::LogError ("SysUncertaintyService") << " Failed to decode name = " << name << " Parameter !!";
	  continue;
	}
      } else {
	sysName = name;
	evtWeightEntry = new WeightEntry(src);
      }
      
      //std::cout << " sysName = " << sysName << std::endl;

      evtReweightEntries_.insert(std::pair<std::string, WeightEntryBase*>(sysName, evtWeightEntry));
    }
  }

  /// destructor
  ~SysUncertaintyService()
  {
    for ( std::map<std::string, WeightEntryBase*>::iterator it = evtReweightEntries_.begin();
	  it != evtReweightEntries_.end(); ++it ) {
      delete it->second;
    }
    
    if ( gSysUncertaintyService == this ) gSysUncertaintyService = 0;
  }

  void update(const std::string& systematic, const edm::Event& evt, const edm::EventSetup& es)
  {
    //std::cout << "<SysUncertaintyService::update>:" << std::endl;

    currentSystematic_ = systematic;

    static TPRegexp regexpParser_array_entry(SysUncertaintyService_namespace::regexpParserExpr_array_entry);
    static TPRegexp regexpParser_array_elements(SysUncertaintyService_namespace::regexpParserExpr_array_elements);

    TString currentSystematic_tstring = currentSystematic_.data();
    
    std::string sysName = "undefined";
    if ( regexpParser_array_entry.Match(currentSystematic_tstring) == 1 ) {
      TObjArray* subStrings = regexpParser_array_elements.MatchS(currentSystematic_tstring);
      
      if ( subStrings->GetEntries() == 3 ) {
	sysName = ((TObjString*)subStrings->At(1))->GetString().Data();
      } else {
	edm::LogError ("update") << " Failed to decode name = " << currentSystematic_ << " of current systematic uncertainty !!";
	currentEventReweight_ = SysUncertaintyService_namespace::defaultEvtReweight_error;
      }
    } else {
      sysName = currentSystematic_;
    }

    //std::cout << " sysName = " << sysName << std::endl;

    std::map<std::string, WeightEntryBase*>::const_iterator evtReweightEntry = evtReweightEntries_.find(sysName);
    if ( evtReweightEntry != evtReweightEntries_.end() ) {
      evtReweightEntry->second->update(evt, es);
      currentEventReweight_ = evtReweightEntry->second->getWeight(currentSystematic_);
    } else {
//--- no reweight defined for current systematic
//    (e.g. current systematic is a shift of the muon Pt;
//     handled by repeating a cut on the shifted value, not by applying a reweight)
      currentEventReweight_ = 1.;
    }
  }

  const std::string& getCurrentSystematic() const
  {
    if ( currentSystematic_ == "undefined" ) {
      edm::LogError ("getCurrentSystematic") << " Current systematic uninitialized; presumable update function has not been called !!";
    }
    return currentSystematic_;
  }  

  static const std::string& getNameCentralValue() 
  {
    static std::string nameCentralValue = "CENTRAL_VALUE";
    return nameCentralValue; 
  }

  double getWeight() const
  {
    if ( currentEventReweight_ == -1. ) {
      edm::LogError ("getReweight") << " Current event reweight uninitialized; presumable update function has not been called !!";
    }
    return currentEventReweight_;
  }

 private:
  static SysUncertaintyService* gSysUncertaintyService;

  std::string currentSystematic_;
  double currentEventReweight_;

  struct WeightEntryBase
  {
    WeightEntryBase(const edm::InputTag& src)
      : src_(src) {}
    virtual ~WeightEntryBase() {}
    virtual void update(const edm::Event&, const edm::EventSetup&) = 0;
    virtual double getWeight(const std::string&) const =  0;
    edm::InputTag src_;
  };

  struct WeightEntry : WeightEntryBase
  {
    WeightEntry(const edm::InputTag& src)
      : WeightEntryBase(src) {}
    virtual ~WeightEntry() {}
    void update(const edm::Event& evt, const edm::EventSetup& es)
    {  
      //std::cout << "<WeightEntry::update>:" << std::endl;
      //std::cout << " src = " << src_.label() << std::endl;

      edm::Handle<double> weight;
      evt.getByLabel(src_, weight);

      //std::cout << " weight = " << (*weight) << std::endl;

      value_ = (*weight);
    }
    double getWeight(const std::string&) const 
    {      
      //std::cout << "<WeightEntry::getWeight>:" << std::endl;
      //std::cout << " value = " << value_ << std::endl;

      return value_;
    }
    double value_;
  };

  struct WeightVectorEntry : WeightEntryBase
  {
    WeightVectorEntry(const edm::InputTag& src, unsigned numValues)
      : WeightEntryBase(src), numValues_(numValues)
    {
      values_.resize(numValues_);
    }
    virtual ~WeightVectorEntry() {}
    void update(const edm::Event& evt, const edm::EventSetup& es)
    {
      //std::cout << "<WeightVectorEntry::update>:" << std::endl;
      //std::cout << " src = " << src_.label() << std::endl;

      edm::Handle<vdouble> weights;
      evt.getByLabel(src_, weights);

      //std::cout << " weights = " << format_vdouble(*weights) << std::endl;

      values_ = (*weights);
      numValues_ = values_.size();
    }
    double getWeight(const std::string& sysName) const
    { 
      //std::cout << "<WeightVectorEntry::getWeight>:" << std::endl;

      static TPRegexp regexpParser_array_entry(SysUncertaintyService_namespace::regexpParserExpr_array_entry);
      static TPRegexp regexpParser_array_elements(SysUncertaintyService_namespace::regexpParserExpr_array_elements);
      
      TString sysName_tstring = sysName.data();
      
      if ( regexpParser_array_entry.Match(sysName_tstring) == 1 ) {
	TObjArray* subStrings = regexpParser_array_elements.MatchS(sysName_tstring);
	
	if ( subStrings->GetEntries() == 3 ) {
	  unsigned index = (unsigned)atoi(((TObjString*)subStrings->At(2))->GetString().Data());

	  //std::cout << " index = " << index << std::endl;
	  
	  if ( index < numValues_ ) {
	    //std::cout << " value = " << values_[index] << std::endl;

	    return values_[index];
	  } else {
	    edm::LogError ("getWeight") << "Invalid index = " << index << " (valid values = 0.." << (numValues_ - 1) << ") !!";
	    return SysUncertaintyService_namespace::defaultEvtReweight_error;
	  }
	} 
      }
  
//--- failure to parse sysName
      edm::LogError ("getWeight") << " Failed to decode sysName = " << sysName << " !!";
      return SysUncertaintyService_namespace::defaultEvtReweight_error;
    }
    typedef std::vector<double> vdouble;
    vdouble values_;
    unsigned numValues_;
  };

  std::map<std::string, WeightEntryBase*> evtReweightEntries_;
  
};
 
#endif
