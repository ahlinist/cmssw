
/** \executable studySVfitVisPtCuts
 *
 * Study effect of Pt and eta cuts applied on visible decay products
 * on tau+ tau- mass reconstruction
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: studySVfitVisPtCuts.cc,v 1.1 2012/03/06 17:34:42 veelken Exp $
 *
 */

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TMatrixD.h>
#include <TH2.h>
#include <TH1.h>
#include <TBenchmark.h>
#include <TMath.h>
#include <TRandom3.h>

#include <iostream>
#include <string>
#include <vector>

using namespace NSVfitStandalone;
using namespace SVfit_namespace;

struct inputVariableSet
{
  inputVariableSet(TTree* tree, bool runSVfit)
    : recMEtCov_(2,2),
      runSVfit_(runSVfit)
  {
    if ( tree ) {
      tree->SetBranchAddress("genTau1En", &genTau1En_);
      tree->SetBranchAddress("genTau1Px", &genTau1Px_);
      tree->SetBranchAddress("genTau1Py", &genTau1Py_);
      tree->SetBranchAddress("genTau1Pz", &genTau1Pz_);
      tree->SetBranchAddress("genTau1M", &genTau1Mass_);
      tree->SetBranchAddress("genVis1En", &genVis1En_);
      tree->SetBranchAddress("genVis1Px", &genVis1Px_);
      tree->SetBranchAddress("genVis1Py", &genVis1Py_);
      tree->SetBranchAddress("genVis1Pz", &genVis1Pz_);
      tree->SetBranchAddress("genVis1M", &genVis1Mass_);
      tree->SetBranchAddress("genNu1En", &genNu1En_);
      tree->SetBranchAddress("genNu1Px", &genNu1Px_);
      tree->SetBranchAddress("genNu1Py", &genNu1Py_);
      tree->SetBranchAddress("genNu1Pz", &genNu1Pz_);
      tree->SetBranchAddress("genNu1M", &genNu1Mass_);
      
      tree->SetBranchAddress("genTau2En", &genTau2En_);
      tree->SetBranchAddress("genTau2Px", &genTau2Px_);
      tree->SetBranchAddress("genTau2Py", &genTau2Py_);
      tree->SetBranchAddress("genTau2Pz", &genTau2Pz_);
      tree->SetBranchAddress("genTau2M", &genTau2Mass_);
      tree->SetBranchAddress("genVis2En", &genVis2En_);
      tree->SetBranchAddress("genVis2Px", &genVis2Px_);
      tree->SetBranchAddress("genVis2Py", &genVis2Py_);
      tree->SetBranchAddress("genVis2Pz", &genVis2Pz_);
      tree->SetBranchAddress("genVis2M", &genVis2Mass_);
      tree->SetBranchAddress("genNu2En", &genNu2En_);
      tree->SetBranchAddress("genNu2Px", &genNu2Px_);
      tree->SetBranchAddress("genNu2Py", &genNu2Py_);
      tree->SetBranchAddress("genNu2Pz", &genNu2Pz_);
      tree->SetBranchAddress("genNu2M", &genNu2Mass_);
      
      tree->SetBranchAddress("recMEtPx", &recMEtPx_);
      tree->SetBranchAddress("recMEtPy", &recMEtPy_);
      tree->SetBranchAddress("recMEtVxx", &recMEtVxx_);
      tree->SetBranchAddress("recMEtVxy", &recMEtVxy_);
      tree->SetBranchAddress("recMEtVyy", &recMEtVyy_);
      
      tree->SetBranchAddress("genMEtPx", &genMEtPx_);
      tree->SetBranchAddress("genMEtPy", &genMEtPy_);
      
      tree->SetBranchAddress("genMtautau", &genMtautau_);
    }
  }
  ~inputVariableSet() {}

  void initialize()
  {
    genTau1P4_.SetPxPyPzE(genTau1Px_, genTau1Py_, genTau1Pz_, genTau1En_);
    genVis1P4_.SetPxPyPzE(genVis1Px_, genVis1Py_, genVis1Pz_, genVis1En_);
    genNu1P4_.SetPxPyPzE(genNu1Px_, genNu1Py_, genNu1Pz_, genNu1En_);

    genTau2P4_.SetPxPyPzE(genTau2Px_, genTau2Py_, genTau2Pz_, genTau2En_);
    genVis2P4_.SetPxPyPzE(genVis2Px_, genVis2Py_, genVis2Pz_, genVis2En_);
    genNu2P4_.SetPxPyPzE(genNu2Px_, genNu2Py_, genNu2Pz_, genNu2En_);

    recMEtCov_(0,0) = recMEtVxx_;
    recMEtCov_(0,1) = recMEtVxy_;
    recMEtCov_(1,0) = recMEtVxy_;
    recMEtCov_(1,1) = recMEtVyy_;

    if ( runSVfit_ ) {
      svFitMassFromGenInput_ = compSVfitMassFromGenInput();
    } else {
      svFitMassFromGenInput_ = -1.;
    }
  }

  bool passedVisEtaCutsLeg1() const { 
    return (TMath::Abs(genVis1P4_.eta()) < 2.1);
  }
  bool passedVisEtaAndPtCutsLeg1() const { 
    return (passedVisEtaCutsLeg1() && genVis1P4_.pt() > 15);
  }
  bool passedVisEtaCutsLeg2() const { 
    return (TMath::Abs(genVis2P4_.eta()) < 2.3);
  }
  bool passedVisEtaAndPtCutsLeg2() const { 
    return (passedVisEtaCutsLeg2() && genVis2P4_.pt() > 20);
  }
  bool passedVisEtaCutsLeg1and2() const { 
    return (passedVisEtaCutsLeg1() && passedVisEtaCutsLeg2());
  }
  bool passedVisEtaAndPtCutsLeg1and2() const { 
    return (passedVisEtaAndPtCutsLeg1() && passedVisEtaAndPtCutsLeg2());
  }

  double compSVfitMassFromGenInput() const
  {
    //std::cout << "<compSVfitMassFromGenInput>:" << std::endl;
    //std::cout << " genVis1: E = " << genVis1P4_.E() << "," 
    //	        << " Px = " << genVis1P4_.Px() << ", Py = " << genVis1P4_.Py() << ", Pz = " << genVis1P4_.Pz() 
    //	        << " (phi = " << genVis1P4_.phi() << ")" << std::endl;
    //std::cout << " genVis2: E = " << genVis2P4_.E() << "," 
    //	        << " Px = " << genVis2P4_.Px() << ", Py = " << genVis2P4_.Py() << ", Pz = " << genVis2P4_.Pz() 
    //	        << " (phi = " << genVis2P4_.phi() << ")" << std::endl;
    std::vector<MeasuredTauLepton> tauDecayProduts;
    tauDecayProduts.push_back(MeasuredTauLepton(kLepDecay, genVis1P4_));
    tauDecayProduts.push_back(MeasuredTauLepton(kHadDecay, genVis2P4_));
    Vector recMEtP3(genMEtPx_, genMEtPy_, 0.);
    //std::cout << " genMEt: Px = " << genMEtPx_ << ", Py = " << genMEtPy_ 
    //	        << " (phi = " << recMEtP3.phi() << ")" << std::endl;
    TMatrixD pfMEtSignCovMatrix(2,2);
    pfMEtSignCovMatrix(0,0) = recMEtVxx_;
    pfMEtSignCovMatrix(0,1) = recMEtVxy_;
    pfMEtSignCovMatrix(1,0) = recMEtVxy_;
    pfMEtSignCovMatrix(1,1) = recMEtVyy_;
    //std::cout << " pfMEtSignCovMatrix:" << std::endl;
    //pfMEtSignCovMatrix.Print();
    NSVfitStandaloneAlgorithm svFit(tauDecayProduts, recMEtP3, pfMEtSignCovMatrix);
    svFit.fit();
    double svFitMtautau = ( svFit.isValidSolution() ) ?
      svFit.mass() : -1.;
    //std::cout << " svFitMtautau = " << svFitMtautau << std::endl;
    return svFitMtautau;
  }

