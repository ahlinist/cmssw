#ifndef TauAnalysis_FittingTools_TemplateFitAdapterRooFit_h  
#define TauAnalysis_FittingTools_TemplateFitAdapterRooFit_h

/** \class TemplateFitAdapter_RooFit
 *
 * Adapter plugin to interface the TemplateHistFitter class with RooFit algorithms
 * in order to perform the actual fitting
 *
 * NOTE: A "workaround" is used to handle statistical uncertainties on the templates,
 *       which are not directly supported by the likelihood function build by the RooFit package
 *       that is used by the TemplateFitAdapter_RooFit class internally to perform fits.
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
 * \version $Revision: 1.3 $
 *
 * $Id: TemplateFitAdapter_RooFit.h,v 1.3 2010/05/27 12:41:21 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/FittingTools/interface/TemplateFitAdapterBase.h"
#include "TauAnalysis/FittingTools/interface/SmoothPdfWrapperBase.h"

#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooAbsPdf.h>
#include <RooFitResult.h>
#include <RooCategory.h>
#include <RooSimultaneous.h>

#include <vector>
#include <string>
#include <map>

class TemplateFitAdapter_RooFit : public TemplateFitAdapterBase
{
 public:
  
  explicit TemplateFitAdapter_RooFit(const edm::ParameterSet&);
  ~TemplateFitAdapter_RooFit();

 protected:

//--- auxiliary functions specific to RooFit algorithms
  void fitImp(int, int);
  void unpackFitResultsImpSpecific();
  void saveFitResultsImpSpecific();
  void makeControlPlotsImpSpecific();

 private:

  std::string moduleLabel_;

//--- auxiliary data-structures specific to RooFit algorithms
  struct model1dTypeRooFitSpecific 
  {
    model1dTypeRooFitSpecific(const std::string&, const std::string&, RooRealVar*, RooRealVar*, bool, 
			      bool, const edm::ParameterSet&, bool, const edm::ParameterSet&, 
			      const TemplateFitAdapterBase::model1dType*);
    ~model1dTypeRooFitSpecific();

    void buildPdf();

    std::string processName_;
    std::string varName_;

    const TemplateFitAdapterBase::model1dType* model1dEntryBase_;

    RooRealVar* varRef_;

    RooRealVar* normRef_;
    std::string fittedEventFractionName_;
    RooAbsReal* fittedEventFraction_;
    std::string numEventsFittedName_;
    RooAbsReal* numEventsFitted_;

    std::string pdfName_;
    RooAbsPdf* pdf_;

    bool fitSimultaneously_;
    std::string auxNormTemplateShapeName_;
    RooRealVar* auxNormTemplateShape_;
    std::string auxPdfTemplateShapeSumName_;
    RooAbsPdf* auxPdfTemplateShapeSum_;

    std::string dataHistName_;
    RooDataHist* dataHist_;

//--- data-members for fitting template histograms
//    by smooth functions which then get used instead of the histograms
//    in template fits
    bool applySmoothing_;
    edm::ParameterSet cfgSmoothing_;
    SmoothPdfWrapperBase* auxSmoothPdfWrapper_;
    std::string auxSmoothHistName_;
    TH1* auxSmoothHist_;
    std::string auxSmoothDataHistName_;
    RooDataHist* auxSmoothDataHist_;

//--- data-members for "horizontal morphing" of template histograms
//    between two endpoints, using "linear interpolation of histograms" 
//    algorithm by Alex Read (NIM A 425 (1999) 357-369)
    bool applyHorizontalMorphing_;
    edm::ParameterSet cfgHorizontalMorphing_;
    TH1* auxMorphHist_upperBound_;
    std::string auxMorphDataHistName_upperBound_;
    RooDataHist* auxMorphDataHist_upperBound_;
    std::string auxMorphHistPdfName_upperBound_;
    RooHistPdf* auxMorphHistPdf_upperBound_;
    TH1* auxMorphHist_lowerBound_;
    std::string auxMorphDataHistName_lowerBound_;
    RooDataHist* auxMorphDataHist_lowerBound_;
    std::string auxMorphHistPdfName_lowerBound_;
    RooHistPdf* auxMorphHistPdf_lowerBound_;
    std::string auxMorphPdfCoeffName_;
    RooAbsReal* auxMorphPdfCoeff_;

//--- data-members for simultaneous fits 
//    of templates and distributions observed in data
//    via RooParametricStepFunctions
    TArrayD* pdfBinning_;
    TObjArray* pdfCoeffCollection_;
    RooArgList* pdfCoeffArgs_;
  };

  struct modelNdTypeRooFitSpecific
  {
    modelNdTypeRooFitSpecific(const std::string&, double, bool, double, double);
    ~modelNdTypeRooFitSpecific();

    void addVar(const std::string&, RooRealVar*, bool, 
		bool, const edm::ParameterSet&, bool, const edm::ParameterSet&,
		const TemplateFitAdapterBase::model1dType*);

    virtual void print(std::ostream&) const;

    std::string processName_;

    std::map<std::string, model1dTypeRooFitSpecific*> model1dEntriesImpSpecific_; // key = varName
    
    RooRealVar* norm_;

//--- data-members for adding constraints obtained from dedicated studies
//    on the normalization factors to be determined by the template fit
    bool applyNormConstraint_;
    RooAbsPdf* pdfNormConstraint_;
    RooConstVar* meanNormConstraint_;
    RooConstVar* sigmaNormConstraint_;
  };

//--- auxiliary functions specific to RooFit algorithms
  void initializeImpSpecific();

  void buildFitData();
  void buildFitModel();
  
  RooAbsData* fitData_;

  typedef std::map<std::string, modelNdTypeRooFitSpecific*> modelNdEntryMapImpSpecific;
  modelNdEntryMapImpSpecific modelNdEntriesImpSpecific_; // key = processName

  typedef std::map<std::string, RooAbsPdf*> pdfModelSumMap;
  pdfModelSumMap pdfModelSums_; // key = varName

  RooAbsPdf* fitModel_;

  RooCategory* fitCategories_;

  typedef std::map<std::string, RooRealVar*> varMap; // key = varName
  varMap fitVariables_;

  RooFitResult* fitResultImpSpecific_;
};

#endif  


