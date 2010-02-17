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
  * \version $Revision: 1.5 $
  *
  * $Id: SysUncertaintyService.h,v 1.5 2010/02/12 17:21:32 veelken Exp $
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
#include <stdio.h>

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

    if ( cfg.exists("weights") ) {
      typedef std::vector<edm::ParameterSet> vParameterSet;
      vParameterSet cfgEvtReweightEntries = cfg.getParameter<vParameterSet>("weights");
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

    if ( cfg.exists("sources") ) {
      typedef std::vector<std::string> vstring;
      edm::ParameterSet cfgInputTags = cfg.getParameter<edm::ParameterSet>("sources");
      vstring inputTagLabels = cfgInputTags.getParameterNamesForType<vstring>();
      for ( vstring::const_iterator inputTagLabel = inputTagLabels.begin();
	    inputTagLabel != inputTagLabels.end(); ++inputTagLabel ) {
	//std::cout << " inputTagLabel = " << (*inputTagLabel) << std::endl;

	vstring inputTagEntry = cfgInputTags.getParameter<vstring>(*inputTagLabel);

	if ( (inputTagEntry.size() % 2) != 0 ) {
	  edm::LogError ("SysUncertaintyService") << " Invalid inputTagEntry = " << format_vstring(inputTagEntry) << " Parameter !!";
	  continue;
	}

	unsigned numPatterns = (inputTagEntry.size() / 2);
	for ( unsigned iPattern = 0; iPattern < numPatterns; ++iPattern ) {
	  std::string sysName = inputTagEntry[2*iPattern];
	  //std::cout << " sysName = " << sysName << std::endl;

	  std::string modInputTag = inputTagEntry[2*iPattern + 1];
	  //std::cout << " modInputTag = " << modInputTag << std::endl;

	  std::string regexpPattern = sysName;
	  bool doPatternMatching = false;

	  size_t pos = regexpPattern.find("*");
	  while ( pos != std::string::npos ) {
	    const std::string regexpPattern_any = "[a-zA-Z0-9]*";
	    regexpPattern.replace(pos, 1, regexpPattern_any);
	    doPatternMatching = true;
	    pos = regexpPattern.find("*", pos + regexpPattern_any.length());
	  }

	  //std::cout << " regexpPattern = " << regexpPattern << std::endl;
	  //std::cout << " doPatternMatching = " << doPatternMatching << std::endl;
	    
	  inputTagEntries_[*inputTagLabel][sysName] = new inputTagEntryType(modInputTag, regexpPattern, doPatternMatching);
	}
      }
    }
  }

  /// destructor
  ~SysUncertaintyService()
  {
    for ( std::map<std::string, WeightEntryBase*>::iterator it = evtReweightEntries_.begin();
	  it != evtReweightEntries_.end(); ++it ) {
      delete it->second;
    }

    for ( std::map<std::string, std::map<std::string, inputTagEntryType*> >::iterator it_1 = inputTagEntries_.begin();
	  it_1 != inputTagEntries_.end(); ++it_1 ) {
      for ( std::map<std::string, inputTagEntryType*>::iterator it_2 = it_1->second.begin();
	    it_2 != it_1->second.end(); ++it_2 ) {
	delete it_2->second;
      }
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

  edm::InputTag getInputTag(const edm::InputTag& inputTag) const
  {
    //std::cout << "<SysUncertaintyService::getInputTag>" << std::endl;

    const std::string& currentSystematic = getCurrentSystematic();
    //std::cout << " currentSystematic = " << currentSystematic << std::endl;

    if ( currentSystematic == "undefined" || currentSystematic == getNameCentralValue() ) return inputTag;

    const std::string& inputTagLabel = inputTag.label();
    //std::cout << " inputTagLabel = " << inputTagLabel << std::endl;

    std::map<std::string, std::map<std::string, inputTagEntryType*> >::const_iterator inputTagEntry 
      = inputTagEntries_.find(inputTagLabel);
    if ( inputTagEntry != inputTagEntries_.end() ) {
      //std::cout << "--> found matching inputTagEntry..." << std::endl;

      std::map<std::string, inputTagEntryType*>::const_iterator modInputTag = inputTagEntry->second.find(currentSystematic);
      if ( modInputTag != inputTagEntry->second.end() ) {
//--- specific "modification rule" for current systematic uncertainty found;
//    return modified InputTag according to specific "modification rule"
	//std::cout << "--> found specific 'modification rule'..." << std::endl;
	return edm::InputTag(modInputTag->second->label_, inputTag.instance(), inputTag.process());
      } else {
//--- no specific "modification rule" found; check if there exists 
//    a generic "modification rule" with pattern matching current systematic uncertainty	
	//std::cout << "--> no specific 'modification rule' found, checking for matching generic 'modification rule'..." << std::endl;
	for ( std::map<std::string, inputTagEntryType*>::const_iterator modInputTag = inputTagEntry->second.begin();
	      modInputTag != inputTagEntry->second.end(); ++modInputTag ) {
	  //std::cout << " modInputTag.label = " << modInputTag->second->label_ << std::endl;
	  //std::cout << " modInputTag.pattern = " << modInputTag->second->pattern_ << std::endl;

	  if ( modInputTag->second->pattern_ &&
	       modInputTag->second->pattern_->Match(currentSystematic) ) {
//--- found generic "modification rule" for current systematic uncertainty;
//    return modified InputTag by adding name of current systematic uncertainty 
//    to end of inputTagName passed as function argument 
	    //std::cout << "--> found match with generic 'modification rule'..." << std::endl;

	    std::string modInputTagLabel = composeModuleName(inputTagLabel, currentSystematic);
	    //std::cout << " modInputTagLabel = " << modInputTagLabel << std::endl;

	    return edm::InputTag(modInputTagLabel, inputTag.instance(), inputTag.process());
	  }
	}
      }
    }

//--- no "modification rule" matching inputTagName passed as function argument found;
//    return original InputTag
    return inputTag;
  }

 private:
  std::string composeModuleName(const std::string& inputTagLabel, const std::string& sysName) const
  {
//--- utility function to compose module name by concatenating two strings;
//    if the last character of part_1 is lower-case (upper-case),
//    capitalize (lowercase) the first character of part_2
//
//    WARNING: C++ implementation needs to match exactly its python equivalent,
//             defined in TauAnalysis/CandidateTools/python/tools/composeModuleName.py
//
    std::string modInputTag = inputTagLabel;
    if ( islower(inputTagLabel[inputTagLabel.length() - 1]) || isdigit(inputTagLabel[inputTagLabel.length() - 1]) ) {
      if ( sysName.length() >= 1 ) modInputTag += toupper(sysName[0]);
      if ( sysName.length() >= 2 ) modInputTag += std::string(sysName, 1);
    } else {
      if ( sysName.length() >= 1 ) modInputTag += tolower(sysName[0]);
      if ( sysName.length() >= 2 ) modInputTag += std::string(sysName, 1);
    }

    return modInputTag;
  }

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

  std::map<std::string, WeightEntryBase*> evtReweightEntries_; // key = sysName

  struct inputTagEntryType
  {
    inputTagEntryType(const std::string& modInputTagLabel, const std::string& regexpPattern, bool doPatternMatching)
      : label_(modInputTagLabel),
	pattern_(0)
    {
      if ( doPatternMatching ) pattern_ = new TPRegexp(regexpPattern);
    }
    ~inputTagEntryType()
    {
      delete pattern_;
    }
    std::string label_;
    TPRegexp* pattern_;
  };

  std::map<std::string, std::map<std::string, inputTagEntryType*> > inputTagEntries_; // keys = orig. inputTag, sysName
  
};
 
#endif
