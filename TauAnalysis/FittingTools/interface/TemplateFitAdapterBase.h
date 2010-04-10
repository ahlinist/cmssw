#ifndef TauAnalysis_FittingTools_TemplateFitAdapterBase_h  
#define TauAnalysis_FittingTools_TemplateFitAdapterBase_h

/** \class TemplateFitAdapterBase
 *
 * Estimate contribution of signal and background processes to final event sample 
 * by fitting shape "templates" for different processes to distribution observed in data
 * (class implements "template" method for data-driven background estimation)
 *
 * NOTE: The TemplateFitAdapterBase class is capable of fitting distributions of multiple different observables simultaneously,
 *       determining the contribution of signal and background processes as the normalization parameters of the templates
 *       that best fit the combination of all observed distributions.
 *
 *       A "workaround" is used to handle statistical uncertainties on the templates,
 *       which are not directly supported by the likelihood function build by the RooFit package
 *       that is used by the TemplateFitAdapterBase class internally to perform fits.
 *
 *       In order to avoid systematically underestimating (overestimating) template shapes with large (small)
 *       statistical uncertainties (cf. http://root.cern.ch/phpBB2/viewtopic.php?p=38815#38815 )
 *       the template shapes determined from background enriched (control) data samples
 *       can by either parametrized by a "smooth" function prior to the fit,
 *       or by fitting the distribution observed in the final event sample simultaneously with the templates
 *       (cf. https://hypernews.cern.ch/HyperNews/CMS/get/ewk/278.html for more details about and discussion of this technique,
 *        originating from BaBar physics analyses)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: TemplateFitAdapterBase.h,v 1.4 2009/12/04 13:45:22 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <TVectorD.h>
#include <TMatrixD.h>
#include <TH1.h>

#include <vector>
#include <string>
#include <map>

class TemplateFitAdapterBase 
{
 public:
  
  explicit TemplateFitAdapterBase(const edm::ParameterSet&);
  virtual ~TemplateFitAdapterBase();

  struct fitRangeEntryType
  {
    std::string name_;
    std::string title_;
    double min_;
    double max_;
  };

  struct fitResultType
  {
    struct normEntryType
    {
      double value_;
      double errUp_;
      double errDown_;
    };

    struct distrEntryType
    {
      std::map<std::string, const TH1*> templates_; // key = processName
      const TH1* data_;
      std::vector<fitRangeEntryType> fitRanges_;
    };

    int status_;

    TVectorD normValues_;
    TMatrixD normCov_;

    std::map<std::string, normEntryType> normalizations_; // key = processName
    std::map<std::string, distrEntryType> distributions_; // key = varName
  };

  struct drawOptionsType
  {
    drawOptionsType(const edm::ParameterSet&);
    ~drawOptionsType() {}
    int lineColor_;
    int lineStyle_;
    int lineWidth_;
  };

  virtual const fitResultType* fit(int, int);

  virtual void saveFitResults();

  virtual void print(std::ostream& stream) const;

  virtual void makeControlPlots();
 
  int getErrors() const { return error_; }

  friend class TemplateHistFitter;

 protected:

//--- auxiliary functions
  virtual void initialize();
  virtual void fitImp(int, int) = 0;
  virtual void unpackFitResults();
  virtual void unpackFitResultsImpSpecific() {}
  virtual void printImpSpecific(std::ostream&) const {}
  virtual void saveFitResultsImpSpecific() {}
  virtual void makeControlPlotsImpSpecific() {}

//--- auxiliary data-structures
  struct data1dType
  {
    data1dType(const std::string&, const std::string&, const std::string&, const std::vector<fitRangeEntryType>&);
    virtual ~data1dType();

    virtual void initialize();
    virtual void fluctuate(bool, bool, double);

    std::string processName_;
    std::string varName_;
    std::string meName_;
    MonitorElement* me_;

    std::vector<fitRangeEntryType> fitRanges_;

    TH1* histogram_;   
    double integral_;
    double fittedIntegral_;
    double fittedFraction_;

    TH1* fluctHistogram_;

    int error_;
  };

  struct dataNdType 
  {
    dataNdType();
    ~dataNdType();

    void addVar(const std::string&, const std::string&, const std::vector<fitRangeEntryType>&);
    void initialize();
    void fluctuate(bool, bool);

    std::vector<std::string> varNames_;

    std::map<std::string, data1dType*> data1dEntries_; // key = varName

    int error_;
  };

  struct model1dType : public data1dType
  {
    struct sysErrType
    {
      std::string fluctName_;

      std::string meName_;
      MonitorElement* me_;
      TH1* histogram_;

      double pullRMS_;
      double pullMin_;
      double pullMax_;

      int fluctMode_;
    };

    model1dType(const std::string&, const std::string&, const std::string&, const std::vector<fitRangeEntryType>&);
    virtual ~model1dType();

    void initialize();
    void fluctuate(bool, bool, double);

    std::vector<sysErrType> sysErrFluctuations_;
  };

  struct modelNdType
  {
    modelNdType(const std::string&);
    ~modelNdType();

    void addVar(const std::string&, const std::string&, const std::vector<fitRangeEntryType>&);
    void initialize();
    void fluctuate(bool, bool);   

    std::string processName_;
    std::vector<std::string> varNames_;

    std::map<std::string, model1dType*> model1dEntries_; // key = varName

    int error_;
  };

//--- configuration parameters 
  std::string dqmDirectory_fitResult_;
  std::string controlPlotsFileName_;

//--- other data-members
  typedef std::vector<std::string> vstring;
  vstring processNames_;
  vstring varNames_;

  std::map<std::string, drawOptionsType*> drawOptions_; // key = processName

  dataNdType* dataNdEntry_;

  typedef std::map<std::string, modelNdType*> modelNdEntryMap;
  modelNdEntryMap modelNdEntries_; // key = processName

  bool isFirstFit_;
  fitResultType* fitResult_;

  int error_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<TemplateFitAdapterBase* (const edm::ParameterSet&)> TemplateHistFitAdapterPluginFactory;

#endif  


