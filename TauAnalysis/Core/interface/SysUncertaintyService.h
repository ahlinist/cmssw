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
  * \version $Revision: 1.1 $
  *
  * $Id: SysUncertaintyService.h,v 1.1 2010/01/07 13:22:07 veelken Exp $
  *
  */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <string>
#include <vector>

const double defaultEvtReweight_error = -1.;

class SysUncertaintyService
{
 public:
  /// constructor 
  explicit SysUncertaintyService(const edm::ParameterSet& cfg, edm::ActivityRegistry&)
  {
    std::cout << "<SysUncertaintyService::SysUncertaintyService>:" << std::endl;
  
//--- ensure that SysUncertaintyService is singleton
    assert(!gSysUncertaintyService);
    gSysUncertaintyService = this;

    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgEvtReweightEntries = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgEvtReweightEntry = cfgEvtReweightEntries.begin();
	  cfgEvtReweightEntry != cfgEvtReweightEntries.end(); ++cfgEvtReweightEntry ) {
      std::string name = cfgEvtReweightEntry->getParameter<std::string>("name");
      std::cout << " name = " << name << std::endl;
      
      TPRegexp regexpParser_array_entry("[[:alnum:]]+\\[[[:digit:]]+\\]");
      TPRegexp regexpParser_array_elements("([[:alnum:]]+)\\[([[:digit:]]+)\\]");
      
      TString name_tstring = name.data();
      
      edm::InputTag src = cfgEvtReweightEntry->getParameter<edm::InputTag>("src");
      std::cout << " src = " << src.label() << ", instance = " << src.instance() << std::endl;
      
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
    currentSystematic_ = systematic;

    for ( std::map<std::string, WeightEntryBase*>::iterator evtReweightEntry = evtReweightEntries_.begin();
	  evtReweightEntry != evtReweightEntries_.end(); ++evtReweightEntry ) {
      evtReweightEntry->second->update(evt, es);
    }
  }

  const std::string& getCurrentSystematic() const
  {
    return currentSystematic_;
  }  

  double getWeight() const
  {
    static TPRegexp regexpParser_array_entry("[[:alnum:]]+\\[[[:digit:]]+\\]");
    static TPRegexp regexpParser_array_name("([[:alnum:]]+)\\[[[:digit:]]+\\]");
    
    TString currentSystematic_tstring = currentSystematic_.data();
    
    std::string sysName = "undefined";
    if ( regexpParser_array_entry.Match(currentSystematic_tstring) == 1 ) {
      TObjArray* subStrings = regexpParser_array_name.MatchS(currentSystematic_tstring);
      
      if ( subStrings->GetEntries() == 2 ) {
	sysName = ((TObjString*)subStrings->At(1))->GetString().Data();
      } else {
	edm::LogError ("getReweight") << " Failed to decode name = " << currentSystematic_ << " of current systematic uncertainty !!";
	return defaultEvtReweight_error;
      }
    } else {
      sysName = currentSystematic_;
    }

    std::map<std::string, WeightEntryBase*>::const_iterator evtReweightEntry = evtReweightEntries_.find(sysName);
    if ( evtReweightEntry != evtReweightEntries_.end() ) {
      return evtReweightEntry->second->getWeight(currentSystematic_);
    } else {
//--- no reweight defined for current systematic
//    (e.g. current systematic is a shift of the muon Pt;
//     handled by repeating a cut on the shifted value, not by applying a reweight)
      return 1.;
    }
  }

 private:
  static SysUncertaintyService* gSysUncertaintyService;

  std::string currentSystematic_;

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
      edm::Handle<double> weight;
      evt.getByLabel(src_, weight);
      value_ = (*weight);
    }
    double getWeight(const std::string&) const 
    {
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
      edm::Handle<vdouble> weights;
      evt.getByLabel(src_, weights);
      values_ = (*weights);
      numValues_ = values_.size();
    }
    double getWeight(const std::string& sysName) const
    { 
      static TPRegexp regexpParser_array_entry("[[:alnum:]]+\\[[[:digit:]]+\\]");
      static TPRegexp regexpParser_array_index("[[:alnum:]]+\\[([[:digit:]]+)\\]");
      
      TString sysName_tstring = sysName.data();
      
      if ( regexpParser_array_entry.Match(sysName_tstring) == 1 ) {
	TObjArray* subStrings = regexpParser_array_index.MatchS(sysName_tstring);
	
	if ( subStrings->GetEntries() == 2 ) {
	  unsigned index = (unsigned)atoi(((TObjString*)subStrings->At(1))->GetString().Data());
	  
	  if ( index < numValues_ ) {
	    return values_[index];
	  } else {
	    edm::LogError ("getWeight") << "Invalid index = " << index << " (valid values = 0.." << (numValues_ - 1) << ") !!";
	    return defaultEvtReweight_error;
	  }
	} 
      }
  
//--- failure to parse sysName
      edm::LogError ("getWeight") << " Failed to decode sysName = " << sysName << " !!";
      return defaultEvtReweight_error;
    }
    typedef std::vector<double> vdouble;
    vdouble values_;
    unsigned numValues_;
  };

  std::map<std::string, WeightEntryBase*> evtReweightEntries_;
  
};
 
#endif
