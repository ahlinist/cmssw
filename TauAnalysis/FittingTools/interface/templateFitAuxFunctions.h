#ifndef TauAnalysis_BgEstimationTools_templateBgEstFitAuxFunctions_h
#define TauAnalysis_BgEstimationTools_templateBgEstFitAuxFunctions_h

#include "DQMServices/Core/interface/DQMStore.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include "TauAnalysis/FittingTools/interface/TemplateFitAdapterBase.h"

#include <TH1.h>
#include <TArrayD.h>
#include <TVectorD.h>
#include <TMatrixD.h>

#include <string>
#include <vector>

enum { kCoherent, kIncoherent };

typedef std::pair<double, double> double_pair;

const std::string meOptionsValue = std::string(meOptionsSeparator).append("a1").append(meOptionsSeparator).append("s1");
const std::string meOptionsErr = std::string(meOptionsSeparator).append("a2").append(meOptionsSeparator).append("s1");
const std::string meOptionsCov = std::string(meOptionsSeparator).append("a1").append(meOptionsSeparator).append("s2");

double getSampledPull(double, double, double);

void sampleHistogram_stat(const TH1*, TH1*, double = -1., bool = false);
void sampleHistogram_sys(TH1*, const TH1*, double, double, double, int);

TArrayD getBinningInRange(const TAxis*, const TemplateFitAdapterBase::fitRangeEntryType* = 0);

double getIntegral(const TH1*, const std::vector<TemplateFitAdapterBase::fitRangeEntryType>* = 0);

void makeHistogramPositive(TH1*);

TH1* makeSubrangeHistogram(const TH1*, const std::vector<TemplateFitAdapterBase::fitRangeEntryType>*);
TH1* makeSerializedHistogram(const TH1*);
TH1* makeConcatenatedHistogram(const std::string&, const std::vector<const TH1*>&, const std::vector<double>* = 0);

void saveMonitorElement_float(DQMStore&, const std::string&, float, const std::string&);
void saveFitParameter(DQMStore&, const std::string&, const std::string&, const std::string&, double, double, double);

void makeControlPlotsNdObsDistribution(const TemplateFitAdapterBase::fitResultType*,
				       const std::map<std::string, TemplateFitAdapterBase::drawOptionsType*>&,
				       const std::string&);
void makeControlPlotsCovariance(const TVectorD&, const TVectorD&, const TMatrixD&, 
				const std::vector<std::string>&, const std::string&, const char*);

void drawErrorEllipse(double, double, double, double, double, double, double, const char*, const char*, const char*);

double compChi2red(const TemplateFitAdapterBase::fitResultType*);

#endif
