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
  * $Id: SysUncertaintyService.h,v 1.1 2009/03/20 17:05:18 veelken Exp $
  *
  */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <vector>

class SysUncertaintyService
{
 public:
  /// constructor 
  explicit SysUncertaintyService(const edm::ParameterSet&);

  /// destructor
  virtual ~SysUncertaintyService();

  void update(const std::string&, const edm::Event&, const edm::EventSetup&);

  const std::string& getCurrentSystematic() const;

  double getWeight() const;

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
    WeightEntry(const edm::InputTag&);
    virtual ~WeightEntry() {}
    void update(const edm::Event&, const edm::EventSetup&);
    double getWeight(const std::string&) const;
    double value_;
  };

  struct WeightVectorEntry : WeightEntryBase
  {
    WeightVectorEntry(const edm::InputTag&, unsigned);
    virtual ~WeightVectorEntry() {}
    void update(const edm::Event&, const edm::EventSetup&);
    double getWeight(const std::string&) const;
    typedef std::vector<double> vdouble;
    vdouble values_;
    unsigned numValues_;
  };

  std::map<std::string, WeightEntryBase*> evtReweightEntries_;
  
};
 
#endif