  Float_t genTau1En_;
  Float_t genTau1Px_;
  Float_t genTau1Py_;
  Float_t genTau1Pz_;
  reco::Candidate::LorentzVector genTau1P4_;
  Float_t genTau1Mass_;
  Float_t genVis1En_;
  Float_t genVis1Px_;
  Float_t genVis1Py_;
  Float_t genVis1Pz_;
  reco::Candidate::LorentzVector genVis1P4_;
  Float_t genVis1Mass_;
  Float_t genNu1En_;
  Float_t genNu1Px_;
  Float_t genNu1Py_;
  Float_t genNu1Pz_;
  reco::Candidate::LorentzVector genNu1P4_;
  Float_t genNu1Mass_;

  Float_t genTau2En_;
  Float_t genTau2Px_;
  Float_t genTau2Py_;
  Float_t genTau2Pz_;
  reco::Candidate::LorentzVector genTau2P4_;
  Float_t genTau2Mass_;
  Float_t genVis2En_;
  Float_t genVis2Px_;
  Float_t genVis2Py_;
  Float_t genVis2Pz_;
  reco::Candidate::LorentzVector genVis2P4_;
  Float_t genVis2Mass_;
  Float_t genNu2En_;
  Float_t genNu2Px_;
  Float_t genNu2Py_;
  Float_t genNu2Pz_;
  reco::Candidate::LorentzVector genNu2P4_;
  Float_t genNu2Mass_;

  Float_t recMEtPx_;
  Float_t recMEtPy_;
  Float_t recMEtVxx_;
  Float_t recMEtVxy_;
  Float_t recMEtVyy_;
  TMatrixD recMEtCov_;

  Float_t genMEtPx_;
  Float_t genMEtPy_;
  
  Float_t genMtautau_;

  bool runSVfit_;
  Float_t svFitMassFromGenInput_; // computed "on-the-fly"
};

