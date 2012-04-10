
/** \class testMarkovChainIntegration
 *
 * Test MarkovChainIntegrator class on two simple test cases
 *  (1) one-dimensional Gaussian
 *  (2) two-dimensional Gaussian
 *      with strong correlation between the two variables
 *
 * The 2nd test case is taken from the paper:
 *   "Probabilistic Inference Using Markov Chain Monte Carlo Methods",
 *   R. Neal, http://www.cs.toronto.edu/pub/radford/review.pdf
 *  (section 4.4)
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.9 $
 *
 * $Id: testMarkovChainIntegration.h,v 1.9 2012/04/03 10:17:08 veelken Exp $
 *
 */

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/MarkovChainIntegrator.h"

#include <Math/Functor.h>
#include <TMath.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TBenchmark.h>

#include <iostream>
#include <iomanip>

class FunctorGaussian1d : public ROOT::Math::Functor
{
 public:

  FunctorGaussian1d(double mean, double sigma, int verbosity)
    : mean_(mean),
      sigma_(sigma),
      verbosity_(verbosity)
  {}

  unsigned int NDim() const 
  {
    return 1;
  }

 protected:

  double mean_;
  double sigma_;

  int verbosity_;

 private:

  virtual double DoEval(const double* x) const
  {
    double retVal = TMath::Gaus(x[0], mean_, sigma_, true);
    //if ( verbosity_ ) {
    //  std::cout << "<FunctorGaussian1d::DoEval>:" <<  std::endl;
    //  std::cout << " x = " << x[0] << ": retVal = " << retVal << std::endl;
    //}
    return retVal;
  } 
};

class PlotGaussian1d : public FunctorGaussian1d
{
 public:

  PlotGaussian1d(const std::string& name, const FunctorGaussian1d& functor)
    : FunctorGaussian1d(functor),
      name_(name),
      histogram_(0),
      function_(0)
  {
    double xMin = -2.;
    double xMax = +2.;
    double binWidth = 0.1;
    int nBinsX = TMath::Nint((xMax - xMin)/binWidth);

    std::string histogramName = std::string(name).append("_histogram");
    histogram_ = new TH1D(histogramName.data(), histogramName.data(), nBinsX, xMin, xMax);

    std::string functionName = std::string(name).append("_function");
    std::string functionFormula = Form("(%f/0.68)*TMath::Gaus(x, %f, %f, 1)", binWidth, mean_, sigma_);
    function_ = new TF1(functionName.data(), functionFormula.data(), xMin, xMax);
  }
  ~PlotGaussian1d()
  {
    std::string canvasName = std::string(name_).append("_canvas");
    TCanvas* canvas = new TCanvas(canvasName.data(), canvasName.data(), 800, 600);
    canvas->SetFillColor(10);
    canvas->SetBorderSize(2);
  
    if ( histogram_->Integral() > 0. ) {
      if ( !histogram_->GetSumw2N() ) histogram_->Sumw2();
      histogram_->Scale(1./histogram_->Integral());
    }

    histogram_->SetStats(false);
    histogram_->SetTitle("");
    histogram_->SetMinimum(0.);
    histogram_->SetMaximum(1.2e-1);
    histogram_->SetMarkerStyle(20);
    
    histogram_->Draw("axis");

    function_->SetLineColor(2);
    function_->SetLineWidth(2);
    function_->Draw("Lsame");

    histogram_->Draw("e1psame");

    canvas->Update();
    std::string outputFileName = Form("testMarkovChainIntegration_%s.eps", name_.data());
    canvas->Print(outputFileName.data());

    delete canvas;
  }
  
 private:

  virtual double DoEval(const double* x) const
  {
    //std::cout << "<PlotGaussian1d::DoEval>: filling x = " << x[0] << std::endl;
    histogram_->Fill(x[0]);
    return 0.;
  } 

  std::string name_;

  TH1* histogram_;
  TF1* function_;
};

class FunctorGaussian2d : public ROOT::Math::Functor
{
 public:

