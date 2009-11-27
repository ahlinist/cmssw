#ifndef TauAnalysis_BgEstimationTools_templateBgEstFitAuxFunctions_h
#define TauAnalysis_BgEstimationTools_templateBgEstFitAuxFunctions_h

#include "DQMServices/Core/interface/DQMStore.h"

#include "TauAnalysis/FittingTools/interface/TemplateFitAdapterBase.h"

#include <TH1.h>
#include <TArrayD.h>
#include <TVectorD.h>
#include <TMatrixD.h>

#include <string>
#include <vector>

enum { kCoherent, kIncoherent };

typedef std::pair<double, double> double_pair;

double getSampledPull(double, double, double);

void sampleHistogram_stat(const TH1*, TH1*);
void sampleHistogram_sys(TH1*, const TH1*, double, double, double, int);

TArrayD getBinning(const TH1*);

double getIntegral(const TH1*);
double getIntegral(const TH1*, double, double);

void makeHistogramPositive(TH1*);

TH1* makeConcatenatedHistogram(const std::string&, const std::vector<const TH1*>&, 
			       const std::vector<double_pair>&, const std::vector<double>*);

void saveMonitorElement_float(DQMStore&, const char*, float);
void saveFitParameter(DQMStore&, const std::string&, const std::string&, const std::string&, double, double, double);

void makeControlPlotsObsDistribution(const TemplateFitAdapterBase::fitResultType*,
				     const std::map<std::string, TemplateFitAdapterBase::drawOptionsType*>&,
				     const std::string&);
void makeControlPlotsCovariance(const TVectorD&, const TVectorD&, const TMatrixD&, 
				const std::vector<std::string>&, const std::string&, const char*);

void drawErrorEllipse(double, double, double, double, double, double, double, const char*, const char*, const char*);

double compChi2red(const TemplateFitAdapterBase::fitResultType*);

#endif