struct plotEntryType
{
  plotEntryType(Float_t massPoint, Float_t massWindowLo, Float_t massWindowHi)
    : massPoint_(massPoint),
      massWindowLo_(massWindowLo),
      massWindowHi_(massWindowHi)
  {
    directory_ = Form("massEq%1.0fGeV", massPoint_);
  }
  ~plotEntryType() {}
  void bookHistograms(TFileDirectory& dir)
  {
    TFileDirectory subdir = dir.mkdir(directory_.data());

    histogramLeg1X_ = 
      subdir.make<TH1D>("histogramLeg1X",       
			"histogramLeg1X", 50, 0., 1.);
    histogramLeg1X_fine_ = 
      subdir.make<TH1D>("histogramLeg1X_fine",       
			"histogramLeg1X_fine", 200, 0., 1.);
    histogramLeg1VisMass_ = 
      subdir.make<TH1D>("histogramLeg1VisMass", 
			"histogramLeg1VisMass", 45, 0., 1.8);
    histogramLeg1NuMass_ = 
      subdir.make<TH1D>("histogramLeg1NuMass",  
			"histogramLeg1NuMass", 45, 0., 1.8);
    histogramLeg1NuMassVsX_ = 
      subdir.make<TH2D>("histogramLeg1NuMassVsX" , 
			"histogramLeg1NuMassVsX", 50, 0., 1., 45, 0., 1.8);
    
    histogramLeg1XdPhiLt30_ = 
      subdir.make<TH1D>("histogramLeg1XdPhiLt30" , 
			"histogramLeg1XdPhiLt30", 50, 0., 1.);
    histogramLeg1XdPhi30to60_ = 
      subdir.make<TH1D>("histogramLeg1XdPhi30to60" , 
			"histogramLeg1XdPhi30to60", 50, 0., 1.);
    histogramLeg1XdPhi60to90_ = 
      subdir.make<TH1D>("histogramLeg1XdPhi60to90" , 
			"histogramLeg1XdPhi60to90", 50, 0., 1.);
    histogramLeg1XdPhi90to120_ = 
      subdir.make<TH1D>("histogramLeg1XdPhi90to120" , 
			"histogramLeg1XdPhi90to120", 50, 0., 1.);
    histogramLeg1XdPhi120to140_ = 
      subdir.make<TH1D>("histogramLeg1XdPhi120to140" , 
			"histogramLeg1XdPhi120to140", 50, 0., 1.);
    histogramLeg1XdPhi140to160_ = 
      subdir.make<TH1D>("histogramLeg1XdPhi140to160" , 
			"histogramLeg1XdPhi140to160", 50, 0., 1.);
    histogramLeg1XdPhi160to170_ = 
      subdir.make<TH1D>("histogramLeg1XdPhi160to170" , 
			"histogramLeg1XdPhi160to170", 50, 0., 1.);
    histogramLeg1XdPhi170to175_ = 
      subdir.make<TH1D>("histogramLeg1XdPhi170to175" , 
			"histogramLeg1XdPhi170to175", 50, 0., 1.);
    histogramLeg1XdPhiGt175_ = 
      subdir.make<TH1D>("histogramLeg1XdPhiGt175" , 
			"histogramLeg1XdPhiGt175", 50, 0., 1.);
    
    histogramLeg2X_ = 
      subdir.make<TH1D>("histogramLeg2X",       
			"histogramLeg2X", 50, 0., 1.);
    histogramLeg2X_fine_ = 
      subdir.make<TH1D>("histogramLeg2X_fine",       
			"histogramLeg2X_fine", 200, 0., 1.);
    histogramLeg2VisMass_ = 
      subdir.make<TH1D>("histogramLeg2VisMass", 
			"histogramLeg2VisMass", 45, 0., 1.8);
    histogramLeg2NuMass_ = 
      subdir.make<TH1D>("histogramLeg2NuMass",  
			"histogramLeg2NuMass", 45, 0., 1.8);
    histogramLeg2NuMassVsX_ = 
      subdir.make<TH2D>("histogramLeg2NuMassVsX" , 
			"histogramLeg2NuMassVsX", 50, 0., 1., 45, 0., 1.8);

    histogramLeg2XdPhiLt30_ = 
      subdir.make<TH1D>("histogramLeg2XdPhiLt30" , 
			"histogramLeg2XdPhiLt30", 50, 0., 1.);
    histogramLeg2XdPhi30to60_ = 
      subdir.make<TH1D>("histogramLeg2XdPhi30to60" , 
			"histogramLeg2XdPhi30to60", 50, 0., 1.);
    histogramLeg2XdPhi60to90_ = 
      subdir.make<TH1D>("histogramLeg2XdPhi60to90" , 
			"histogramLeg2XdPhi60to90", 50, 0., 1.);
    histogramLeg2XdPhi90to120_ = 
      subdir.make<TH1D>("histogramLeg2XdPhi90to120" , 
			"histogramLeg2XdPhi90to120", 50, 0., 1.);
    histogramLeg2XdPhi120to140_ = 
      subdir.make<TH1D>("histogramLeg2XdPhi120to140" , 
			"histogramLeg2XdPhi120to140", 50, 0., 1.);
    histogramLeg2XdPhi140to160_ = 
      subdir.make<TH1D>("histogramLeg2XdPhi140to160" , 
			"histogramLeg2XdPhi140to160", 50, 0., 1.);
    histogramLeg2XdPhi160to170_ = 
      subdir.make<TH1D>("histogramLeg2XdPhi160to170" , 
			"histogramLeg2XdPhi160to170", 50, 0., 1.);
    histogramLeg2XdPhi170to175_ = 
      subdir.make<TH1D>("histogramLeg2XdPhi170to175" , 
			"histogramLeg2XdPhi170to175", 50, 0., 1.);
    histogramLeg2XdPhiGt175_ = 
      subdir.make<TH1D>("histogramLeg2XdPhiGt175" , 
			"histogramLeg2XdPhiGt175", 50, 0., 1.);

    histogramProdAngle_rf_ = 
      subdir.make<TH1D>("histogramProdAngle_rf" , 
			"histogramProdAngle_rf", 180, 0., TMath::Pi());
    histogramLeg12dPhi_ = 
      subdir.make<TH1D>("histogramLeg12dPhi" , 
			"histogramLeg12dPhi", 180, 0., TMath::Pi());

    histogramSVfitMassFromGenInput_ = 
      subdir.make<TH1D>("histogramSVfitMassFromGenInput",
			"histogramSVfitMassFromGenInput", 250, 0., 250.);

    histogramEventCounter_ = 
      subdir.make<TH1D>("histogramEventCounter",       
			"histogramEventCounter", 2, -0.5, +1.5);
  }
  void fillHistograms(const inputVariableSet& inputVariables, Float_t evtWeight)
  {
    if ( inputVariables.genMtautau_ > massWindowLo_ && inputVariables.genMtautau_ < massWindowHi_ ) {
      Float_t genX1 = inputVariables.genVis1En_/inputVariables.genTau1En_;
      histogramLeg1X_->Fill(genX1, evtWeight);
      histogramLeg1X_fine_->Fill(genX1, evtWeight);
      histogramLeg1VisMass_->Fill(inputVariables.genVis1Mass_, evtWeight);
      histogramLeg1NuMass_->Fill(inputVariables.genNu1Mass_, evtWeight);
      histogramLeg1NuMassVsX_->Fill(genX1, inputVariables.genNu1Mass_, evtWeight);

      Float_t genX2 = inputVariables.genVis2En_/inputVariables.genTau2En_;
      histogramLeg2X_->Fill(genX2, evtWeight);
      histogramLeg2X_fine_->Fill(genX2, evtWeight);
      histogramLeg2VisMass_->Fill(inputVariables.genVis2Mass_, evtWeight);
      histogramLeg2NuMass_->Fill(inputVariables.genNu2Mass_, evtWeight);
      histogramLeg2NuMassVsX_->Fill(genX2, inputVariables.genNu2Mass_, evtWeight);

      reco::Candidate::LorentzVector genTauPair = inputVariables.genTau1P4_ + inputVariables.genTau2P4_;
      Float_t prodAngle_rf = SVfit_namespace::decayAngleFromLabMomenta(genTauPair, inputVariables.genTau1P4_);
      histogramProdAngle_rf_->Fill(prodAngle_rf);

      Float_t dPhi12 = TMath::ACos(TMath::Cos(inputVariables.genVis1P4_.phi() - inputVariables.genVis2P4_.phi()));
      histogramLeg12dPhi_->Fill(dPhi12, evtWeight);

      Float_t dPhi12_deg = dPhi12*TMath::RadToDeg();

      if ( dPhi12_deg < 30. ) {
	histogramLeg1XdPhiLt30_->Fill(genX1, evtWeight);
	histogramLeg2XdPhiLt30_->Fill(genX2, evtWeight);
      } else if ( dPhi12_deg < 60. ) {
	histogramLeg1XdPhi30to60_->Fill(genX1, evtWeight);
	histogramLeg2XdPhi30to60_->Fill(genX2, evtWeight);
      } else if ( dPhi12_deg < 90. ) {
	histogramLeg1XdPhi60to90_->Fill(genX1, evtWeight);
	histogramLeg2XdPhi60to90_->Fill(genX2, evtWeight);
      } else if ( dPhi12_deg < 120. ) {
	histogramLeg1XdPhi90to120_->Fill(genX1, evtWeight);
	histogramLeg2XdPhi90to120_->Fill(genX2, evtWeight);
      } else if ( dPhi12_deg < 140. ) {
	histogramLeg1XdPhi120to140_->Fill(genX1, evtWeight);
	histogramLeg2XdPhi120to140_->Fill(genX2, evtWeight);
      } else if ( dPhi12_deg < 160. ) {
	histogramLeg1XdPhi140to160_->Fill(genX1, evtWeight);
	histogramLeg2XdPhi140to160_->Fill(genX2, evtWeight);
      } else if ( dPhi12_deg < 170. ) {
	histogramLeg1XdPhi160to170_->Fill(genX1, evtWeight);
	histogramLeg2XdPhi160to170_->Fill(genX2, evtWeight);
      } else if ( dPhi12_deg < 175. ) {
	histogramLeg1XdPhi170to175_->Fill(genX1, evtWeight);
	histogramLeg2XdPhi170to175_->Fill(genX2, evtWeight);
      } else {
	histogramLeg1XdPhiGt175_->Fill(genX1, evtWeight);
	histogramLeg2XdPhiGt175_->Fill(genX2, evtWeight);
      }
      
      histogramSVfitMassFromGenInput_->Fill(inputVariables.svFitMassFromGenInput_, evtWeight);

      histogramEventCounter_->Fill(1., evtWeight);
    } else {
      histogramEventCounter_->Fill(0., evtWeight);
    }
  }

  std::string directory_;

  Float_t massPoint_;
  Float_t massWindowLo_;
  Float_t massWindowHi_;

  TH1* histogramLeg1X_;
  TH1* histogramLeg1X_fine_;
  TH1* histogramLeg1VisMass_;
  TH1* histogramLeg1NuMass_;
  TH2* histogramLeg1NuMassVsX_;

  TH1* histogramLeg1XdPhiLt30_;
  TH1* histogramLeg1XdPhi30to60_;
  TH1* histogramLeg1XdPhi60to90_;
  TH1* histogramLeg1XdPhi90to120_;
  TH1* histogramLeg1XdPhi120to140_;
  TH1* histogramLeg1XdPhi140to160_;
  TH1* histogramLeg1XdPhi160to170_;
  TH1* histogramLeg1XdPhi170to175_;
  TH1* histogramLeg1XdPhiGt175_;