  FunctorGaussian2d(double meanX, double meanY, double sigmaX, double sigmaY, double rho, int verbosity)
    : mean_(2),
      cov_(2, 2),
      covInverse_(2, 2),
      verbosity_(verbosity)
  {
    mean_(0) = meanX;
    mean_(1) = meanY;

    cov_(0, 0) = sigmaX*sigmaX;
    cov_(0, 1) = sigmaX*sigmaY*rho;
    cov_(1, 0) = cov_(0, 1);
    cov_(1, 1) = sigmaY*sigmaY;
    //std::cout << "cov:" << std::endl;
    //cov_.Print();

    det_ = cov_.Determinant();
    //std::cout << "det = " << det_ << std::endl;

    if ( det_ == 0. )
      throw cms::Exception("FunctorGaussian2d")
	<< "Failed to invert covariance matrix, det = " << cov_.Determinant() << " !!\n";
    covInverse_ = cov_;
    covInverse_.Invert();
    //std::cout << "covInverse:" << std::endl;
    //covInverse_.Print();

    normFactor_ = 1./(2.*TMath::Pi()*TMath::Sqrt(TMath::Abs(det_)));
    //std::cout << "normFactor = " << normFactor_ << std::endl;
  }

  unsigned int NDim() const 
  {
    return 2;
  }

 protected:

  TVectorD mean_;
  TMatrixD cov_;
  TMatrixD covInverse_;
  mutable TVectorD residual_;
  double det_;
  double normFactor_;

  int verbosity_;

 private:

  virtual double DoEval(const double* x) const
  {
    double residualX = x[0] - mean_(0);
    double residualY = x[1] - mean_(1);
    double retVal = normFactor_*TMath::Exp(-0.5*(covInverse_(0, 0)*residualX*residualX 
                                            + 2.*covInverse_(0, 1)*residualX*residualY 
                     		               + covInverse_(1, 1)*residualY*residualY));
    //if ( verbosity_ ) {
    //  std::cout << "<FunctorGaussian2d::DoEval>:" <<  std::endl;
    //  std::cout << " x = " << x[0] << ", y = " << x[1] << ": retVal = " << retVal << std::endl;
    //}
    return retVal;
  } 
};

class PlotGaussian2d : public FunctorGaussian2d
{
 public:

  PlotGaussian2d(const std::string& name, const FunctorGaussian2d& functor)
    : FunctorGaussian2d(functor),
      name_(name),
      histogram_(0),
      function_(0)
  {
    double xMin = -12.;
    double xMax = +12.;
    int nBinsX = TMath::Nint(10*(xMax - xMin));
    double yMin = -12.;
    double yMax = +12.;
    int nBinsY = TMath::Nint(10*(yMax - yMin));

    std::string histogramName = std::string(name).append("_histogram");
    histogram_ = new TH2D(histogramName.data(), histogramName.data(), nBinsX, xMin, xMax, nBinsY, yMin, yMax);

    std::string functionName = std::string(name).append("_function");
    std::string functionFormula = 
      Form("%f*TMath::Gaus(%f*(x-%f)*(x-%f) + 2.*%f*(x-%f)*(y-%f) + %f*(y-%f)*(y-%f), 0., 1., 0)", 
	   normFactor_,
	   covInverse_(0, 0), mean_(0), mean_(0), covInverse_(0, 1), mean_(0), mean_(1), covInverse_(1, 1), mean_(1), mean_(1));
    function_ = new TF2(functionName.data(), functionFormula.data(), xMin, xMax, yMin, yMax);
  }
  ~PlotGaussian2d()
  {    
    std::string canvasName = std::string(name_).append("_canvas");
    TCanvas* canvas = new TCanvas(canvasName.data(), canvasName.data(), 1200, 600);
    canvas->SetFillColor(10);
    canvas->SetBorderSize(2);

    std::string leftPadName = std::string(name_).append("_leftPad");
    TPad* leftPad = new TPad(leftPadName.data(), leftPadName.data(), 0.00, 0.00, 0.50, 1.00);
    leftPad->SetFillColor(10);

    std::string rightPadName = std::string(name_).append("_rightPad");
    TPad* rightPad = new TPad(rightPadName.data(), rightPadName.data(), 0.50, 0.00, 1.00, 1.00);
    rightPad->SetFillColor(10);

    canvas->cd();
    leftPad->Draw();
    leftPad->cd();
						    
    histogram_->SetStats(false);
    histogram_->SetTitle("Markov Chain");
    
    gStyle->SetPalette(1,0);
    histogram_->Draw("COL");

    canvas->cd();
    rightPad->Draw();
    rightPad->cd();

    function_->Draw("COL");

    canvas->Update();
    std::string outputFileName = Form("testMarkovChainIntegration_%s.eps", name_.data());
    canvas->Print(outputFileName.data());

    delete leftPad;
    delete rightPad;
    delete canvas;
  }
  
 private:

  virtual double DoEval(const double* x) const
  {
    histogram_->Fill(x[0], x[1]);
    return 0.;
  } 

  std::string name_;

  double xMin_;
  double xMax_;
  double yMin_;
  double yMax_;

  TH2* histogram_;
  TF2* function_;
};

