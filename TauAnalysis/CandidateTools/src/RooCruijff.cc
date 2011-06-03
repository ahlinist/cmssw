/*****************************************************************************
 * Package: RooRarFit                                                        *
 *    File: $Id: RooCruijff.cc,v 1.2 2011/05/28 09:23:27 friis Exp $   *
 * Authors:                                                                  *
 *    Karsten Koeneke, Massachusetts Institute of Technology, Cambridge, USA *
 *    Vouter Hulbergen                                                       *
 *                                                                           *
 * Copyright (c) 2006, Massachsetts Institute of Technology, Cambridge, USA  *
 *****************************************************************************/

// This is an implementation for the Cruijff function for RooFit

#include "TauAnalysis/CandidateTools/interface/RooCruijff.h"

#include <iostream>
#include <memory>
#include <math.h>

#include "RooRealVar.h"
#include "RooRealConstant.h"
#include "RooAbsData.h"
#include "RooArgSet.h"

#include "TH1.h"


RooCruijff::RooCruijff(const char *name, const char *title,
		       RooAbsReal& _x, RooAbsReal& _m0,
		       RooAbsReal& _sigmaL, RooAbsReal& _sigmaR,
		       RooAbsReal& _alphaL, RooAbsReal& _alphaR)
  :
  RooAbsPdf(name, title),
  x("x", "x", this, _x),
  m0("m0", "m0", this, _m0),
  sigmaL("sigmaL", "sigmaL", this, _sigmaL),
  sigmaR("sigmaR", "sigmaR", this, _sigmaR),
  alphaL("alphaL", "alphaL", this, _alphaL),
  alphaR("alphaR", "alphaR", this, _alphaR)
{
}

RooCruijff::RooCruijff(const RooCruijff& other, const char* name) :
  RooAbsPdf(other, name),
  x("x", this, other.x),
  m0("m0", this, other.m0),
  sigmaL("sigmaL", this, other.sigmaL),
  sigmaR("sigmaR", this, other.sigmaR),
  alphaL("alphaL", this, other.alphaL),
  alphaR("alphaR", this, other.alphaR)
{
}

Double_t RooCruijff::evaluate() const
{
  // build the functional form
  double sigma = 0.0;
  double alpha = 0.0;
  double dx = (x - m0);
  if(dx<0){
    sigma = sigmaL;
    alpha = alphaL;
  } else {
    sigma = sigmaR;
    alpha = alphaR;
  }
  double f = 2*sigma*sigma + alpha*dx*dx ;
  return exp(-dx*dx/f) ;
}

RooArgSet RooCruijff::estimateParameters(RooAbsData& data, double errorFactor) {
  std::cout << "RooCruijff::Estimating parameters" << std::endl;
  std::cout << "Printing x" << std::endl;
  this->Print("v");
  std::cout << "x: " << x << std::endl;
  std::cout << "var: " << x.arg().GetName() << std::endl;

  std::auto_ptr<TH1> histo(data.createHistogram(x.arg().GetName()));
  assert(histo.get());
  std::cout << "Setting m0" << std::endl;
  m0 = histo->GetBinCenter(histo->GetMaximumBin());
  sigmaL = histo->GetRMS();
  sigmaR = histo->GetRMS();
  return RooArgSet(m0.arg(), sigmaR.arg(), sigmaL.arg());
}

ClassImp(RooCruijff)