  TH1* histogramLeg2X_;
  TH1* histogramLeg2X_fine_;
  TH1* histogramLeg2VisMass_;
  TH1* histogramLeg2NuMass_;
  TH2* histogramLeg2NuMassVsX_;

  TH1* histogramLeg2XdPhiLt30_;
  TH1* histogramLeg2XdPhi30to60_;
  TH1* histogramLeg2XdPhi60to90_;
  TH1* histogramLeg2XdPhi90to120_;
  TH1* histogramLeg2XdPhi120to140_;
  TH1* histogramLeg2XdPhi140to160_;
  TH1* histogramLeg2XdPhi160to170_;
  TH1* histogramLeg2XdPhi170to175_;
  TH1* histogramLeg2XdPhiGt175_;

  TH1* histogramProdAngle_rf_;
  TH1* histogramLeg12dPhi_;

  TH1* histogramSVfitMassFromGenInput_;

  TH1* histogramEventCounter_;
};

void genTauToLepDecay_ps(inputVariableSet& inputVariables, reco::Candidate::LorentzVector& nuP4, TRandom& rnd)
{
//--- generate "random" leptonic tau decay 
//    using simple "Phase-Space" model implemented in SVfit
//   (assuming tau decay matrix element to be constant)

  double visMass = muonMass;
  double theta_rf = 0.;
  double nuMass = 0.;
  bool keep = false;
  while ( !keep ) {
    theta_rf = rnd.Uniform(0., TMath::Pi());
    nuMass = rnd.Uniform(0., tauLeptonMass);
    double term1 = tauLeptonMass2 - square(nuMass + visMass);
    double term2 = tauLeptonMass2 - square(nuMass - visMass);
    double prob = 0.5*nuMass*(TMath::Sqrt(TMath::Max(0., term1*term2))/(2.*tauLeptonMass))*TMath::Sin(theta_rf);
    //std::cout << "prob = " << prob << std::endl;
    keep = (prob > rnd.Rndm());
  }
  double phi_rf = rnd.Uniform(-TMath::Pi(), +TMath::Pi());
  //std::cout << "visMass1 (PS) = " << visMass << std::endl;
  //std::cout << "nuMass1 (PS) = " << nuMass << std::endl;
  double terma = tauLeptonMass2 - square(nuMass + visMass);
  double termb = tauLeptonMass2 - square(nuMass - visMass);
  double p_rf = TMath::Sqrt(TMath::Max(0., terma*termb))/(2.*tauLeptonMass);
  //std::cout << "P1 (rf,PS) = " << p_rf << std::endl;
  double visPx_rf = p_rf*TMath::Cos(phi_rf)*TMath::Sin(theta_rf);
  double visPy_rf = p_rf*TMath::Sin(phi_rf)*TMath::Sin(theta_rf);
  double visPz_rf = p_rf*TMath::Cos(theta_rf);
  double visEn_rf = TMath::Sqrt(square(p_rf) + square(visMass));
  reco::Candidate::LorentzVector visP4_rf(visPx_rf, visPy_rf, visPz_rf, visEn_rf);      
  reco::Candidate::LorentzVector visP4 = boostToLab(inputVariables.genTau1P4_, visP4_rf);      
  //std::cout << "genTau1 (lab): E = " << inputVariables.genTau1P4_.E() << "," 
  //	  << " Px = " << inputVariables.genTau1P4_.Px() << "," 
  //	  << " Py = " << inputVariables.genTau1P4_.Py() << "," 
  //	  << " Pz = " << inputVariables.genTau1P4_.Pz() << std::endl;
  //std::cout << "genVis1 (rf,PS): E = " << visP4_rf.E() << "," 
  //	  << " Px = " << visP4_rf.Px() << "," 
  //	  << " Py = " << visP4_rf.Py() << "," 
  //	  << " Pz = " << visP4_rf.Pz() << std::endl;
  //std::cout << "genVis1 (lab,PS): E = " << visP4.E() << "," 
  //	  << " Px = " << visP4.Px() << "," 
  //	  << " Py = " << visP4.Py() << "," 
  //	  << " Pz = " << visP4.Pz() << std::endl;
  //std::cout << "X1 = " << (visP4.E()/inputVariables.genTau1P4_.E()) << std::endl;
  double nuEn_rf = TMath::Sqrt(square(p_rf) + square(nuMass));
  reco::Candidate::LorentzVector nuP4_rf(-visPx_rf, -visPy_rf, -visPz_rf, nuEn_rf);
  nuP4 = boostToLab(inputVariables.genTau1P4_, nuP4_rf);
  inputVariables.genVis1En_  = visP4.energy();
  inputVariables.genVis1Px_  = visP4.px();
  inputVariables.genVis1Py_  = visP4.py();
  inputVariables.genVis1Pz_  = visP4.pz();
  inputVariables.genNu1En_   = nuP4.energy();
  inputVariables.genNu1Px_   = nuP4.px();
  inputVariables.genNu1Py_   = nuP4.py();
  inputVariables.genNu1Pz_   = nuP4.pz();
  inputVariables.genNu1Mass_ = nuMass;
}