int main(int argc, char* argv[]) 
{
//--- suppress graphics windows popping-up 
//    when TCanvas objects are drawn
  gROOT->SetBatch(true);

  int verbosity = 0;

//--- compute integral over 1d Gaussian in range x = -1sigma..+1sigma
  FunctorGaussian1d integrandGaus1d(0., 1., verbosity);
  std::vector<double> xMinGaus1d(1);
  xMinGaus1d[0] = -1.;
  std::vector<double> xMaxGaus1d(1);
  xMaxGaus1d[0] = +1.;
  //
  // CV: the expected value of the integral is (for sigma = 1.0, <x> = 0.0)
  //
  //               +1
  //       integral   (1/(sqrt(2 pi) sigma) e^(-0.5*((x - <x>)/sigma)^2))^2 dx
  //               -1
  //      ---------------------------------------------------------------------
  //
  //               +1
  //       integral   1/(sqrt(2 pi) sigma) e^(-0.5*((x - <x>)/sigma)^2) dx
  //               -1
  //
  //    = 0.348213 (computed with Mathematica)
  //
  const double integralGaus1d_expected = 0.348213;

  TBenchmark* benchmarkGaus1d_Metropolis = new TBenchmark();
  benchmarkGaus1d_Metropolis->Start("Gaus1d_Metropolis");
  edm::ParameterSet cfgGaus1d_Metropolis;
  cfgGaus1d_Metropolis.addParameter<std::string>("mode", "Metropolis");
  cfgGaus1d_Metropolis.addParameter<std::string>("initMode", "uniform");
  cfgGaus1d_Metropolis.addParameter<unsigned>("numIterBurnin", 1500);
  cfgGaus1d_Metropolis.addParameter<unsigned>("numIterSampling", 500);
  cfgGaus1d_Metropolis.addParameter<unsigned>("numIterSimAnnealingPhase1", 100);
  cfgGaus1d_Metropolis.addParameter<unsigned>("numIterSimAnnealingPhase2", 1300);
  cfgGaus1d_Metropolis.addParameter<double>("T0", 15.);
  cfgGaus1d_Metropolis.addParameter<double>("alpha", 0.995);
  cfgGaus1d_Metropolis.addParameter<unsigned>("numChains", 10);
  cfgGaus1d_Metropolis.addParameter<unsigned>("numBatches", 10);
  cfgGaus1d_Metropolis.addParameter<unsigned>("L", 1);
  cfgGaus1d_Metropolis.addParameter<double>("epsilon0", 5.e-3);
  cfgGaus1d_Metropolis.addParameter<double>("nu", 0.71);
  cfgGaus1d_Metropolis.addParameter<int>("verbosity", verbosity);
  MarkovChainIntegrator integratorGaus1d_Metropolis(cfgGaus1d_Metropolis);
  integratorGaus1d_Metropolis.setIntegrand(integrandGaus1d);
  PlotGaussian1d* plotGaus1d_Metropolis = new PlotGaussian1d("Gaus1d_Metropolis", integrandGaus1d);
  integratorGaus1d_Metropolis.registerCallBackFunction(*plotGaus1d_Metropolis);
  double integralGaus1d_Metropolis, integralErrGaus1d_Metropolis;
  integratorGaus1d_Metropolis.integrate(xMinGaus1d, xMaxGaus1d, integralGaus1d_Metropolis, integralErrGaus1d_Metropolis);
  std::cout << "Metropolis(Gaus1d):" << std::endl;
  std::cout << " integral(-1..+1) = " << integralGaus1d_Metropolis << " +/- " << integralErrGaus1d_Metropolis
	    << " (expected = " << integralGaus1d_expected << ")" << std::endl;
  benchmarkGaus1d_Metropolis->Show("Gaus1d_Metropolis");
  delete benchmarkGaus1d_Metropolis;
  delete plotGaus1d_Metropolis;
  std::cout << std::endl;
  
  TBenchmark* benchmarkGaus1d_Hybrid = new TBenchmark();
  benchmarkGaus1d_Hybrid->Start("Gaus1d_Hybrid");
  edm::ParameterSet cfgGaus1d_Hybrid;
  cfgGaus1d_Hybrid.addParameter<std::string>("mode", "Hybrid");
  cfgGaus1d_Hybrid.addParameter<std::string>("initMode", "uniform");
  cfgGaus1d_Hybrid.addParameter<unsigned>("numIterBurnin", 1500);
  cfgGaus1d_Hybrid.addParameter<unsigned>("numIterSampling", 500);
  cfgGaus1d_Hybrid.addParameter<unsigned>("numIterSimAnnealingPhase1", 100);
  cfgGaus1d_Hybrid.addParameter<unsigned>("numIterSimAnnealingPhase2", 1300);
  cfgGaus1d_Hybrid.addParameter<double>("T0", 15.);
  cfgGaus1d_Hybrid.addParameter<double>("alpha", 0.95);
  cfgGaus1d_Hybrid.addParameter<unsigned>("numChains", 10);
  cfgGaus1d_Hybrid.addParameter<unsigned>("numBatches", 10);
  cfgGaus1d_Hybrid.addParameter<unsigned>("L", 10);
  cfgGaus1d_Hybrid.addParameter<double>("epsilon0", 5.e-4);
  cfgGaus1d_Hybrid.addParameter<double>("nu", 0.71);
  cfgGaus1d_Hybrid.addParameter<int>("verbosity", verbosity);
  MarkovChainIntegrator integratorGaus1d_Hybrid(cfgGaus1d_Hybrid);
  integratorGaus1d_Hybrid.setIntegrand(integrandGaus1d);
  PlotGaussian1d* plotGaus1d_Hybrid = new PlotGaussian1d("Gaus1d_Hybrid", integrandGaus1d);
  integratorGaus1d_Hybrid.registerCallBackFunction(*plotGaus1d_Hybrid);
  double integralGaus1d_Hybrid, integralErrGaus1d_Hybrid;
  integratorGaus1d_Hybrid.integrate(xMinGaus1d, xMaxGaus1d, integralGaus1d_Hybrid, integralErrGaus1d_Hybrid);
  std::cout << "Hybrid(Gaus1d):" << std::endl;
  std::cout << " integral(-1..+1) = " << integralGaus1d_Hybrid << " +/- " << integralErrGaus1d_Hybrid 
	    << " (expected = " << integralGaus1d_expected << ")" << std::endl;
  benchmarkGaus1d_Hybrid->Show("Gaus1d_Hybrid");
  delete benchmarkGaus1d_Hybrid;
  delete plotGaus1d_Hybrid;
  std::cout << std::endl;

//--- compute integral over 2d Gaussian in range x = -10sigma..+10sigma, y = -10sigma..+10sigma,
  FunctorGaussian2d integrandGaus2d(0., 0., 1., 1., 0.99, verbosity);
  std::vector<double> xMinGaus2d(2);
  xMinGaus2d[0] = -10.;
  xMinGaus2d[1] = -10.;
  std::vector<double> xMaxGaus2d(2);
  xMaxGaus2d[0] = +10.;
  xMaxGaus2d[1] = +10.;
  //
  // CV: the expected value of the integral is (for sigmaX = 1.0, <x> = 0.0, sigmaY = 1.0, <y> = 0.0, rho = 0.99)
  //
  //               +10 +10
  //       integral        (1/(2 pi sqrt(det V)) e^(-0.5*{x - <x>, y - <y>}^T V^-1 {x - <x>, y - <y>}))^2 dx dy
  //               -10 -10
  //      -------------------------------------------------------------------------------------------------------
  //               +10 +10
  //       integral        1/(2 pi sqrt(det V)) e^(-0.5*{x - <x>, y - <y>}^T V^-1 {x - <x>, y - <y>}) dx dy
  //               -10 -10
  //
  //     with V = {{sigmaX^2, rho*sigmX*sigmaY}, {rho*sigmX*sigmaY, sigmaY^2}}
  //
  //    = 0.608513 (computed with Mathematica)
  //
  const double integralGaus2d_expected = 0.608513;

  TBenchmark* benchmarkGaus2d_Metropolis = new TBenchmark();
  benchmarkGaus2d_Metropolis->Start("Gaus2d_Metropolis");
  edm::ParameterSet cfgGaus2d_Metropolis;
  cfgGaus2d_Metropolis.addParameter<std::string>("mode", "Metropolis");
  cfgGaus2d_Metropolis.addParameter<std::string>("initMode", "uniform");
  cfgGaus2d_Metropolis.addParameter<unsigned>("numIterBurnin", 15000);
  cfgGaus2d_Metropolis.addParameter<unsigned>("numIterSampling", 50000);
  cfgGaus2d_Metropolis.addParameter<unsigned>("numIterSimAnnealingPhase1", 1000);
  cfgGaus2d_Metropolis.addParameter<unsigned>("numIterSimAnnealingPhase2", 13000);
  cfgGaus2d_Metropolis.addParameter<double>("T0", 15.);
  cfgGaus2d_Metropolis.addParameter<double>("alpha", 0.9995);
  cfgGaus2d_Metropolis.addParameter<unsigned>("numChains", 10);
  cfgGaus2d_Metropolis.addParameter<unsigned>("numBatches", 10);
  cfgGaus2d_Metropolis.addParameter<unsigned>("L", 1);
  cfgGaus2d_Metropolis.addParameter<double>("epsilon0", 1.e-2);
  cfgGaus2d_Metropolis.addParameter<double>("nu", 0.71);
  cfgGaus2d_Metropolis.addParameter<int>("verbosity", verbosity);
  MarkovChainIntegrator integratorGaus2d_Metropolis(cfgGaus2d_Metropolis);
  integratorGaus2d_Metropolis.setIntegrand(integrandGaus2d);
  PlotGaussian2d* plotGaus2d_Metropolis = new PlotGaussian2d("Gaus2d_Metropolis", integrandGaus2d);
  integratorGaus2d_Metropolis.registerCallBackFunction(*plotGaus2d_Metropolis);
  double integralGaus2d_Metropolis, integralErrGaus2d_Metropolis;
  integratorGaus2d_Metropolis.integrate(xMinGaus2d, xMaxGaus2d, integralGaus2d_Metropolis, integralErrGaus2d_Metropolis);
  std::cout << "Metropolis(Gaus2d):" << std::endl;
  std::cout << " integral(-10..+10, -10..+10) = " << integralGaus2d_Metropolis << " +/- " << integralErrGaus2d_Metropolis
	    << " (expected = " << integralGaus2d_expected << ")" << std::endl;
  benchmarkGaus2d_Metropolis->Show("Gaus2d_Metropolis");
  delete benchmarkGaus2d_Metropolis;
  delete plotGaus2d_Metropolis;
  std::cout << std::endl;

  TBenchmark* benchmarkGaus2d_Hybrid = new TBenchmark();
  benchmarkGaus2d_Hybrid->Start("Gaus2d_Hybrid");
  edm::ParameterSet cfgGaus2d_Hybrid;
  cfgGaus2d_Hybrid.addParameter<std::string>("mode", "Hybrid");
  cfgGaus2d_Hybrid.addParameter<std::string>("initMode", "uniform");
  cfgGaus2d_Hybrid.addParameter<unsigned>("numIterBurnin", 1500);
  cfgGaus2d_Hybrid.addParameter<unsigned>("numIterSampling", 5000);
  cfgGaus2d_Hybrid.addParameter<unsigned>("numIterSimAnnealingPhase1", 100);
  cfgGaus2d_Hybrid.addParameter<unsigned>("numIterSimAnnealingPhase2", 1300);
  cfgGaus2d_Hybrid.addParameter<double>("T0", 15.);
  cfgGaus2d_Hybrid.addParameter<double>("alpha", 0.995);
  cfgGaus2d_Hybrid.addParameter<unsigned>("numChains", 10);
  cfgGaus2d_Hybrid.addParameter<unsigned>("numBatches", 10);
  cfgGaus2d_Hybrid.addParameter<unsigned>("L", 200);
  cfgGaus2d_Hybrid.addParameter<double>("epsilon0", 2.5e-4);
  cfgGaus2d_Hybrid.addParameter<double>("nu", 0.71);
  cfgGaus2d_Hybrid.addParameter<int>("verbosity", verbosity);
  MarkovChainIntegrator integratorGaus2d_Hybrid(cfgGaus2d_Hybrid);
  integratorGaus2d_Hybrid.setIntegrand(integrandGaus2d);
  PlotGaussian2d* plotGaus2d_Hybrid = new PlotGaussian2d("Gaus2d_Hybrid", integrandGaus2d);
  integratorGaus2d_Hybrid.registerCallBackFunction(*plotGaus2d_Hybrid);
  double integralGaus2d_Hybrid, integralErrGaus2d_Hybrid;
  integratorGaus2d_Hybrid.integrate(xMinGaus2d, xMaxGaus2d, integralGaus2d_Hybrid, integralErrGaus2d_Hybrid);
  std::cout << "Hybrid(Gaus2d):" << std::endl;
  std::cout << " integral(-10..+10, -10..+10) = " << integralGaus2d_Hybrid << " +/- " << integralErrGaus2d_Hybrid 
	    << " (expected = " << integralGaus2d_expected << ")" << std::endl;
  benchmarkGaus2d_Hybrid->Show("Gaus2d_Hybrid");
  delete benchmarkGaus2d_Hybrid;
  delete plotGaus2d_Hybrid;
  std::cout << std::endl;

  return 0;
}