void genTauToLepDecay_me(inputVariableSet& inputVariables, reco::Candidate::LorentzVector& nuP4, TRandom& rnd)
{
//--- generate "random" leptonic tau decay 
//    using "real" tau decay matrix element
//   (averaging over both polarization states of the tau lepton)
//
//    NOTE: The formulas taken from the paper
//           "Tau polarization and its correlations as a probe of new physics",
//           B.K. Bullock, K. Hagiwara and A.D. Martin,
//           Nucl. Phys. B395 (1993) 499.
//
  
  double visMass = muonMass;
  double visMass2 = square(visMass);
  double theta_rf = 0.;
  double nuMass = 0.;
  double p_rf = 0.;
  bool keep = false;
  while ( !keep ) {
    //theta_rf = rnd.Uniform(0., TMath::Pi());
    double cosTheta_rf = rnd.Uniform(-1., +1);
    theta_rf = TMath::ACos(cosTheta_rf);
    nuMass = rnd.Uniform(0., tauLeptonMass);
    double term1 = tauLeptonMass2 - square(nuMass + visMass);
    double term2 = tauLeptonMass2 - square(nuMass - visMass);
    p_rf = TMath::Sqrt(TMath::Max(0., term1*term2))/(2.*tauLeptonMass);
    double E_rf = TMath::Sqrt(square(p_rf) + square(visMass));
    double Emax = (tauLeptonMass2 + square(visMass))/(2.*tauLeptonMass); // formula (2.6)
    double term3 = tauLeptonMass2 - visMass2;
    double term4 = tauLeptonMass2 + visMass2;
    double term5 = tauLeptonMass*visMass;
    double term6 = square(term5);
    double norm_factor = 1./(0.75*square(visMass2)*(TMath::Log(term5) - TMath::Log(visMass2))
			   + (1./(8.*square(tauLeptonMass2)))*(0.5*cube(term3)*term4
                                                             - 0.25*term3*(cube(tauLeptonMass2) + 5.*term4*term6 + cube(visMass2))));
    //std::cout << "norm_factor = " << norm_factor << std::endl;
    double prob = norm_factor*p_rf*E_rf*(3.*Emax - 2.*E_rf - (square(visMass)/E_rf))/* *TMath::Sin(theta_rf)*/ *(nuMass/tauLeptonMass);
    //std::cout << "prob = " << prob << std::endl;
    keep = (prob > rnd.Rndm());
  }
  double phi_rf = rnd.Uniform(-TMath::Pi(), +TMath::Pi());
  //std::cout << "visMass1 (ME) = " << visMass << std::endl;
  //std::cout << "nuMass1 (ME) = " << nuMass << std::endl;
  //std::cout << "P1 (rf,ME) = " << p_rf << std::endl;
  double visPx_rf = p_rf*TMath::Cos(phi_rf)*TMath::Sin(theta_rf);
  double visPy_rf = p_rf*TMath::Sin(phi_rf)*TMath::Sin(theta_rf);
  double visPz_rf = p_rf*TMath::Cos(theta_rf);
  double visEn_rf = TMath::Sqrt(square(p_rf) + square(visMass));
  reco::Candidate::LorentzVector visP4_rf(visPx_rf, visPy_rf, visPz_rf, visEn_rf);      
  reco::Candidate::LorentzVector visP4 = boostToLab(inputVariables.genTau1P4_, visP4_rf);      
  //std::cout << "genTau1 (lab): E = " << inputVariables.genTau1P4_.E() << "," 
  //	  << " Px = " << inputVariables.genTau1P4_.Px() << "," 
  //	  << " Py = " << inputVariables.genTau1P4_.Py() << "," 
  //	  << " Pz = " << inputVariables.genTau1P4_.Pz() << std::endl;
  //std::cout << "genVis1 (rf,ME): E = " << visP4_rf.E() << "," 
  //	  << " Px = " << visP4_rf.Px() << "," 
  //	  << " Py = " << visP4_rf.Py() << "," 
  //	  << " Pz = " << visP4_rf.Pz() << std::endl;
  //std::cout << "genVis1 (lab,ME): E = " << visP4.E() << "," 
  //	  << " Px = " << visP4.Px() << "," 
  //	  << " Py = " << visP4.Py() << "," 
  //	  << " Pz = " << visP4.Pz() << std::endl;
  //std::cout << "X1 = " << (visP4.E()/inputVariables.genTau1P4_.E()) << std::endl;
  double nuEn_rf = TMath::Sqrt(square(p_rf) + square(nuMass));
  reco::Candidate::LorentzVector nuP4_rf(-visP4_rf.px(), -visP4_rf.py(), -visP4_rf.pz(), nuEn_rf);
  nuP4 = boostToLab(inputVariables.genTau1P4_, nuP4_rf);
  inputVariables.genVis1En_  = visP4.energy();
  inputVariables.genVis1Px_  = visP4.px();
  inputVariables.genVis1Py_  = visP4.py();
  inputVariables.genVis1Pz_  = visP4.pz();
  inputVariables.genNu1En_   = nuP4.energy();
  inputVariables.genNu1Px_   = nuP4.px();
  inputVariables.genNu1Py_   = nuP4.py();
  inputVariables.genNu1Pz_   = nuP4.pz();
  inputVariables.genNu1Mass_ = nuMass;
}

void genTauToHadDecay(inputVariableSet& inputVariables, reco::Candidate::LorentzVector& nuP4, TRandom& rnd)
{
//--- generate "random" hadronic tau decay 
//    using simple "Phase-Space" model implemented in SVfit
//   (assuming tau decay matrix element to be constant)

  double visMass = inputVariables.genVis2Mass_;
  double theta_rf = 0.;
  bool keep = false;
  while ( !keep ) {
    theta_rf = rnd.Uniform(0., TMath::Pi());
    double prob = TMath::Sin(theta_rf);
    //std::cout << "prob = " << prob << std::endl;
    keep = (prob > rnd.Rndm());
  }
  double phi_rf = rnd.Uniform(-TMath::Pi(), +TMath::Pi());
  double term = tauLeptonMass2 - square(visMass);
  double p_rf = term/(2.*tauLeptonMass);
  //std::cout << "P2 (rf) = " << p_rf << std::endl;
  double visPx_rf = p_rf*TMath::Cos(phi_rf)*TMath::Sin(theta_rf);
  double visPy_rf = p_rf*TMath::Sin(phi_rf)*TMath::Sin(theta_rf);
  double visPz_rf = p_rf*TMath::Cos(theta_rf);
  double visEn_rf = TMath::Sqrt(square(p_rf) + square(visMass));
  reco::Candidate::LorentzVector visP4_rf(visPx_rf, visPy_rf, visPz_rf, visEn_rf);
  reco::Candidate::LorentzVector visP4 = boostToLab(inputVariables.genTau2P4_, visP4_rf);
  //std::cout << "genTau2 (lab): E = " << inputVariables.genTau2P4_.E() << "," 
  //	  << " Px = " << inputVariables.genTau2P4_.Px() << "," 
  //	  << " Py = " << inputVariables.genTau2P4_.Py() << "," 
  //	  << " Pz = " << inputVariables.genTau2P4_.Pz() << std::endl;
  //std::cout << "genVis2 (rf): E = " << visP4_rf.E() << "," 
  //	  << " Px = " << visP4_rf.Px() << "," 
  //	  << " Py = " << visP4_rf.Py() << "," 
  //	  << " Pz = " << visP4_rf.Pz() << std::endl;
  //std::cout << "genVis2 (lab): E = " << visP4.E() << "," 
  //	  << " Px = " << visP4.Px() << "," 
  //	  << " Py = " << visP4.Py() << "," 
  //	  << " Pz = " << visP4.Pz() << std::endl;
  //std::cout << "X2 = " << (visP4.E()/inputVariables.genTau2P4_.E()) << std::endl;
  double nuEn_rf = p_rf;
  reco::Candidate::LorentzVector nuP4_rf(-visPx_rf, -visPy_rf, -visPz_rf, nuEn_rf);
  nuP4 = boostToLab(inputVariables.genTau1P4_, nuP4_rf);
  inputVariables.genVis2En_  = visP4.energy();
  inputVariables.genVis2Px_  = visP4.px();
  inputVariables.genVis2Py_  = visP4.py();
  inputVariables.genVis2Pz_  = visP4.pz();
  inputVariables.genNu2En_   = nuP4.energy();
  inputVariables.genNu2Px_   = nuP4.px();
  inputVariables.genNu2Py_   = nuP4.py();
  inputVariables.genNu2Pz_   = nuP4.pz();
  inputVariables.genNu2Mass_ = 0.; 
}

void fillHistograms(inputVariableSet& inputVariables, 
		    const reco::Candidate::LorentzVector& nuP4Leg1, const reco::Candidate::LorentzVector& nuP4Leg2, 		    
		    std::vector<plotEntryType*>& plotsBeforeVisPtCuts,
		    std::vector<plotEntryType*>& plotsAfterVisEtaCutsLeg1, 
		    std::vector<plotEntryType*>& plotsAfterVisEtaAndPtCutsLeg1,
		    std::vector<plotEntryType*>& plotsAfterVisEtaCutsLeg2, 
		    std::vector<plotEntryType*>& plotsAfterVisEtaAndPtCutsLeg2,
		    std::vector<plotEntryType*>& plotsAfterVisEtaCutsLeg1and2, 
		    std::vector<plotEntryType*>& plotsAfterVisEtaAndPtCutsLeg1and2,
		    double evtWeight)
{
  inputVariables.genMEtPx_ += (nuP4Leg1.px() + nuP4Leg2.px()) - (inputVariables.genNu1Px_ + inputVariables.genNu2Px_);
  inputVariables.genMEtPy_ += (nuP4Leg1.py() + nuP4Leg2.py()) - (inputVariables.genNu1Py_ + inputVariables.genNu2Py_);
 
  inputVariables.initialize();

  for ( std::vector<plotEntryType*>::iterator plot = plotsBeforeVisPtCuts.begin();
	plot != plotsBeforeVisPtCuts.end(); ++plot ) {
    (*plot)->fillHistograms(inputVariables, evtWeight);
  }
  
  bool passedVisEtaCutsLeg1          = inputVariables.passedVisEtaCutsLeg1();
  bool passedVisEtaAndPtCutsLeg1     = inputVariables.passedVisEtaAndPtCutsLeg1();
  bool passedVisEtaCutsLeg2          = inputVariables.passedVisEtaCutsLeg2();
  bool passedVisEtaAndPtCutsLeg2     = inputVariables.passedVisEtaAndPtCutsLeg2();
  bool passedVisEtaCutsLeg1and2      = inputVariables.passedVisEtaCutsLeg1and2();
  bool passedVisEtaAndPtCutsLeg1and2 = inputVariables.passedVisEtaAndPtCutsLeg1and2();
  
  if ( passedVisEtaCutsLeg1 ) {
    for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaCutsLeg1.begin();
	  plot != plotsAfterVisEtaCutsLeg1.end(); ++plot ) {
      (*plot)->fillHistograms(inputVariables, evtWeight);
    }
  }
  if ( passedVisEtaAndPtCutsLeg1 ) {
    for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaAndPtCutsLeg1.begin();
	  plot != plotsAfterVisEtaAndPtCutsLeg1.end(); ++plot ) {
      (*plot)->fillHistograms(inputVariables, evtWeight);
    }
  }
  if ( passedVisEtaCutsLeg2 ) {
    for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaCutsLeg2.begin();
	  plot != plotsAfterVisEtaCutsLeg2.end(); ++plot ) {
      (*plot)->fillHistograms(inputVariables, evtWeight);
    }
  }
  if ( passedVisEtaAndPtCutsLeg2 ) {
    for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaAndPtCutsLeg2.begin();
	  plot != plotsAfterVisEtaAndPtCutsLeg2.end(); ++plot ) {
      (*plot)->fillHistograms(inputVariables, evtWeight);
    }
  }
  if ( passedVisEtaCutsLeg1and2 ) {
    for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaCutsLeg1and2.begin();
	  plot != plotsAfterVisEtaCutsLeg1and2.end(); ++plot ) {
      (*plot)->fillHistograms(inputVariables, evtWeight);
    }
  }
  if ( passedVisEtaAndPtCutsLeg1and2 ) {
    for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaAndPtCutsLeg1and2.begin();
	  plot != plotsAfterVisEtaAndPtCutsLeg1and2.end(); ++plot ) {
      (*plot)->fillHistograms(inputVariables, evtWeight);
    }
  }
}

int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<studySVfitVisPtCuts>:" << std::endl;

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("studySVfitVisPtCuts");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("studySVfitVisPtCuts") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgStudySVfitVisPtCuts = cfg.getParameter<edm::ParameterSet>("studySVfitVisPtCuts");

  std::string treeName = cfgStudySVfitVisPtCuts.getParameter<std::string>("treeName");

  typedef std::vector<double> vdouble;
  vdouble massPoints = cfgStudySVfitVisPtCuts.getParameter<vdouble>("massPoints");

  bool runSVfit = cfgStudySVfitVisPtCuts.getParameter<bool>("runSVfit");
  std::cout << " runSVfit = " << runSVfit << std::endl;

  fwlite::InputSource inputFiles(cfg); 
  int maxEvents = inputFiles.maxEvents();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  std::vector<plotEntryType*> plotsBeforeVisEtaAndPtCuts;
  TFileDirectory dirBeforeVisEtaAndPtCuts = fs.mkdir("plotsBeforeVisEtaAndPtCuts");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg1;
  TFileDirectory dirAfterVisEtaCutsLeg1 = fs.mkdir("plotsAfterVisEtaCutsLeg1");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg1;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg1 = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg1");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg2;
  TFileDirectory dirAfterVisEtaCutsLeg2 = fs.mkdir("plotsAfterVisEtaCutsLeg2");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg2;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg2 = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg2");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg1and2;
  TFileDirectory dirAfterVisEtaCutsLeg1and2 = fs.mkdir("plotsAfterVisEtaCutsLeg1and2");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg1and2;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg1and2 = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg1and2");

  std::vector<plotEntryType*> plotsBeforeVisEtaAndPtCuts_toyMCps;
  TFileDirectory dirBeforeVisEtaAndPtCuts_toyMCps = fs.mkdir("plotsBeforeVisEtaAndPtCuts_toyMCps");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg1_toyMCps;
  TFileDirectory dirAfterVisEtaCutsLeg1_toyMCps = fs.mkdir("plotsAfterVisEtaCutsLeg1_toyMCps");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg1_toyMCps;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg1_toyMCps = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg1_toyMCps");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg2_toyMCps;
  TFileDirectory dirAfterVisEtaCutsLeg2_toyMCps = fs.mkdir("plotsAfterVisEtaCutsLeg2_toyMCps");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg2_toyMCps;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg2_toyMCps = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg2_toyMCps");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg1and2_toyMCps;
  TFileDirectory dirAfterVisEtaCutsLeg1and2_toyMCps = fs.mkdir("plotsAfterVisEtaCutsLeg1and2_toyMCps");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg1and2_toyMCps;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg1and2_toyMCps = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg1and2_toyMCps");

  std::vector<plotEntryType*> plotsBeforeVisEtaAndPtCuts_toyMCme;
  TFileDirectory dirBeforeVisEtaAndPtCuts_toyMCme = fs.mkdir("plotsBeforeVisEtaAndPtCuts_toyMCme");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg1_toyMCme;
  TFileDirectory dirAfterVisEtaCutsLeg1_toyMCme = fs.mkdir("plotsAfterVisEtaCutsLeg1_toyMCme");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg1_toyMCme;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg1_toyMCme = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg1_toyMCme");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg2_toyMCme;
  TFileDirectory dirAfterVisEtaCutsLeg2_toyMCme = fs.mkdir("plotsAfterVisEtaCutsLeg2_toyMCme");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg2_toyMCme;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg2_toyMCme = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg2_toyMCme");
  std::vector<plotEntryType*> plotsAfterVisEtaCutsLeg1and2_toyMCme;
  TFileDirectory dirAfterVisEtaCutsLeg1and2_toyMCme = fs.mkdir("plotsAfterVisEtaCutsLeg1and2_toyMCme");
  std::vector<plotEntryType*> plotsAfterVisEtaAndPtCutsLeg1and2_toyMCme;
  TFileDirectory dirAfterVisEtaAndPtCutsLeg1and2_toyMCme = fs.mkdir("plotsAfterVisEtaAndPtCutsLeg1and2_toyMCme");

  size_t numMassPoints = massPoints.size();
  for ( size_t iMassPoint = 0; iMassPoint < numMassPoints; ++iMassPoint ) {
    Float_t massPoint = massPoints[iMassPoint];
    Float_t minMass = ( iMassPoint == 0 ) ? 
      massPoint - 0.5*(massPoints[iMassPoint + 1] - massPoint) : 0.5*(massPoints[iMassPoint - 1] + massPoint);
    Float_t maxMass = ( iMassPoint < (numMassPoints - 1) ) ? 
      0.5*(massPoint + massPoints[iMassPoint + 1]) : massPoint + 0.5*(massPoint - massPoints[iMassPoint - 1]);

    plotEntryType* plotBeforeVisEtaAndPtCuts = new plotEntryType(massPoint, minMass, maxMass);
    plotBeforeVisEtaAndPtCuts->bookHistograms(dirBeforeVisEtaAndPtCuts);
    plotsBeforeVisEtaAndPtCuts.push_back(plotBeforeVisEtaAndPtCuts);
    plotEntryType* plotAfterVisEtaCutsLeg1 = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg1->bookHistograms(dirAfterVisEtaCutsLeg1);
    plotsAfterVisEtaCutsLeg1.push_back(plotAfterVisEtaCutsLeg1);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg1 = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg1->bookHistograms(dirAfterVisEtaAndPtCutsLeg1);
    plotsAfterVisEtaAndPtCutsLeg1.push_back(plotAfterVisEtaAndPtCutsLeg1);
    plotEntryType* plotAfterVisEtaCutsLeg2 = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg2->bookHistograms(dirAfterVisEtaCutsLeg2);
    plotsAfterVisEtaCutsLeg2.push_back(plotAfterVisEtaCutsLeg2);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg2 = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg2->bookHistograms(dirAfterVisEtaAndPtCutsLeg2);
    plotsAfterVisEtaAndPtCutsLeg2.push_back(plotAfterVisEtaAndPtCutsLeg2);
    plotEntryType* plotAfterVisEtaCutsLeg1and2 = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg1and2->bookHistograms(dirAfterVisEtaCutsLeg1and2);
    plotsAfterVisEtaCutsLeg1and2.push_back(plotAfterVisEtaCutsLeg1and2);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg1and2 = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg1and2->bookHistograms(dirAfterVisEtaAndPtCutsLeg1and2);
    plotsAfterVisEtaAndPtCutsLeg1and2.push_back(plotAfterVisEtaAndPtCutsLeg1and2);
    
    plotEntryType* plotBeforeVisEtaAndPtCuts_toyMCps = new plotEntryType(massPoint, minMass, maxMass);
    plotBeforeVisEtaAndPtCuts_toyMCps->bookHistograms(dirBeforeVisEtaAndPtCuts_toyMCps);
    plotsBeforeVisEtaAndPtCuts_toyMCps.push_back(plotBeforeVisEtaAndPtCuts_toyMCps);
    plotEntryType* plotAfterVisEtaCutsLeg1_toyMCps = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg1_toyMCps->bookHistograms(dirAfterVisEtaCutsLeg1_toyMCps);
    plotsAfterVisEtaCutsLeg1_toyMCps.push_back(plotAfterVisEtaCutsLeg1_toyMCps);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg1_toyMCps = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg1_toyMCps->bookHistograms(dirAfterVisEtaAndPtCutsLeg1_toyMCps);
    plotsAfterVisEtaAndPtCutsLeg1_toyMCps.push_back(plotAfterVisEtaAndPtCutsLeg1_toyMCps);
    plotEntryType* plotAfterVisEtaCutsLeg2_toyMCps = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg2_toyMCps->bookHistograms(dirAfterVisEtaCutsLeg2_toyMCps);
    plotsAfterVisEtaCutsLeg2_toyMCps.push_back(plotAfterVisEtaCutsLeg2_toyMCps);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg2_toyMCps = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg2_toyMCps->bookHistograms(dirAfterVisEtaAndPtCutsLeg2_toyMCps);
    plotsAfterVisEtaAndPtCutsLeg2_toyMCps.push_back(plotAfterVisEtaAndPtCutsLeg2_toyMCps);
    plotEntryType* plotAfterVisEtaCutsLeg1and2_toyMCps = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg1and2_toyMCps->bookHistograms(dirAfterVisEtaCutsLeg1and2_toyMCps);
    plotsAfterVisEtaCutsLeg1and2_toyMCps.push_back(plotAfterVisEtaCutsLeg1and2_toyMCps);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg1and2_toyMCps = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg1and2_toyMCps->bookHistograms(dirAfterVisEtaAndPtCutsLeg1and2_toyMCps);
    plotsAfterVisEtaAndPtCutsLeg1and2_toyMCps.push_back(plotAfterVisEtaAndPtCutsLeg1and2_toyMCps);

    plotEntryType* plotBeforeVisEtaAndPtCuts_toyMCme = new plotEntryType(massPoint, minMass, maxMass);
    plotBeforeVisEtaAndPtCuts_toyMCme->bookHistograms(dirBeforeVisEtaAndPtCuts_toyMCme);
    plotsBeforeVisEtaAndPtCuts_toyMCme.push_back(plotBeforeVisEtaAndPtCuts_toyMCme);
    plotEntryType* plotAfterVisEtaCutsLeg1_toyMCme = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg1_toyMCme->bookHistograms(dirAfterVisEtaCutsLeg1_toyMCme);
    plotsAfterVisEtaCutsLeg1_toyMCme.push_back(plotAfterVisEtaCutsLeg1_toyMCme);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg1_toyMCme = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg1_toyMCme->bookHistograms(dirAfterVisEtaAndPtCutsLeg1_toyMCme);
    plotsAfterVisEtaAndPtCutsLeg1_toyMCme.push_back(plotAfterVisEtaAndPtCutsLeg1_toyMCme);
    plotEntryType* plotAfterVisEtaCutsLeg2_toyMCme = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg2_toyMCme->bookHistograms(dirAfterVisEtaCutsLeg2_toyMCme);
    plotsAfterVisEtaCutsLeg2_toyMCme.push_back(plotAfterVisEtaCutsLeg2_toyMCme);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg2_toyMCme = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg2_toyMCme->bookHistograms(dirAfterVisEtaAndPtCutsLeg2_toyMCme);
    plotsAfterVisEtaAndPtCutsLeg2_toyMCme.push_back(plotAfterVisEtaAndPtCutsLeg2_toyMCme);
    plotEntryType* plotAfterVisEtaCutsLeg1and2_toyMCme = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaCutsLeg1and2_toyMCme->bookHistograms(dirAfterVisEtaCutsLeg1and2_toyMCme);
    plotsAfterVisEtaCutsLeg1and2_toyMCme.push_back(plotAfterVisEtaCutsLeg1and2_toyMCme);
    plotEntryType* plotAfterVisEtaAndPtCutsLeg1and2_toyMCme = new plotEntryType(massPoint, minMass, maxMass);
    plotAfterVisEtaAndPtCutsLeg1and2_toyMCme->bookHistograms(dirAfterVisEtaAndPtCutsLeg1and2_toyMCme);
    plotsAfterVisEtaAndPtCutsLeg1and2_toyMCme.push_back(plotAfterVisEtaAndPtCutsLeg1and2_toyMCme);
  }

  TRandom3 rnd;

  int numEvents_processed = 0;
  
  typedef std::vector<std::string> vstring;
  for ( vstring::const_iterator inputFileName = inputFiles.files().begin();
  	inputFileName != inputFiles.files().end(); ++inputFileName ) {
    TFile* inputFile = new TFile(inputFileName->data());
    TTree* tree = dynamic_cast<TTree*>(inputFile->Get(treeName.c_str()));

    inputVariableSet inputVariables(tree, runSVfit);
    inputVariableSet inputVariables_toyMCps(NULL, runSVfit);
    inputVariableSet inputVariables_toyMCme(NULL, runSVfit);

    int numEntries = tree->GetEntries();
    for ( int iEntry = 0; iEntry < numEntries && (numEvents_processed < maxEvents || maxEvents == -1); ++iEntry ) {
      //std::cout << "Event #" << numEvents_processed << ":" << std::endl;
      if ( numEvents_processed > 0 && (numEvents_processed % 1000) == 0 ) {
	std::cout << "processing Event " << numEvents_processed << std::endl;
      }

      tree->GetEntry(iEntry);

      inputVariables.initialize();

      const Float_t evtWeight = 1.0;

      for ( std::vector<plotEntryType*>::iterator plot = plotsBeforeVisEtaAndPtCuts.begin();
	    plot != plotsBeforeVisEtaAndPtCuts.end(); ++plot ) {
	(*plot)->fillHistograms(inputVariables, evtWeight);
      }
      
      bool passedVisEtaCutsLeg1          = inputVariables.passedVisEtaCutsLeg1();
      bool passedVisEtaAndPtCutsLeg1     = inputVariables.passedVisEtaAndPtCutsLeg1();
      bool passedVisEtaCutsLeg2          = inputVariables.passedVisEtaCutsLeg2();
      bool passedVisEtaAndPtCutsLeg2     = inputVariables.passedVisEtaAndPtCutsLeg2();
      bool passedVisEtaCutsLeg1and2      = inputVariables.passedVisEtaCutsLeg1and2();
      bool passedVisEtaAndPtCutsLeg1and2 = inputVariables.passedVisEtaAndPtCutsLeg1and2();
      
      if ( passedVisEtaCutsLeg1 ) {
	for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaCutsLeg1.begin();
	      plot != plotsAfterVisEtaCutsLeg1.end(); ++plot ) {
	  (*plot)->fillHistograms(inputVariables, evtWeight);
	}
      }
      if ( passedVisEtaAndPtCutsLeg1 ) {
	for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaAndPtCutsLeg1.begin();
	      plot != plotsAfterVisEtaAndPtCutsLeg1.end(); ++plot ) {
	  (*plot)->fillHistograms(inputVariables, evtWeight);
	}
      }
      if ( passedVisEtaCutsLeg2 ) {
	for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaCutsLeg2.begin();
	      plot != plotsAfterVisEtaCutsLeg2.end(); ++plot ) {
	  (*plot)->fillHistograms(inputVariables, evtWeight);
	}
      }
      if ( passedVisEtaAndPtCutsLeg2 ) {
	for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaAndPtCutsLeg2.begin();
	      plot != plotsAfterVisEtaAndPtCutsLeg2.end(); ++plot ) {
	  (*plot)->fillHistograms(inputVariables, evtWeight);
	}
      }
      if ( passedVisEtaCutsLeg1and2 ) {
	for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaCutsLeg1and2.begin();
	      plot != plotsAfterVisEtaCutsLeg1and2.end(); ++plot ) {
	  (*plot)->fillHistograms(inputVariables, evtWeight);
	}
      }
      if ( passedVisEtaAndPtCutsLeg1and2 ) {
	for ( std::vector<plotEntryType*>::iterator plot = plotsAfterVisEtaAndPtCutsLeg1and2.begin();
	      plot != plotsAfterVisEtaAndPtCutsLeg1and2.end(); ++plot ) {
	  (*plot)->fillHistograms(inputVariables, evtWeight);
	}
      }

      //-------------------------------------------------------------------------
      // toy Monte Carlo to check difference between simple Phase-Space model
      // used by SVfit likelihood functions and "real" tau decays implemented in TAUOLA

      inputVariables_toyMCps = inputVariables;

      reco::Candidate::LorentzVector nuP4Leg1_toyMCps;
      reco::Candidate::LorentzVector nuP4Leg2_toyMCps;
      genTauToLepDecay_ps(inputVariables_toyMCps, nuP4Leg1_toyMCps, rnd);
      genTauToHadDecay(inputVariables_toyMCps, nuP4Leg2_toyMCps, rnd);
      
      fillHistograms(inputVariables_toyMCps, nuP4Leg1_toyMCps, nuP4Leg2_toyMCps,
		     plotsBeforeVisEtaAndPtCuts_toyMCps,
		     plotsAfterVisEtaCutsLeg1_toyMCps, 
		     plotsAfterVisEtaAndPtCutsLeg1_toyMCps,
		     plotsAfterVisEtaCutsLeg2_toyMCps, 
		     plotsAfterVisEtaAndPtCutsLeg2_toyMCps,
		     plotsAfterVisEtaCutsLeg1and2_toyMCps, 
		     plotsAfterVisEtaAndPtCutsLeg1and2_toyMCps, 
		     evtWeight);
      //-------------------------------------------------------------------------

      //-------------------------------------------------------------------------
      // toy Monte Carlo to check difference between tau decay matrix elements
      // implemented in SVfit likelihood functions and tau decays implemented in TAUOLA

      inputVariables_toyMCme = inputVariables;

      reco::Candidate::LorentzVector nuP4Leg1_toyMCme;
      reco::Candidate::LorentzVector nuP4Leg2_toyMCme;
      genTauToLepDecay_me(inputVariables_toyMCme, nuP4Leg1_toyMCme, rnd);
      genTauToHadDecay(inputVariables_toyMCme, nuP4Leg2_toyMCme, rnd);
      
      fillHistograms(inputVariables_toyMCme, nuP4Leg1_toyMCme, nuP4Leg2_toyMCme,
		     plotsBeforeVisEtaAndPtCuts_toyMCme,
		     plotsAfterVisEtaCutsLeg1_toyMCme, 
		     plotsAfterVisEtaAndPtCutsLeg1_toyMCme,
		     plotsAfterVisEtaCutsLeg2_toyMCme, 
		     plotsAfterVisEtaAndPtCutsLeg2_toyMCme,
		     plotsAfterVisEtaCutsLeg1and2_toyMCme, 
		     plotsAfterVisEtaAndPtCutsLeg1and2_toyMCme, 
		     evtWeight);
      //-------------------------------------------------------------------------

      ++numEvents_processed;
    }
    
    delete inputFile;
  }
  
  clock.Show("studySVfitVisPtCuts");

  return 0;
}
