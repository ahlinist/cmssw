#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>

// global flag to include ppmumux histograms
const bool usePpMuMux = false;


// function to scale histograms to the normalized luminosity
void ScaleHistograms(TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, TH1F* BdJpsiK0, 
		     TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F* ppmumux);           


// function to add the various backgrounds into one histogram
TH1F* addBackgrounds(TString name, TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, 
		     TH1F* BdJpsiK0, TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F *ppmumux);      

// function to draw histograms in a standard way (normalized to one)
void drawStandardNormalized(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux);

// draw histograms scaled to correc lumi
void drawStandardLumiScaled(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux);


// ***************main function*****************************
void MakePlots() {


//    TFile * BtoJPsiMuMuHistoFile = new TFile ( "BtoJPsiMuMuHistoFile_8Dec09_Bs_phiCut0_007.root" , "READ");
//    TFile * JPsiMuMuHistoFile  = new TFile ( "JPsiMuMuHistoFile_8Dec09_Bs_phiCut0_007.root", "READ");
//    TFile * BtoJPsiMuMuHistoFile = new TFile ( "BtoJPsiMuMuHistoFile7TeV_Bs.root" , "READ");
//    TFile * JPsiMuMuHistoFile  = new TFile ( "JPsiMuMuHistoFile7TeV_Bs.root", "READ");
//  TFile * BtoJPsiMuMuHistoFile = new TFile ( "BtoJPsiMuMuHistoFile_8Dec09_Bdclosest.root" , "READ");
//  TFile * JPsiMuMuHistoFile  = new TFile ( "JPsiMuMuHistoFile_8Dec09_Bdclosest.root", "READ");

   TFile * BtoJPsiMuMuHistoFile = new TFile ( "BtoJPsiMuMuHistoFile.root" , "READ");
   TFile * JPsiMuMuHistoFile  = new TFile ( "JPsiMuMuHistoFile.root", "READ");

//   TFile * ppmuXHistoFile   = new TFile ("ppmuXHistoFile.root", "READ");
//  TFile * ppmumuXHistoFile = new TFile ("ppmumuXHistoFile.root", "READ");
//  TFile * ppmumuXHistoFile = new TFile ("ppMuMuHistoFile_8Dec09_Bs.root", "READ");
  TFile * ppmumuXHistoFile = new TFile ("JPsiMuMuHistoFile.root", "READ");


TH1F * hJPsiMass_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsJPsiPhiSignal"                );
TH1F * hJPsiMass_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsJPsiKKSignal"                 );
TH1F * hJPsiMass_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdJPsiKstarSignal"              );
TH1F * hJPsiMass_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsOther"                        );
TH1F * hJPsiMass_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdOther"                        );
TH1F * hJPsiMass_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_Other"                          );
TH1F * hJPsiMass_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsJpsiEta"                      );
TH1F * hJPsiMass_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdJpsiK10"                      );
TH1F * hJPsiMass_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdJpsiK0"                       );
TH1F * hJPsiMass_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BpJpsiKp"                       );
TH1F * hJPsiMass_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hJPsiMass_Other"                          );
TH1F * hJPsiMass_ppmumux                       =(TH1F*) ppmumuXHistoFile->Get("hJPsiMass_Other"                          );


TH1F * hPhiMass_BsJPsiPhiSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsJPsiPhiSignal"                 );
TH1F * hPhiMass_BsJPsiKKSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsJPsiKKSignal"                  );
TH1F * hPhiMass_BdJPsiKstarSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdJPsiKstarSignal"               );
TH1F * hPhiMass_BsOther                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsOther"                         );
TH1F * hPhiMass_BdOther                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdOther"                         );
TH1F * hPhiMass_BOther                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_Other"                           );
TH1F * hPhiMass_BsJpsiEta                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsJpsiEta"                       );
TH1F * hPhiMass_BdJpsiK10                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdJpsiK10"                       );
TH1F * hPhiMass_BdJpsiK0                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdJpsiK0"                        );
TH1F * hPhiMass_BpJpsiKp                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BpJpsiKp"                        );
TH1F * hPhiMass_JpsiOther                          =(TH1F*) JPsiMuMuHistoFile->Get("hPhiMass_Other"                           );
TH1F * hPhiMass_ppmumux                          =(TH1F*) ppmumuXHistoFile->Get("hPhiMass_Other"                           );


TH1F * hPhiMassFinal_BsJPsiPhiSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsJPsiPhiSignal"            );
TH1F * hPhiMassFinal_BsJPsiKKSignal            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsJPsiKKSignal"             );
TH1F * hPhiMassFinal_BdJPsiKstarSignal         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdJPsiKstarSignal"          );
TH1F * hPhiMassFinal_BsOther                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsOther"                    );
TH1F * hPhiMassFinal_BdOther                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdOther"                    );
TH1F * hPhiMassFinal_BOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_Other"                      );
TH1F * hPhiMassFinal_BsJpsiEta                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsJpsiEta"                  );
TH1F * hPhiMassFinal_BdJpsiK10                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdJpsiK10"                  );
TH1F * hPhiMassFinal_BdJpsiK0                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdJpsiK0"                   );
TH1F * hPhiMassFinal_BpJpsiKp                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BpJpsiKp"                   );
TH1F * hPhiMassFinal_JpsiOther                     =(TH1F*) JPsiMuMuHistoFile->Get("hPhiMassFinal_Other"                      );
TH1F * hPhiMassFinal_ppmumux                     =(TH1F*) ppmumuXHistoFile->Get("hPhiMassFinal_Other"                      );


TH1F * hBsMassFinal_BsJPsiPhiSignal            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsJPsiPhiSignal"             );
TH1F * hBsMassFinal_BsJPsiKKSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsJPsiKKSignal"              );
TH1F * hBsMassFinal_BdJPsiKstarSignal          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdJPsiKstarSignal"           );
TH1F * hBsMassFinal_BsOther                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsOther"                     );
TH1F * hBsMassFinal_BdOther                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdOther"                     );
TH1F * hBsMassFinal_BOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_Other"                       );
TH1F * hBsMassFinal_BsJpsiEta                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsJpsiEta"                   );
TH1F * hBsMassFinal_BdJpsiK10                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdJpsiK10"                   );
TH1F * hBsMassFinal_BdJpsiK0                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdJpsiK0"                    );
TH1F * hBsMassFinal_BpJpsiKp                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BpJpsiKp"                    );
TH1F * hBsMassFinal_JpsiOther                      =(TH1F*) JPsiMuMuHistoFile->Get("hBsMassFinal_Other"                       );
TH1F * hBsMassFinal_ppmumux                      =(TH1F*) ppmumuXHistoFile->Get("hBsMassFinal_Other"                       );

TH1F * hBsMassFinalAfterFit_BsJPsiPhiSignal    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJPsiPhiSignal"     );
TH1F * hBsMassFinalAfterFit_BsJPsiKKSignal     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJPsiKKSignal"      );
TH1F * hBsMassFinalAfterFit_BdJPsiKstarSignal  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdJPsiKstarSignal"   );
TH1F * hBsMassFinalAfterFit_BsOther            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsOther"             );
TH1F * hBsMassFinalAfterFit_BdOther            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdOther"             );
TH1F * hBsMassFinalAfterFit_BOther              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_Other"               );
TH1F * hBsMassFinalAfterFit_BsJpsiEta          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJpsiEta"           );
TH1F * hBsMassFinalAfterFit_BdJpsiK10          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdJpsiK10"           );
TH1F * hBsMassFinalAfterFit_BdJpsiK0           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdJpsiK0"            );
TH1F * hBsMassFinalAfterFit_BpJpsiKp           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BpJpsiKp"            );
TH1F * hBsMassFinalAfterFit_JpsiOther              =(TH1F*) JPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_Other"               );
TH1F * hBsMassFinalAfterFit_ppmumux              =(TH1F*) ppmumuXHistoFile->Get("hBsMassFinalAfterFit_Other"               );

TH1F * hK1Pt_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsJPsiPhiSignal"                    );
TH1F * hK1Pt_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsJPsiKKSignal"                     );
TH1F * hK1Pt_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdJPsiKstarSignal"                  );
TH1F * hK1Pt_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsOther"                            );
TH1F * hK1Pt_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdOther"                            );
TH1F * hK1Pt_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_Other"                              );
TH1F * hK1Pt_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsJpsiEta"                          );
TH1F * hK1Pt_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdJpsiK10"                          );
TH1F * hK1Pt_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdJpsiK0"                           );
TH1F * hK1Pt_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BpJpsiKp"                           );
TH1F * hK1Pt_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hK1Pt_Other"                              );
TH1F * hK1Pt_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hK1Pt_Other"                              );

TH1F * hK2Pt_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsJPsiPhiSignal"                    );
TH1F * hK2Pt_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsJPsiKKSignal"                     );
TH1F * hK2Pt_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdJPsiKstarSignal"                  );
TH1F * hK2Pt_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsOther"                            );
TH1F * hK2Pt_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdOther"                            );
TH1F * hK2Pt_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_Other"                              );
TH1F * hK2Pt_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsJpsiEta"                          );
TH1F * hK2Pt_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdJpsiK10"                          );
TH1F * hK2Pt_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdJpsiK0"                           );
TH1F * hK2Pt_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BpJpsiKp"                           );
TH1F * hK2Pt_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hK2Pt_Other"                              );
TH1F * hK2Pt_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hK2Pt_Other"                              );

TH1F * hDist3D_BsJPsiPhiSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsJPsiPhiSignal"                  );
TH1F * hDist3D_BsJPsiKKSignal                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsJPsiKKSignal"                   );
TH1F * hDist3D_BdJPsiKstarSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdJPsiKstarSignal"                );
TH1F * hDist3D_BsOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsOther"                          );
TH1F * hDist3D_BdOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdOther"                          );
TH1F * hDist3D_BOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_Other"                            );
TH1F * hDist3D_BsJpsiEta                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsJpsiEta"                        );
TH1F * hDist3D_BdJpsiK10                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdJpsiK10"                        );
TH1F * hDist3D_BdJpsiK0                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdJpsiK0"                         );
TH1F * hDist3D_BpJpsiKp                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BpJpsiKp"                         );
TH1F * hDist3D_JpsiOther                           =(TH1F*) JPsiMuMuHistoFile->Get("hDist3D_Other"                            );
TH1F * hDist3D_ppmumux                           =(TH1F*) ppmumuXHistoFile->Get("hDist3D_Other"                            );

TH1F * hDistSign3D_BsJPsiPhiSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsJPsiPhiSignal"              );
TH1F * hDistSign3D_BsJPsiKKSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsJPsiKKSignal"               );
TH1F * hDistSign3D_BdJPsiKstarSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdJPsiKstarSignal"            );
TH1F * hDistSign3D_BsOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsOther"                      );
TH1F * hDistSign3D_BdOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdOther"                      );
TH1F * hDistSign3D_BOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_Other"                        );
TH1F * hDistSign3D_BsJpsiEta                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsJpsiEta"                    );
TH1F * hDistSign3D_BdJpsiK10                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdJpsiK10"                    );
TH1F * hDistSign3D_BdJpsiK0                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdJpsiK0"                     );
TH1F * hDistSign3D_BpJpsiKp                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BpJpsiKp"                     );
TH1F * hDistSign3D_JpsiOther                       =(TH1F*) JPsiMuMuHistoFile->Get("hDistSign3D_Other"                        );
TH1F * hDistSign3D_ppmumux                       =(TH1F*) ppmumuXHistoFile->Get("hDistSign3D_Other"                        );

TH1F * hDistSign1D_BsJPsiPhiSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsJPsiPhiSignal"              );
TH1F * hDistSign1D_BsJPsiKKSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsJPsiKKSignal"               );
TH1F * hDistSign1D_BdJPsiKstarSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdJPsiKstarSignal"            );
TH1F * hDistSign1D_BsOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsOther"                      );
TH1F * hDistSign1D_BdOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdOther"                      );
TH1F * hDistSign1D_BOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_Other"                        );
TH1F * hDistSign1D_BsJpsiEta                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsJpsiEta"                    );
TH1F * hDistSign1D_BdJpsiK10                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdJpsiK10"                    );
TH1F * hDistSign1D_BdJpsiK0                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdJpsiK0"                     );
TH1F * hDistSign1D_BpJpsiKp                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BpJpsiKp"                     );
TH1F * hDistSign1D_JpsiOther                       =(TH1F*) JPsiMuMuHistoFile->Get("hDistSign1D_Other"                        );
TH1F * hDistSign1D_ppmumux                       =(TH1F*) ppmumuXHistoFile->Get("hDistSign1D_Other"                        );

TH1F * hPointingAngle_BsJPsiPhiSignal          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsJPsiPhiSignal"           );
TH1F * hPointingAngle_BsJPsiKKSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsJPsiKKSignal"            );
TH1F * hPointingAngle_BdJPsiKstarSignal        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdJPsiKstarSignal"         );
TH1F * hPointingAngle_BsOther                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsOther"                   );
TH1F * hPointingAngle_BdOther                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdOther"                   );
TH1F * hPointingAngle_BOther                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_Other"                     );
TH1F * hPointingAngle_BsJpsiEta                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsJpsiEta"                 );
TH1F * hPointingAngle_BdJpsiK10                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdJpsiK10"                 );
TH1F * hPointingAngle_BdJpsiK0                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdJpsiK0"                  );
TH1F * hPointingAngle_BpJpsiKp                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BpJpsiKp"                  );
TH1F * hPointingAngle_JpsiOther                    =(TH1F*) JPsiMuMuHistoFile->Get("hPointingAngle_Other"                     );
TH1F * hPointingAngle_ppmumux                    =(TH1F*) ppmumuXHistoFile->Get("hPointingAngle_Other"                     );

TH1F * hChi2_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsJPsiPhiSignal"                    );
TH1F * hChi2_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsJPsiKKSignal"                     );
TH1F * hChi2_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdJPsiKstarSignal"                  );
TH1F * hChi2_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsOther"                            );
TH1F * hChi2_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdOther"                            );
TH1F * hChi2_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_Other"                              );
TH1F * hChi2_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsJpsiEta"                          );
TH1F * hChi2_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdJpsiK10"                          );
TH1F * hChi2_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdJpsiK0"                           );
TH1F * hChi2_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BpJpsiKp"                           );
TH1F * hChi2_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hChi2_Other"                              );
TH1F * hChi2_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hChi2_Other"                              );

TH1F * hChi2Ndof_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsJPsiPhiSignal"                );
TH1F * hChi2Ndof_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsJPsiKKSignal"                 );
TH1F * hChi2Ndof_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdJPsiKstarSignal"              );
TH1F * hChi2Ndof_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsOther"                        );
TH1F * hChi2Ndof_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdOther"                        );
TH1F * hChi2Ndof_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_Other"                          );
TH1F * hChi2Ndof_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsJpsiEta"                      );
TH1F * hChi2Ndof_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdJpsiK10"                      );
TH1F * hChi2Ndof_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdJpsiK0"                       );
TH1F * hChi2Ndof_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BpJpsiKp"                       );
TH1F * hChi2Ndof_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hChi2Ndof_Other"                          );
TH1F * hChi2Ndof_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hChi2Ndof_Other"                          );

TH1F * hBsVtxProb_BsJPsiPhiSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsJPsiPhiSignal"               );
TH1F * hBsVtxProb_BsJPsiKKSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsJPsiKKSignal"                );
TH1F * hBsVtxProb_BdJPsiKstarSignal            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdJPsiKstarSignal"             );
TH1F * hBsVtxProb_BsOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsOther"                       );
TH1F * hBsVtxProb_BdOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdOther"                       );
TH1F * hBsVtxProb_BOther                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_Other"                         );
TH1F * hBsVtxProb_BsJpsiEta                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsJpsiEta"                     );
TH1F * hBsVtxProb_BdJpsiK10                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdJpsiK10"                     );
TH1F * hBsVtxProb_BdJpsiK0                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdJpsiK0"                      );
TH1F * hBsVtxProb_BpJpsiKp                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BpJpsiKp"                    );
TH1F * hBsVtxProb_JpsiOther                        =(TH1F*) JPsiMuMuHistoFile->Get("hBsVtxProb_Other"                         );
TH1F * hBsVtxProb_ppmumux                        =(TH1F*) ppmumuXHistoFile->Get("hBsVtxProb_Other"                         );

TH1F * hTime_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BsJPsiPhiSignal"                );
TH1F * hTime_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BsJPsiKKSignal"                 );
TH1F * hTime_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BdJPsiKstarSignal"              );
TH1F * hTime_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BsOther"                        );
TH1F * hTime_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BdOther"                        );
TH1F * hTime_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_Other"                          );
TH1F * hTime_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BsJpsiEta"                      );
TH1F * hTime_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BdJpsiK10"                      );
TH1F * hTime_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BdJpsiK0"                       );
TH1F * hTime_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hTime_BpJpsiKp"                       );
TH1F * hTime_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hTime_Other"                          );
TH1F * hTime_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hTime_Other"                          );

TH1F * hBsMass_NoTimeCut_BsJPsiPhiSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BsJPsiPhiSignal"               );
TH1F * hBsMass_NoTimeCut_BsJPsiKKSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BsJPsiKKSignal"                );
TH1F * hBsMass_NoTimeCut_BdJPsiKstarSignal            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BdJPsiKstarSignal"             );
TH1F * hBsMass_NoTimeCut_BsOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BsOther"                       );
TH1F * hBsMass_NoTimeCut_BdOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BdOther"                       );
TH1F * hBsMass_NoTimeCut_BOther                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_Other"                         );
TH1F * hBsMass_NoTimeCut_BsJpsiEta                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BsJpsiEta"                     );
TH1F * hBsMass_NoTimeCut_BdJpsiK10                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BdJpsiK10"                     );
TH1F * hBsMass_NoTimeCut_BdJpsiK0                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BdJpsiK0"                      );
TH1F * hBsMass_NoTimeCut_BpJpsiKp                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_BpJpsiKp"                    );
TH1F * hBsMass_NoTimeCut_JpsiOther                        =(TH1F*) JPsiMuMuHistoFile->Get("hBsMass_NoTimeCut_Other"                         );
TH1F * hBsMass_NoTimeCut_ppmumux                        =(TH1F*) ppmumuXHistoFile->Get("hBsMass_NoTimeCut_Other"                         );

TH1F * hBdChi2_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BsJPsiPhiSignal"                );
TH1F * hBdChi2_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BsJPsiKKSignal"                 );
TH1F * hBdChi2_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BdJPsiKstarSignal"              );
TH1F * hBdChi2_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BsOther"                        );
TH1F * hBdChi2_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BdOther"                        );
TH1F * hBdChi2_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_Other"                          );
TH1F * hBdChi2_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BsJpsiEta"                      );
TH1F * hBdChi2_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BdJpsiK10"                      );
TH1F * hBdChi2_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BdJpsiK0"                       );
TH1F * hBdChi2_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2_BpJpsiKp"                       );
TH1F * hBdChi2_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdChi2_Other"                          );
TH1F * hBdChi2_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdChi2_Other"                          );

TH1F * hBdChi2Ndof_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BsJPsiPhiSignal"                );
TH1F * hBdChi2Ndof_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BsJPsiKKSignal"                 );
TH1F * hBdChi2Ndof_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BdJPsiKstarSignal"              );
TH1F * hBdChi2Ndof_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BsOther"                        );
TH1F * hBdChi2Ndof_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BdOther"                        );
TH1F * hBdChi2Ndof_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_Other"                          );
TH1F * hBdChi2Ndof_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BsJpsiEta"                      );
TH1F * hBdChi2Ndof_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BdJpsiK10"                      );
TH1F * hBdChi2Ndof_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BdJpsiK0"                       );
TH1F * hBdChi2Ndof_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdChi2Ndof_BpJpsiKp"                       );
TH1F * hBdChi2Ndof_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdChi2Ndof_Other"                          );
TH1F * hBdChi2Ndof_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdChi2Ndof_Other"                          );


TH1F * hBdVtxProb_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BsJPsiPhiSignal"                );
TH1F * hBdVtxProb_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BsJPsiKKSignal"                 );
TH1F * hBdVtxProb_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BdJPsiKstarSignal"              );
TH1F * hBdVtxProb_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BsOther"                        );
TH1F * hBdVtxProb_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BdOther"                        );
TH1F * hBdVtxProb_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_Other"                          );
TH1F * hBdVtxProb_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BsJpsiEta"                      );
TH1F * hBdVtxProb_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BdJpsiK10"                      );
TH1F * hBdVtxProb_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BdJpsiK0"                       );
TH1F * hBdVtxProb_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdVtxProb_BpJpsiKp"                       );
TH1F * hBdVtxProb_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdVtxProb_Other"                          );
TH1F * hBdVtxProb_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdVtxProb_Other"                          );

TH1F * hBdK1Pt_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BsJPsiPhiSignal"                );
TH1F * hBdK1Pt_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BsJPsiKKSignal"                 );
TH1F * hBdK1Pt_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BdJPsiKstarSignal"              );
TH1F * hBdK1Pt_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BsOther"                        );
TH1F * hBdK1Pt_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BdOther"                        );
TH1F * hBdK1Pt_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_Other"                          );
TH1F * hBdK1Pt_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BsJpsiEta"                      );
TH1F * hBdK1Pt_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BdJpsiK10"                      );
TH1F * hBdK1Pt_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BdJpsiK0"                       );
TH1F * hBdK1Pt_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK1Pt_BpJpsiKp"                       );
TH1F * hBdK1Pt_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdK1Pt_Other"                          );
TH1F * hBdK1Pt_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdK1Pt_Other"                          );

TH1F * hBdK2Pt_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BsJPsiPhiSignal"                );
TH1F * hBdK2Pt_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BsJPsiKKSignal"                 );
TH1F * hBdK2Pt_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BdJPsiKstarSignal"              );
TH1F * hBdK2Pt_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BsOther"                        );
TH1F * hBdK2Pt_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BdOther"                        );
TH1F * hBdK2Pt_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_Other"                          );
TH1F * hBdK2Pt_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BsJpsiEta"                      );
TH1F * hBdK2Pt_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BdJpsiK10"                      );
TH1F * hBdK2Pt_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BdJpsiK0"                       );
TH1F * hBdK2Pt_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdK2Pt_BpJpsiKp"                       );
TH1F * hBdK2Pt_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdK2Pt_Other"                          );
TH1F * hBdK2Pt_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdK2Pt_Other"                          );

TH1F * hKstarMass_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BsJPsiPhiSignal"                );
TH1F * hKstarMass_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BsJPsiKKSignal"                 );
TH1F * hKstarMass_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BdJPsiKstarSignal"              );
TH1F * hKstarMass_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BsOther"                        );
TH1F * hKstarMass_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BdOther"                        );
TH1F * hKstarMass_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_Other"                          );
TH1F * hKstarMass_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BsJpsiEta"                      );
TH1F * hKstarMass_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BdJpsiK10"                      );
TH1F * hKstarMass_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BdJpsiK0"                       );
TH1F * hKstarMass_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hKstarMass_BpJpsiKp"                       );
TH1F * hKstarMass_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hKstarMass_Other"                          );
TH1F * hKstarMass_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hKstarMass_Other"                          );

TH1F * hBdDistSign3D_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BsJPsiPhiSignal"                );
TH1F * hBdDistSign3D_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BsJPsiKKSignal"                 );
TH1F * hBdDistSign3D_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BdJPsiKstarSignal"              );
TH1F * hBdDistSign3D_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BsOther"                        );
TH1F * hBdDistSign3D_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BdOther"                        );
TH1F * hBdDistSign3D_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_Other"                          );
TH1F * hBdDistSign3D_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BsJpsiEta"                      );
TH1F * hBdDistSign3D_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BdJpsiK10"                      );
TH1F * hBdDistSign3D_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BdJpsiK0"                       );
TH1F * hBdDistSign3D_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign3D_BpJpsiKp"                       );
TH1F * hBdDistSign3D_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdDistSign3D_Other"                          );
TH1F * hBdDistSign3D_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdDistSign3D_Other"                          );

TH1F * hBdDistSign1D_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BsJPsiPhiSignal"                );
TH1F * hBdDistSign1D_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BsJPsiKKSignal"                 );
TH1F * hBdDistSign1D_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BdJPsiKstarSignal"              );
TH1F * hBdDistSign1D_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BsOther"                        );
TH1F * hBdDistSign1D_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BdOther"                        );
TH1F * hBdDistSign1D_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_Other"                          );
TH1F * hBdDistSign1D_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BsJpsiEta"                      );
TH1F * hBdDistSign1D_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BdJpsiK10"                      );
TH1F * hBdDistSign1D_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BdJpsiK0"                       );
TH1F * hBdDistSign1D_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDistSign1D_BpJpsiKp"                       );
TH1F * hBdDistSign1D_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdDistSign1D_Other"                          );
TH1F * hBdDistSign1D_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdDistSign1D_Other"                          );

TH1F * hBdDist3D_BsJPsiPhiSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BsJPsiPhiSignal"                  );
TH1F * hBdDist3D_BsJPsiKKSignal                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BsJPsiKKSignal"                   );
TH1F * hBdDist3D_BdJPsiKstarSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BdJPsiKstarSignal"                );
TH1F * hBdDist3D_BsOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BsOther"                          );
TH1F * hBdDist3D_BdOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BdOther"                          );
TH1F * hBdDist3D_BOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_Other"                            );
TH1F * hBdDist3D_BsJpsiEta                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BsJpsiEta"                        );
TH1F * hBdDist3D_BdJpsiK10                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BdJpsiK10"                        );
TH1F * hBdDist3D_BdJpsiK0                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BdJpsiK0"                         );
TH1F * hBdDist3D_BpJpsiKp                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdDist3D_BpJpsiKp"                         );
TH1F * hBdDist3D_JpsiOther                           =(TH1F*) JPsiMuMuHistoFile->Get("hBdDist3D_Other"                            );
TH1F * hBdDist3D_ppmumux                           =(TH1F*) ppmumuXHistoFile->Get("hBdDist3D_Other"                            );

TH1F * hBdTime_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BsJPsiPhiSignal"                );
TH1F * hBdTime_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BsJPsiKKSignal"                 );
TH1F * hBdTime_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BdJPsiKstarSignal"              );
TH1F * hBdTime_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BsOther"                        );
TH1F * hBdTime_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BdOther"                        );
TH1F * hBdTime_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_Other"                          );
TH1F * hBdTime_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BsJpsiEta"                      );
TH1F * hBdTime_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BdJpsiK10"                      );
TH1F * hBdTime_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BdJpsiK0"                       );
TH1F * hBdTime_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdTime_BpJpsiKp"                       );
TH1F * hBdTime_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdTime_Other"                          );
TH1F * hBdTime_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdTime_Other"                          );

TH1F * hBdMass_NoTimeCut_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BsJPsiPhiSignal"                );
TH1F * hBdMass_NoTimeCut_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BsJPsiKKSignal"                 );
TH1F * hBdMass_NoTimeCut_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BdJPsiKstarSignal"              );
TH1F * hBdMass_NoTimeCut_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BsOther"                        );
TH1F * hBdMass_NoTimeCut_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BdOther"                        );
TH1F * hBdMass_NoTimeCut_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_Other"                          );
TH1F * hBdMass_NoTimeCut_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BsJpsiEta"                      );
TH1F * hBdMass_NoTimeCut_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BdJpsiK10"                      );
TH1F * hBdMass_NoTimeCut_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BdJpsiK0"                       );
TH1F * hBdMass_NoTimeCut_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_BpJpsiKp"                       );
TH1F * hBdMass_NoTimeCut_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdMass_NoTimeCut_Other"                          );
TH1F * hBdMass_NoTimeCut_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdMass_NoTimeCut_Other"                          );

TH1F * hBdMassFinal_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BsJPsiPhiSignal"                );
TH1F * hBdMassFinal_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BsJPsiKKSignal"                 );
TH1F * hBdMassFinal_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BdJPsiKstarSignal"              );
TH1F * hBdMassFinal_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BsOther"                        );
TH1F * hBdMassFinal_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BdOther"                        );
TH1F * hBdMassFinal_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_Other"                          );
TH1F * hBdMassFinal_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BsJpsiEta"                      );
TH1F * hBdMassFinal_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BdJpsiK10"                      );
TH1F * hBdMassFinal_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BdJpsiK0"                       );
TH1F * hBdMassFinal_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinal_BpJpsiKp"                       );
TH1F * hBdMassFinal_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdMassFinal_Other"                          );
TH1F * hBdMassFinal_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdMassFinal_Other"                          );

TH1F * hBdMassFinalAfterFit_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BsJPsiPhiSignal"                );
TH1F * hBdMassFinalAfterFit_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BsJPsiKKSignal"                 );
TH1F * hBdMassFinalAfterFit_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BdJPsiKstarSignal"              );
TH1F * hBdMassFinalAfterFit_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BsOther"                        );
TH1F * hBdMassFinalAfterFit_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BdOther"                        );
TH1F * hBdMassFinalAfterFit_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_Other"                          );
TH1F * hBdMassFinalAfterFit_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BsJpsiEta"                      );
TH1F * hBdMassFinalAfterFit_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BdJpsiK10"                      );
TH1F * hBdMassFinalAfterFit_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BdJpsiK0"                       );
TH1F * hBdMassFinalAfterFit_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_BpJpsiKp"                       );
TH1F * hBdMassFinalAfterFit_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hBdMassFinalAfterFit_Other"                          );
TH1F * hBdMassFinalAfterFit_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hBdMassFinalAfterFit_Other"                          );

TH1F * hResoLxy_BsJPsiPhiSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hResoLxy_BsJPsiPhiSignal"                  );
TH1F * hResoTime_BsJPsiPhiSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hResoTime_BsJPsiPhiSignal"                  );

 TH1F * hBsCt_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BsJPsiPhiSignal"                    );
 TH1F * hBsCt_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BsJPsiKKSignal"                     );
 TH1F * hBsCt_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BdJPsiKstarSignal"                  );
 TH1F * hBsCt_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BsOther"                            );
 TH1F * hBsCt_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BdOther"                            );
 TH1F * hBsCt_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_Other"                              );
 TH1F * hBsCt_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BsJpsiEta"                          );
 TH1F * hBsCt_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BdJpsiK10"                          );
 TH1F * hBsCt_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BdJpsiK0"                           );
 TH1F * hBsCt_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCt_BpJpsiKp"                           );
 TH1F * hBsCt_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hBsCt_Other"                              );
 TH1F * hBsCt_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hBsCt_Other"                              );

 TH1F * hBsCtErr_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BsJPsiPhiSignal"                    );
 TH1F * hBsCtErr_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BsJPsiKKSignal"                     );
 TH1F * hBsCtErr_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BdJPsiKstarSignal"                  );
 TH1F * hBsCtErr_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BsOther"                            );
 TH1F * hBsCtErr_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BdOther"                            );
 TH1F * hBsCtErr_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_Other"                              );
 TH1F * hBsCtErr_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BsJpsiEta"                          );
 TH1F * hBsCtErr_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BdJpsiK10"                          );
 TH1F * hBsCtErr_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BdJpsiK0"                           );
 TH1F * hBsCtErr_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsCtErr_BpJpsiKp"                           );
 TH1F * hBsCtErr_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hBsCtErr_Other"                              );
 TH1F * hBsCtErr_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hBsCtErr_Other"                              );

 ScaleHistograms(
 hPhiMassFinal_BsJPsiPhiSignal  ,  
 hPhiMassFinal_BsJPsiKKSignal   ,  
 hPhiMassFinal_BdJPsiKstarSignal,  
 hPhiMassFinal_BsOther          ,  
 hPhiMassFinal_BdOther          ,  
 hPhiMassFinal_BOther           ,  
 hPhiMassFinal_BsJpsiEta        ,  
 hPhiMassFinal_BdJpsiK10        ,  
 hPhiMassFinal_BdJpsiK0         ,  
 hPhiMassFinal_BpJpsiKp         ,  
 hPhiMassFinal_JpsiOther        ,
 hPhiMassFinal_ppmumux          );  

 ScaleHistograms(
hJPsiMass_BsJPsiPhiSignal   ,            
hJPsiMass_BsJPsiKKSignal    ,            
hJPsiMass_BdJPsiKstarSignal ,            
hJPsiMass_BsOther           ,            
hJPsiMass_BdOther           ,            
hJPsiMass_BOther            ,            
hJPsiMass_BsJpsiEta         ,            
hJPsiMass_BdJpsiK10         ,            
hJPsiMass_BdJpsiK0          ,            
hJPsiMass_BpJpsiKp          ,            
hJPsiMass_JpsiOther         ,
hJPsiMass_ppmumux);           
              
 ScaleHistograms(                         
hPhiMass_BsJPsiPhiSignal    ,            
hPhiMass_BsJPsiKKSignal     ,            
hPhiMass_BdJPsiKstarSignal  ,            
hPhiMass_BsOther            ,            
hPhiMass_BdOther            ,            
hPhiMass_BOther             ,            
hPhiMass_BsJpsiEta          ,            
hPhiMass_BdJpsiK10          ,            
hPhiMass_BdJpsiK0           ,            
hPhiMass_BpJpsiKp           ,            
hPhiMass_JpsiOther,
hPhiMass_ppmumux          );           
                                       
    ScaleHistograms(                                    
hPhiMassFinal_BsJPsiPhiSignal    ,       
hPhiMassFinal_BsJPsiKKSignal     ,       
hPhiMassFinal_BdJPsiKstarSignal  ,       
hPhiMassFinal_BsOther            ,       
hPhiMassFinal_BdOther            ,       
hPhiMassFinal_BOther             ,       
hPhiMassFinal_BsJpsiEta          ,       
hPhiMassFinal_BdJpsiK10          ,       
hPhiMassFinal_BdJpsiK0           ,       
hPhiMassFinal_BpJpsiKp           ,       
hPhiMassFinal_JpsiOther,
hPhiMassFinal_ppmumux          );      
                                       
    ScaleHistograms(                                    
hBsMassFinal_BsJPsiPhiSignal     ,       
hBsMassFinal_BsJPsiKKSignal      ,       
hBsMassFinal_BdJPsiKstarSignal   ,       
hBsMassFinal_BsOther             ,       
hBsMassFinal_BdOther             ,       
hBsMassFinal_BOther              ,       
hBsMassFinal_BsJpsiEta           ,       
hBsMassFinal_BdJpsiK10           ,       
hBsMassFinal_BdJpsiK0            ,       
hBsMassFinal_BpJpsiKp            ,       
hBsMassFinal_JpsiOther,
hBsMassFinal_ppmumux           );      
       
 ScaleHistograms(                                
hBsMassFinalAfterFit_BsJPsiPhiSignal   , 
hBsMassFinalAfterFit_BsJPsiKKSignal    , 
hBsMassFinalAfterFit_BdJPsiKstarSignal , 
hBsMassFinalAfterFit_BsOther           , 
hBsMassFinalAfterFit_BdOther           , 
hBsMassFinalAfterFit_BOther            , 
hBsMassFinalAfterFit_BsJpsiEta         , 
hBsMassFinalAfterFit_BdJpsiK10         , 
hBsMassFinalAfterFit_BdJpsiK0          , 
hBsMassFinalAfterFit_BpJpsiKp          , 
hBsMassFinalAfterFit_JpsiOther,
hBsMassFinalAfterFit_ppmumux         );
     
 ScaleHistograms(                                  
hK1Pt_BsJPsiPhiSignal   ,                
hK1Pt_BsJPsiKKSignal    ,                
hK1Pt_BdJPsiKstarSignal ,                
hK1Pt_BsOther           ,                
hK1Pt_BdOther           ,                
hK1Pt_BOther            ,                
hK1Pt_BsJpsiEta         ,                
hK1Pt_BdJpsiK10         ,                
hK1Pt_BdJpsiK0          ,                
hK1Pt_BpJpsiKp          ,                
hK1Pt_JpsiOther ,
hK1Pt_ppmumux        );               
    
 ScaleHistograms(                                   
hK2Pt_BsJPsiPhiSignal    ,               
hK2Pt_BsJPsiKKSignal     ,               
hK2Pt_BdJPsiKstarSignal  ,               
hK2Pt_BsOther            ,               
hK2Pt_BdOther            ,               
hK2Pt_BOther             ,               
hK2Pt_BsJpsiEta          ,               
hK2Pt_BdJpsiK10          ,               
hK2Pt_BdJpsiK0           ,               
hK2Pt_BpJpsiKp           ,               
hK2Pt_JpsiOther,
hK2Pt_ppmumux          );              
   
 ScaleHistograms(                                    
hDist3D_BsJPsiPhiSignal   ,              
hDist3D_BsJPsiKKSignal    ,              
hDist3D_BdJPsiKstarSignal ,              
hDist3D_BsOther           ,              
hDist3D_BdOther           ,              
hDist3D_BOther            ,              
hDist3D_BsJpsiEta         ,              
hDist3D_BdJpsiK10         ,              
hDist3D_BdJpsiK0          ,              
hDist3D_BpJpsiKp          ,              
hDist3D_JpsiOther,
hDist3D_ppmumux         );             
  
 ScaleHistograms(                                     
hDistSign3D_BsJPsiPhiSignal    ,         
hDistSign3D_BsJPsiKKSignal     ,         
hDistSign3D_BdJPsiKstarSignal  ,         
hDistSign3D_BsOther            ,         
hDistSign3D_BdOther            ,         
hDistSign3D_BOther             ,         
hDistSign3D_BsJpsiEta          ,         
hDistSign3D_BdJpsiK10          ,         
hDistSign3D_BdJpsiK0           ,         
hDistSign3D_BpJpsiKp           ,         
hDistSign3D_JpsiOther ,
hDistSign3D_ppmumux         );        
  
 ScaleHistograms(                                     
hDistSign1D_BsJPsiPhiSignal   ,          
hDistSign1D_BsJPsiKKSignal    ,          
hDistSign1D_BdJPsiKstarSignal ,          
hDistSign1D_BsOther           ,          
hDistSign1D_BdOther           ,          
hDistSign1D_BOther            ,          
hDistSign1D_BsJpsiEta         ,          
hDistSign1D_BdJpsiK10         ,          
hDistSign1D_BdJpsiK0          ,          
hDistSign1D_BpJpsiKp          ,          
hDistSign1D_JpsiOther,
hDistSign1D_ppmumux        );         
          
 ScaleHistograms(                             
hPointingAngle_BsJPsiPhiSignal   ,       
hPointingAngle_BsJPsiKKSignal    ,       
hPointingAngle_BdJPsiKstarSignal ,       
hPointingAngle_BsOther           ,       
hPointingAngle_BdOther           ,       
hPointingAngle_BOther            ,       
hPointingAngle_BsJpsiEta         ,       
hPointingAngle_BdJpsiK10         ,       
hPointingAngle_BdJpsiK0          ,       
hPointingAngle_BpJpsiKp          ,       
hPointingAngle_JpsiOther ,
hPointingAngle_ppmumux        );      
   
 ScaleHistograms(                                    
hChi2_BsJPsiPhiSignal     ,              
hChi2_BsJPsiKKSignal      ,              
hChi2_BdJPsiKstarSignal   ,              
hChi2_BsOther             ,              
hChi2_BdOther             ,              
hChi2_BOther              ,              
hChi2_BsJpsiEta           ,              
hChi2_BdJpsiK10           ,              
hChi2_BdJpsiK0            ,              
hChi2_BpJpsiKp            ,              
hChi2_JpsiOther,
hChi2_ppmumux         );             
        
 ScaleHistograms(                               
hChi2Ndof_BsJPsiPhiSignal   ,            
hChi2Ndof_BsJPsiKKSignal    ,            
hChi2Ndof_BdJPsiKstarSignal ,            
hChi2Ndof_BsOther           ,            
hChi2Ndof_BdOther           ,            
hChi2Ndof_BOther            ,            
hChi2Ndof_BsJpsiEta         ,            
hChi2Ndof_BdJpsiK10         ,            
hChi2Ndof_BdJpsiK0          ,            
hChi2Ndof_BpJpsiKp          ,            
hChi2Ndof_JpsiOther,
hChi2Ndof_ppmumux         );           
     
 ScaleHistograms(                                  
hBsVtxProb_BsJPsiPhiSignal    ,          
hBsVtxProb_BsJPsiKKSignal     ,          
hBsVtxProb_BdJPsiKstarSignal  ,          
hBsVtxProb_BsOther            ,          
hBsVtxProb_BdOther            ,          
hBsVtxProb_BOther             ,          
hBsVtxProb_BsJpsiEta          ,          
hBsVtxProb_BdJpsiK10          ,          
hBsVtxProb_BdJpsiK0           ,          
hBsVtxProb_BpJpsiKp           ,          
hBsVtxProb_JpsiOther,
hBsVtxProb_ppmumux         );         

 ScaleHistograms(                               
hTime_BsJPsiPhiSignal   ,            
hTime_BsJPsiKKSignal    ,            
hTime_BdJPsiKstarSignal ,            
hTime_BsOther           ,            
hTime_BdOther           ,            
hTime_BOther            ,            
hTime_BsJpsiEta         ,            
hTime_BdJpsiK10         ,            
hTime_BdJpsiK0          ,            
hTime_BpJpsiKp          ,            
hTime_JpsiOther,
hTime_ppmumux         );           
     
 ScaleHistograms(                                  
hBsMass_NoTimeCut_BsJPsiPhiSignal    ,          
hBsMass_NoTimeCut_BsJPsiKKSignal     ,          
hBsMass_NoTimeCut_BdJPsiKstarSignal  ,          
hBsMass_NoTimeCut_BsOther            ,          
hBsMass_NoTimeCut_BdOther            ,          
hBsMass_NoTimeCut_BOther             ,          
hBsMass_NoTimeCut_BsJpsiEta          ,          
hBsMass_NoTimeCut_BdJpsiK10          ,          
hBsMass_NoTimeCut_BdJpsiK0           ,          
hBsMass_NoTimeCut_BpJpsiKp           ,          
hBsMass_NoTimeCut_JpsiOther,
hBsMass_NoTimeCut_ppmumux         );         

 ScaleHistograms(                               
hBdChi2_BsJPsiPhiSignal   ,            
hBdChi2_BsJPsiKKSignal    ,            
hBdChi2_BdJPsiKstarSignal ,            
hBdChi2_BsOther           ,            
hBdChi2_BdOther           ,            
hBdChi2_BOther            ,            
hBdChi2_BsJpsiEta         ,            
hBdChi2_BdJpsiK10         ,            
hBdChi2_BdJpsiK0          ,            
hBdChi2_BpJpsiKp          ,            
hBdChi2_JpsiOther,
hBdChi2_ppmumux         );           


 ScaleHistograms(                               
hBdChi2Ndof_BsJPsiPhiSignal   ,            
hBdChi2Ndof_BsJPsiKKSignal    ,            
hBdChi2Ndof_BdJPsiKstarSignal ,            
hBdChi2Ndof_BsOther           ,            
hBdChi2Ndof_BdOther           ,            
hBdChi2Ndof_BOther            ,            
hBdChi2Ndof_BsJpsiEta         ,            
hBdChi2Ndof_BdJpsiK10         ,            
hBdChi2Ndof_BdJpsiK0          ,            
hBdChi2Ndof_BpJpsiKp          ,            
hBdChi2Ndof_JpsiOther,
hBdChi2Ndof_ppmumux         );           

 ScaleHistograms(                               
hBdVtxProb_BsJPsiPhiSignal   ,            
hBdVtxProb_BsJPsiKKSignal    ,            
hBdVtxProb_BdJPsiKstarSignal ,            
hBdVtxProb_BsOther           ,            
hBdVtxProb_BdOther           ,            
hBdVtxProb_BOther            ,            
hBdVtxProb_BsJpsiEta         ,            
hBdVtxProb_BdJpsiK10         ,            
hBdVtxProb_BdJpsiK0          ,            
hBdVtxProb_BpJpsiKp          ,            
hBdVtxProb_JpsiOther,
hBdVtxProb_ppmumux         );           

 ScaleHistograms(                               
hBdK1Pt_BsJPsiPhiSignal   ,            
hBdK1Pt_BsJPsiKKSignal    ,            
hBdK1Pt_BdJPsiKstarSignal ,            
hBdK1Pt_BsOther           ,            
hBdK1Pt_BdOther           ,            
hBdK1Pt_BOther            ,            
hBdK1Pt_BsJpsiEta         ,            
hBdK1Pt_BdJpsiK10         ,            
hBdK1Pt_BdJpsiK0          ,            
hBdK1Pt_BpJpsiKp          ,            
hBdK1Pt_JpsiOther,
hBdK1Pt_ppmumux         );           
 

 ScaleHistograms(                               
hBdK2Pt_BsJPsiPhiSignal   ,            
hBdK2Pt_BsJPsiKKSignal    ,            
hBdK2Pt_BdJPsiKstarSignal ,            
hBdK2Pt_BsOther           ,            
hBdK2Pt_BdOther           ,            
hBdK2Pt_BOther            ,            
hBdK2Pt_BsJpsiEta         ,            
hBdK2Pt_BdJpsiK10         ,            
hBdK2Pt_BdJpsiK0          ,            
hBdK2Pt_BpJpsiKp          ,            
hBdK2Pt_JpsiOther,
hBdK2Pt_ppmumux         );           

 ScaleHistograms(                               
hKstarMass_BsJPsiPhiSignal   ,            
hKstarMass_BsJPsiKKSignal    ,            
hKstarMass_BdJPsiKstarSignal ,            
hKstarMass_BsOther           ,            
hKstarMass_BdOther           ,            
hKstarMass_BOther            ,            
hKstarMass_BsJpsiEta         ,            
hKstarMass_BdJpsiK10         ,            
hKstarMass_BdJpsiK0          ,            
hKstarMass_BpJpsiKp          ,            
hKstarMass_JpsiOther,
hKstarMass_ppmumux         );           

 ScaleHistograms(                               
hBdDistSign3D_BsJPsiPhiSignal   ,            
hBdDistSign3D_BsJPsiKKSignal    ,            
hBdDistSign3D_BdJPsiKstarSignal ,            
hBdDistSign3D_BsOther           ,            
hBdDistSign3D_BdOther           ,            
hBdDistSign3D_BOther            ,            
hBdDistSign3D_BsJpsiEta         ,            
hBdDistSign3D_BdJpsiK10         ,            
hBdDistSign3D_BdJpsiK0          ,            
hBdDistSign3D_BpJpsiKp          ,            
hBdDistSign3D_JpsiOther,
hBdDistSign3D_ppmumux         );           

 ScaleHistograms(                               
hBdDistSign1D_BsJPsiPhiSignal   ,            
hBdDistSign1D_BsJPsiKKSignal    ,            
hBdDistSign1D_BdJPsiKstarSignal ,            
hBdDistSign1D_BsOther           ,            
hBdDistSign1D_BdOther           ,            
hBdDistSign1D_BOther            ,            
hBdDistSign1D_BsJpsiEta         ,            
hBdDistSign1D_BdJpsiK10         ,            
hBdDistSign1D_BdJpsiK0          ,            
hBdDistSign1D_BpJpsiKp          ,            
hBdDistSign1D_JpsiOther,
hBdDistSign1D_ppmumux         );           

 ScaleHistograms(                               
hBdDist3D_BsJPsiPhiSignal   ,            
hBdDist3D_BsJPsiKKSignal    ,            
hBdDist3D_BdJPsiKstarSignal ,            
hBdDist3D_BsOther           ,            
hBdDist3D_BdOther           ,            
hBdDist3D_BOther            ,            
hBdDist3D_BsJpsiEta         ,            
hBdDist3D_BdJpsiK10         ,            
hBdDist3D_BdJpsiK0          ,            
hBdDist3D_BpJpsiKp          ,            
hBdDist3D_JpsiOther,
hBdDist3D_ppmumux         );           

 ScaleHistograms(                               
hBdTime_BsJPsiPhiSignal   ,            
hBdTime_BsJPsiKKSignal    ,            
hBdTime_BdJPsiKstarSignal ,            
hBdTime_BsOther           ,            
hBdTime_BdOther           ,            
hBdTime_BOther            ,            
hBdTime_BsJpsiEta         ,            
hBdTime_BdJpsiK10         ,            
hBdTime_BdJpsiK0          ,            
hBdTime_BpJpsiKp          ,            
hBdTime_JpsiOther,
hBdTime_ppmumux         );           


 ScaleHistograms(                               
hBdMass_NoTimeCut_BsJPsiPhiSignal   ,            
hBdMass_NoTimeCut_BsJPsiKKSignal    ,            
hBdMass_NoTimeCut_BdJPsiKstarSignal ,            
hBdMass_NoTimeCut_BsOther           ,            
hBdMass_NoTimeCut_BdOther           ,            
hBdMass_NoTimeCut_BOther            ,            
hBdMass_NoTimeCut_BsJpsiEta         ,            
hBdMass_NoTimeCut_BdJpsiK10         ,            
hBdMass_NoTimeCut_BdJpsiK0          ,            
hBdMass_NoTimeCut_BpJpsiKp          ,            
hBdMass_NoTimeCut_JpsiOther,
hBdMass_NoTimeCut_ppmumux         );           

 ScaleHistograms(                               
hBdMassFinal_BsJPsiPhiSignal   ,            
hBdMassFinal_BsJPsiKKSignal    ,            
hBdMassFinal_BdJPsiKstarSignal ,            
hBdMassFinal_BsOther           ,            
hBdMassFinal_BdOther           ,            
hBdMassFinal_BOther            ,            
hBdMassFinal_BsJpsiEta         ,            
hBdMassFinal_BdJpsiK10         ,            
hBdMassFinal_BdJpsiK0          ,            
hBdMassFinal_BpJpsiKp          ,            
hBdMassFinal_JpsiOther,
hBdMassFinal_ppmumux         );           

 ScaleHistograms(                               
hBdMassFinalAfterFit_BsJPsiPhiSignal   ,            
hBdMassFinalAfterFit_BsJPsiKKSignal    ,            
hBdMassFinalAfterFit_BdJPsiKstarSignal ,            
hBdMassFinalAfterFit_BsOther           ,            
hBdMassFinalAfterFit_BdOther           ,            
hBdMassFinalAfterFit_BOther            ,            
hBdMassFinalAfterFit_BsJpsiEta         ,            
hBdMassFinalAfterFit_BdJpsiK10         ,            
hBdMassFinalAfterFit_BdJpsiK0          ,            
hBdMassFinalAfterFit_BpJpsiKp          ,            
hBdMassFinalAfterFit_JpsiOther,
hBdMassFinalAfterFit_ppmumux         );           

 ScaleHistograms(
                 hBsCt_BsJPsiPhiSignal  ,
                 hBsCt_BsJPsiKKSignal   ,
                 hBsCt_BdJPsiKstarSignal,
                 hBsCt_BsOther          ,
                 hBsCt_BdOther          ,
                 hBsCt_BOther           ,
                 hBsCt_BsJpsiEta        ,
                 hBsCt_BdJpsiK10        ,
                 hBsCt_BdJpsiK0         ,
                 hBsCt_BpJpsiKp         ,
                 hBsCt_JpsiOther        ,
                 hBsCt_ppmumux          );

 ScaleHistograms(
                 hBsCtErr_BsJPsiPhiSignal  ,
                 hBsCtErr_BsJPsiKKSignal   ,
                 hBsCtErr_BdJPsiKstarSignal,
                 hBsCtErr_BsOther          ,
                 hBsCtErr_BdOther          ,
                 hBsCtErr_BOther           ,
                 hBsCtErr_BsJpsiEta        ,
                 hBsCtErr_BdJpsiK10        ,
                 hBsCtErr_BdJpsiK0         ,
                 hBsCtErr_BpJpsiKp         ,
                 hBsCtErr_JpsiOther        ,
                 hBsCtErr_ppmumux          );


 
 //****************************************
 // create summed histograms for background



 TH1F* hJPsiMass_BBackground =  addBackgrounds("hJPsiMass_BBackground",
					       hJPsiMass_BsJPsiPhiSignal   ,            
					       hJPsiMass_BsJPsiKKSignal    ,            
					       hJPsiMass_BdJPsiKstarSignal ,            
					       hJPsiMass_BsOther           ,            
					       hJPsiMass_BdOther           ,            
					       hJPsiMass_BOther            ,            
					       hJPsiMass_BsJpsiEta         ,            
					       hJPsiMass_BdJpsiK10         ,            
					       hJPsiMass_BdJpsiK0          ,            
					       hJPsiMass_BpJpsiKp          ,            
					       hJPsiMass_JpsiOther,
					       hJPsiMass_ppmumux);           
              
 TH1F* hPhiMass_BBackground =  addBackgrounds("hPhiMass_BBackground",                        
					      hPhiMass_BsJPsiPhiSignal    ,            
					      hPhiMass_BsJPsiKKSignal     ,            
					      hPhiMass_BdJPsiKstarSignal  ,            
					      hPhiMass_BsOther            ,            
					      hPhiMass_BdOther            ,            
					      hPhiMass_BOther             ,            
					      hPhiMass_BsJpsiEta          ,            
					      hPhiMass_BdJpsiK10          ,            
					      hPhiMass_BdJpsiK0           ,            
					      hPhiMass_BpJpsiKp           ,            
					      hPhiMass_JpsiOther,
					      hPhiMass_ppmumux);           
                                       
  TH1F* hPhiMassFinal_BBackground =  addBackgrounds("hPhiMassFinal_BBackground",
						    hPhiMassFinal_BsJPsiPhiSignal    ,       
						    hPhiMassFinal_BsJPsiKKSignal     ,       
						    hPhiMassFinal_BdJPsiKstarSignal  ,       
						    hPhiMassFinal_BsOther            ,       
						    hPhiMassFinal_BdOther            ,       
						    hPhiMassFinal_BOther             ,       
						    hPhiMassFinal_BsJpsiEta          ,       
						    hPhiMassFinal_BdJpsiK10          ,       
						    hPhiMassFinal_BdJpsiK0           ,       
						    hPhiMassFinal_BpJpsiKp           ,       
						    hPhiMassFinal_JpsiOther ,
						    hPhiMassFinal_ppmumux);      
                                       
     TH1F* hBsMassFinal_BBackground =  addBackgrounds("hBsMassFinal_BBackground",                                    
						      hBsMassFinal_BsJPsiPhiSignal     ,       
						      hBsMassFinal_BsJPsiKKSignal      ,       
						      hBsMassFinal_BdJPsiKstarSignal   ,       
						      hBsMassFinal_BsOther             ,       
						      hBsMassFinal_BdOther             ,       
						      hBsMassFinal_BOther              ,       
						      hBsMassFinal_BsJpsiEta           ,       
						      hBsMassFinal_BdJpsiK10           ,       
						      hBsMassFinal_BdJpsiK0            ,       
						      hBsMassFinal_BpJpsiKp            ,       
						      hBsMassFinal_JpsiOther ,
						      hBsMassFinal_ppmumux);      
     

  TH1F* hBsMassFinalAfterFit_BBackground =  addBackgrounds("hBsMassFinalAfterFit_BBackground",                                
							   hBsMassFinalAfterFit_BsJPsiPhiSignal   , 
							   hBsMassFinalAfterFit_BsJPsiKKSignal    , 
							   hBsMassFinalAfterFit_BdJPsiKstarSignal , 
							   hBsMassFinalAfterFit_BsOther           , 
							   hBsMassFinalAfterFit_BdOther           , 
							   hBsMassFinalAfterFit_BOther            , 
							   hBsMassFinalAfterFit_BsJpsiEta         , 
							   hBsMassFinalAfterFit_BdJpsiK10         , 
							   hBsMassFinalAfterFit_BdJpsiK0          , 
							   hBsMassFinalAfterFit_BpJpsiKp          , 
							   hBsMassFinalAfterFit_JpsiOther ,
							   hBsMassFinalAfterFit_ppmumux);
  
  TH1F* hK1Pt_BBackground =  addBackgrounds("hK1Pt_BBackground",                                  
					    hK1Pt_BsJPsiPhiSignal   ,                
					    hK1Pt_BsJPsiKKSignal    ,                
					    hK1Pt_BdJPsiKstarSignal ,                
					    hK1Pt_BsOther           ,                
					    hK1Pt_BdOther           ,                
					    hK1Pt_BOther            ,                
					    hK1Pt_BsJpsiEta         ,                
					    hK1Pt_BdJpsiK10         ,                
					    hK1Pt_BdJpsiK0          ,                
					    hK1Pt_BpJpsiKp          ,                
					    hK1Pt_JpsiOther ,
					    hK1Pt_ppmumux);               
    

  TH1F* hK2Pt_BBackground =  addBackgrounds("hK2Pt_BBackground",                                   
					    hK2Pt_BsJPsiPhiSignal    ,               
					    hK2Pt_BsJPsiKKSignal     ,               
					    hK2Pt_BdJPsiKstarSignal  ,               
					    hK2Pt_BsOther            ,               
					    hK2Pt_BdOther            ,               
					    hK2Pt_BOther             ,               
					    hK2Pt_BsJpsiEta          ,               
					    hK2Pt_BdJpsiK10          ,               
					    hK2Pt_BdJpsiK0           ,               
					    hK2Pt_BpJpsiKp           ,               
					    hK2Pt_JpsiOther ,
					    hK2Pt_ppmumux);              
  

  TH1F* hDist3D_BBackground =  addBackgrounds("hDist3D_BBackground",                                    
					      hDist3D_BsJPsiPhiSignal   ,              
					      hDist3D_BsJPsiKKSignal    ,              
					      hDist3D_BdJPsiKstarSignal ,              
					      hDist3D_BsOther           ,              
					      hDist3D_BdOther           ,              
					      hDist3D_BOther            ,              
					      hDist3D_BsJpsiEta         ,              
					      hDist3D_BdJpsiK10         ,              
					      hDist3D_BdJpsiK0          ,              
					      hDist3D_BpJpsiKp          ,              
					      hDist3D_JpsiOther ,
					      hDist3D_ppmumux);             
  
  TH1F* hDistSign3D_BBackground =  addBackgrounds("hDistSign3D_BBackground",                                     
						  hDistSign3D_BsJPsiPhiSignal    ,         
						  hDistSign3D_BsJPsiKKSignal     ,         
						  hDistSign3D_BdJPsiKstarSignal  ,         
						  hDistSign3D_BsOther            ,         
						  hDistSign3D_BdOther            ,         
						  hDistSign3D_BOther             ,         
						  hDistSign3D_BsJpsiEta          ,         
						  hDistSign3D_BdJpsiK10          ,         
						  hDistSign3D_BdJpsiK0           ,         
						  hDistSign3D_BpJpsiKp           ,         
						  hDistSign3D_JpsiOther  ,
						  hDistSign3D_ppmumux);        
  
  TH1F* hDistSign1D_BBackground =  addBackgrounds("hDistSign1D_BBackground",                                     
						  hDistSign1D_BsJPsiPhiSignal   ,          
						  hDistSign1D_BsJPsiKKSignal    ,          
						  hDistSign1D_BdJPsiKstarSignal ,          
						  hDistSign1D_BsOther           ,          
						  hDistSign1D_BdOther           ,          
						  hDistSign1D_BOther            ,          
						  hDistSign1D_BsJpsiEta         ,          
						  hDistSign1D_BdJpsiK10         ,          
						  hDistSign1D_BdJpsiK0          ,          
						  hDistSign1D_BpJpsiKp          ,          
						  hDistSign1D_JpsiOther ,
						  hDistSign1D_ppmumux);         
          
  TH1F* hPointingAngle_BBackground =  addBackgrounds("hPointingAngle_BBackground",                             
						     hPointingAngle_BsJPsiPhiSignal   ,       
						     hPointingAngle_BsJPsiKKSignal    ,       
						     hPointingAngle_BdJPsiKstarSignal ,       
						     hPointingAngle_BsOther           ,       
						     hPointingAngle_BdOther           ,       
						     hPointingAngle_BOther            ,       
						     hPointingAngle_BsJpsiEta         ,       
						     hPointingAngle_BdJpsiK10         ,       
						     hPointingAngle_BdJpsiK0          ,       
						     hPointingAngle_BpJpsiKp          ,       
						     hPointingAngle_JpsiOther ,
						     hPointingAngle_ppmumux);      
   
  TH1F* hChi2_BBackground =  addBackgrounds("hChi2_BBackground",                                    
					    hChi2_BsJPsiPhiSignal     ,              
					    hChi2_BsJPsiKKSignal      ,              
					    hChi2_BdJPsiKstarSignal   ,              
					    hChi2_BsOther             ,              
					    hChi2_BdOther             ,              
					    hChi2_BOther              ,              
					    hChi2_BsJpsiEta           ,              
					    hChi2_BdJpsiK10           ,              
					    hChi2_BdJpsiK0            ,              
					    hChi2_BpJpsiKp            ,              
					    hChi2_JpsiOther ,
					    hChi2_ppmumux);             
  
  TH1F* hChi2Ndof_BBackground =  addBackgrounds("hChi2Ndof_BBackground",                               
						hChi2Ndof_BsJPsiPhiSignal   ,            
						hChi2Ndof_BsJPsiKKSignal    ,            
						hChi2Ndof_BdJPsiKstarSignal ,            
						hChi2Ndof_BsOther           ,            
						hChi2Ndof_BdOther           ,            
						hChi2Ndof_BOther            ,            
						hChi2Ndof_BsJpsiEta         ,            
						hChi2Ndof_BdJpsiK10         ,            
						hChi2Ndof_BdJpsiK0          ,            
						hChi2Ndof_BpJpsiKp          ,            
						hChi2Ndof_JpsiOther,
						hChi2Ndof_ppmumux);           
     
  TH1F* hBsVtxProb_BBackground =  addBackgrounds("hBsVtxProb_BBackground",                                  
						 hBsVtxProb_BsJPsiPhiSignal    ,          
						 hBsVtxProb_BsJPsiKKSignal     ,          
						 hBsVtxProb_BdJPsiKstarSignal  ,          
						 hBsVtxProb_BsOther            ,          
						 hBsVtxProb_BdOther            ,          
						 hBsVtxProb_BOther             ,          
						 hBsVtxProb_BsJpsiEta          ,          
						 hBsVtxProb_BdJpsiK10          ,          
						 hBsVtxProb_BdJpsiK0           ,          
						 hBsVtxProb_BpJpsiKp           ,          
						 hBsVtxProb_JpsiOther  ,
						 hBsVtxProb_ppmumux);         


  TH1F* hTime_BBackground =  addBackgrounds("hTime_BBackground",                               
						hTime_BsJPsiPhiSignal   ,            
						hTime_BsJPsiKKSignal    ,            
						hTime_BdJPsiKstarSignal ,            
						hTime_BsOther           ,            
						hTime_BdOther           ,            
						hTime_BOther            ,            
						hTime_BsJpsiEta         ,            
						hTime_BdJpsiK10         ,            
						hTime_BdJpsiK0          ,            
						hTime_BpJpsiKp          ,            
						hTime_JpsiOther,
						hTime_ppmumux);           
     
  TH1F* hBsMass_NoTimeCut_BBackground =  addBackgrounds("hBsMass_NoTimeCut_BBackground",                                  
						 hBsMass_NoTimeCut_BsJPsiPhiSignal    ,          
						 hBsMass_NoTimeCut_BsJPsiKKSignal     ,          
						 hBsMass_NoTimeCut_BdJPsiKstarSignal  ,          
						 hBsMass_NoTimeCut_BsOther            ,          
						 hBsMass_NoTimeCut_BdOther            ,          
						 hBsMass_NoTimeCut_BOther             ,          
						 hBsMass_NoTimeCut_BsJpsiEta          ,          
						 hBsMass_NoTimeCut_BdJpsiK10          ,          
						 hBsMass_NoTimeCut_BdJpsiK0           ,          
						 hBsMass_NoTimeCut_BpJpsiKp           ,          
						 hBsMass_NoTimeCut_JpsiOther  ,
						 hBsMass_NoTimeCut_ppmumux);         

 TH1F* hBdChi2_BBackground =  addBackgrounds("hBdChi2_BBackground",                               
						hBdChi2_BsJPsiPhiSignal   ,            
						hBdChi2_BsJPsiKKSignal    ,            
						hBdChi2_BdJPsiKstarSignal ,            
						hBdChi2_BsOther           ,            
						hBdChi2_BdOther           ,            
						hBdChi2_BOther            ,            
						hBdChi2_BsJpsiEta         ,            
						hBdChi2_BdJpsiK10         ,            
						hBdChi2_BdJpsiK0          ,            
						hBdChi2_BpJpsiKp          ,            
						hBdChi2_JpsiOther,
						hBdChi2_ppmumux);           

 TH1F* hBdChi2Ndof_BBackground =  addBackgrounds("hBdChi2Ndof_BBackground",                               
						hBdChi2Ndof_BsJPsiPhiSignal   ,            
						hBdChi2Ndof_BsJPsiKKSignal    ,            
						hBdChi2Ndof_BdJPsiKstarSignal ,            
						hBdChi2Ndof_BsOther           ,            
						hBdChi2Ndof_BdOther           ,            
						hBdChi2Ndof_BOther            ,            
						hBdChi2Ndof_BsJpsiEta         ,            
						hBdChi2Ndof_BdJpsiK10         ,            
						hBdChi2Ndof_BdJpsiK0          ,            
						hBdChi2Ndof_BpJpsiKp          ,            
						hBdChi2Ndof_JpsiOther,
						hBdChi2Ndof_ppmumux);           
 TH1F* hBdVtxProb_BBackground =  addBackgrounds("hBdVtxProb_BBackground",                               
						hBdVtxProb_BsJPsiPhiSignal   ,            
						hBdVtxProb_BsJPsiKKSignal    ,            
						hBdVtxProb_BdJPsiKstarSignal ,            
						hBdVtxProb_BsOther           ,            
						hBdVtxProb_BdOther           ,            
						hBdVtxProb_BOther            ,            
						hBdVtxProb_BsJpsiEta         ,            
						hBdVtxProb_BdJpsiK10         ,            
						hBdVtxProb_BdJpsiK0          ,            
						hBdVtxProb_BpJpsiKp          ,            
						hBdVtxProb_JpsiOther,
						hBdVtxProb_ppmumux);           
 TH1F* hBdK1Pt_BBackground =  addBackgrounds("hBdK1Pt_BBackground",                               
						hBdK1Pt_BsJPsiPhiSignal   ,            
						hBdK1Pt_BsJPsiKKSignal    ,            
						hBdK1Pt_BdJPsiKstarSignal ,            
						hBdK1Pt_BsOther           ,            
						hBdK1Pt_BdOther           ,            
						hBdK1Pt_BOther            ,            
						hBdK1Pt_BsJpsiEta         ,            
						hBdK1Pt_BdJpsiK10         ,            
						hBdK1Pt_BdJpsiK0          ,            
						hBdK1Pt_BpJpsiKp          ,            
						hBdK1Pt_JpsiOther,
						hBdK1Pt_ppmumux);           
 TH1F* hBdK2Pt_BBackground =  addBackgrounds("hBdK2Pt_BBackground",                               
						hBdK2Pt_BsJPsiPhiSignal   ,            
						hBdK2Pt_BsJPsiKKSignal    ,            
						hBdK2Pt_BdJPsiKstarSignal ,            
						hBdK2Pt_BsOther           ,            
						hBdK2Pt_BdOther           ,            
						hBdK2Pt_BOther            ,            
						hBdK2Pt_BsJpsiEta         ,            
						hBdK2Pt_BdJpsiK10         ,            
						hBdK2Pt_BdJpsiK0          ,            
						hBdK2Pt_BpJpsiKp          ,            
						hBdK2Pt_JpsiOther,
						hBdK2Pt_ppmumux);           
 TH1F* hKstarMass_BBackground =  addBackgrounds("hKstarMass_BBackground",                               
						hKstarMass_BsJPsiPhiSignal   ,            
						hKstarMass_BsJPsiKKSignal    ,            
						hKstarMass_BdJPsiKstarSignal ,            
						hKstarMass_BsOther           ,            
						hKstarMass_BdOther           ,            
						hKstarMass_BOther            ,            
						hKstarMass_BsJpsiEta         ,            
						hKstarMass_BdJpsiK10         ,            
						hKstarMass_BdJpsiK0          ,            
						hKstarMass_BpJpsiKp          ,            
						hKstarMass_JpsiOther,
						hKstarMass_ppmumux);           
 TH1F* hBdDistSign3D_BBackground =  addBackgrounds("hBdDistSign3D_BBackground",                               
						hBdDistSign3D_BsJPsiPhiSignal   ,            
						hBdDistSign3D_BsJPsiKKSignal    ,            
						hBdDistSign3D_BdJPsiKstarSignal ,            
						hBdDistSign3D_BsOther           ,            
						hBdDistSign3D_BdOther           ,            
						hBdDistSign3D_BOther            ,            
						hBdDistSign3D_BsJpsiEta         ,            
						hBdDistSign3D_BdJpsiK10         ,            
						hBdDistSign3D_BdJpsiK0          ,            
						hBdDistSign3D_BpJpsiKp          ,            
						hBdDistSign3D_JpsiOther,
						hBdDistSign3D_ppmumux);           
 TH1F* hBdDistSign1D_BBackground =  addBackgrounds("hBdDistSign1D_BBackground",                               
						hBdDistSign1D_BsJPsiPhiSignal   ,            
						hBdDistSign1D_BsJPsiKKSignal    ,            
						hBdDistSign1D_BdJPsiKstarSignal ,            
						hBdDistSign1D_BsOther           ,            
						hBdDistSign1D_BdOther           ,            
						hBdDistSign1D_BOther            ,            
						hBdDistSign1D_BsJpsiEta         ,            
						hBdDistSign1D_BdJpsiK10         ,            
						hBdDistSign1D_BdJpsiK0          ,            
						hBdDistSign1D_BpJpsiKp          ,            
						hBdDistSign1D_JpsiOther,
						hBdDistSign1D_ppmumux);           
 TH1F* hBdDist3D_BBackground =  addBackgrounds("hBdDist3D_BBackground",                               
						hBdDist3D_BsJPsiPhiSignal   ,            
						hBdDist3D_BsJPsiKKSignal    ,            
						hBdDist3D_BdJPsiKstarSignal ,            
						hBdDist3D_BsOther           ,            
						hBdDist3D_BdOther           ,            
						hBdDist3D_BOther            ,            
						hBdDist3D_BsJpsiEta         ,            
						hBdDist3D_BdJpsiK10         ,            
						hBdDist3D_BdJpsiK0          ,            
						hBdDist3D_BpJpsiKp          ,            
						hBdDist3D_JpsiOther,
						hBdDist3D_ppmumux);           
 TH1F* hBdTime_BBackground =  addBackgrounds("hBdTime_BBackground",                               
						hBdTime_BsJPsiPhiSignal   ,            
						hBdTime_BsJPsiKKSignal    ,            
						hBdTime_BdJPsiKstarSignal ,            
						hBdTime_BsOther           ,            
						hBdTime_BdOther           ,            
						hBdTime_BOther            ,            
						hBdTime_BsJpsiEta         ,            
						hBdTime_BdJpsiK10         ,            
						hBdTime_BdJpsiK0          ,            
						hBdTime_BpJpsiKp          ,            
						hBdTime_JpsiOther,
						hBdTime_ppmumux);           
 TH1F* hBdMass_NoTimeCut_BBackground =  addBackgrounds("hBdMass_NoTimeCut_BBackground",                               
						hBdMass_NoTimeCut_BsJPsiPhiSignal   ,            
						hBdMass_NoTimeCut_BsJPsiKKSignal    ,            
						hBdMass_NoTimeCut_BdJPsiKstarSignal ,            
						hBdMass_NoTimeCut_BsOther           ,            
						hBdMass_NoTimeCut_BdOther           ,            
						hBdMass_NoTimeCut_BOther            ,            
						hBdMass_NoTimeCut_BsJpsiEta         ,            
						hBdMass_NoTimeCut_BdJpsiK10         ,            
						hBdMass_NoTimeCut_BdJpsiK0          ,            
						hBdMass_NoTimeCut_BpJpsiKp          ,            
						hBdMass_NoTimeCut_JpsiOther,
						hBdMass_NoTimeCut_ppmumux);           
 TH1F* hBdMassFinal_BBackground =  addBackgrounds("hBdMassFinal_BBackground",                               
						hBdMassFinal_BsJPsiPhiSignal   ,            
						hBdMassFinal_BsJPsiKKSignal    ,            
						hBdMassFinal_BdJPsiKstarSignal ,            
						hBdMassFinal_BsOther           ,            
						hBdMassFinal_BdOther           ,            
						hBdMassFinal_BOther            ,            
						hBdMassFinal_BsJpsiEta         ,            
						hBdMassFinal_BdJpsiK10         ,            
						hBdMassFinal_BdJpsiK0          ,            
						hBdMassFinal_BpJpsiKp          ,            
						hBdMassFinal_JpsiOther,
						hBdMassFinal_ppmumux);           
 TH1F* hBdMassFinalAfterFit_BBackground =  addBackgrounds("hBdMassFinalAfterFit_BBackground",                               
						hBdMassFinalAfterFit_BsJPsiPhiSignal   ,            
						hBdMassFinalAfterFit_BsJPsiKKSignal    ,            
						hBdMassFinalAfterFit_BdJPsiKstarSignal ,            
						hBdMassFinalAfterFit_BsOther           ,            
						hBdMassFinalAfterFit_BdOther           ,            
						hBdMassFinalAfterFit_BOther            ,            
						hBdMassFinalAfterFit_BsJpsiEta         ,            
						hBdMassFinalAfterFit_BdJpsiK10         ,            
						hBdMassFinalAfterFit_BdJpsiK0          ,            
						hBdMassFinalAfterFit_BpJpsiKp          ,            
						hBdMassFinalAfterFit_JpsiOther,
						hBdMassFinalAfterFit_ppmumux);           


 TH1F* hBsCt_BBackground =  addBackgrounds("hBsCt_BBackground",
                                           hBsCt_BsJPsiPhiSignal    ,
                                           hBsCt_BsJPsiKKSignal     ,
                                           hBsCt_BdJPsiKstarSignal  ,
                                           hBsCt_BsOther            ,
                                           hBsCt_BdOther            ,
                                           hBsCt_BOther             ,
                                           hBsCt_BsJpsiEta          ,
                                           hBsCt_BdJpsiK10          ,
                                           hBsCt_BdJpsiK0           ,
                                           hBsCt_BpJpsiKp           ,
                                           hBsCt_JpsiOther ,
                                           hBsCt_ppmumux);

 TH1F* hBsCtErr_BBackground =  addBackgrounds("hBsCtErr_BBackground",
                                              hBsCtErr_BsJPsiPhiSignal    ,
                                              hBsCtErr_BsJPsiKKSignal     ,
                                              hBsCtErr_BdJPsiKstarSignal  ,
                                              hBsCtErr_BsOther            ,
                                              hBsCtErr_BdOther            ,
                                              hBsCtErr_BOther             ,
                                              hBsCtErr_BsJpsiEta          ,
                                              hBsCtErr_BdJpsiK10          ,
                                              hBsCtErr_BdJpsiK0           ,
                                              hBsCtErr_BpJpsiKp           ,
                                              hBsCtErr_JpsiOther ,
                                              hBsCtErr_ppmumux);


  //*********************

 


  drawStandardNormalized("K1Pt",
			 hK1Pt_BsJPsiPhiSignal,                 
			 hK1Pt_BdJPsiKstarSignal,               
			 hK1Pt_BBackground,                     
			 hK1Pt_JpsiOther,
			 hK1Pt_ppmumux);                     
      
  drawStandardNormalized("K2Pt",
			 hK2Pt_BsJPsiPhiSignal,                 
			 hK2Pt_BdJPsiKstarSignal,               
			 hK2Pt_BBackground,                     
			 hK2Pt_JpsiOther,
			 hK2Pt_ppmumux);                     
       
  drawStandardNormalized("Dist3D",
			 hDist3D_BsJPsiPhiSignal,               
			 hDist3D_BdJPsiKstarSignal,             
			 hDist3D_BBackground,                   
			 hDist3D_JpsiOther,
			 hDist3D_ppmumux);                   

  drawStandardNormalized("DistSign3D",
			 hDistSign3D_BsJPsiPhiSignal,           
			 hDistSign3D_BdJPsiKstarSignal,         
			 hDistSign3D_BBackground,               
			 hDistSign3D_JpsiOther,
			 hDistSign3D_ppmumux);               

  drawStandardNormalized("DistSign1D",
			 hDistSign1D_BsJPsiPhiSignal,           
			 hDistSign1D_BdJPsiKstarSignal,         
			 hDistSign1D_BBackground,               
			 hDistSign1D_JpsiOther,
			 hDistSign1D_ppmumux);               

  drawStandardNormalized("PointingAngle",
			 hPointingAngle_BsJPsiPhiSignal,        
			 hPointingAngle_BdJPsiKstarSignal,      
			 hPointingAngle_BBackground,            
			 hPointingAngle_JpsiOther,
			 hPointingAngle_ppmumux);            

  drawStandardNormalized("Chi2",
			 hChi2_BsJPsiPhiSignal,                 
			 hChi2_BdJPsiKstarSignal,               
			 hChi2_BBackground,                     
			 hChi2_JpsiOther,
			 hChi2_ppmumux);                     
 
  drawStandardNormalized("Chi2Ndof", 
			 hChi2Ndof_BsJPsiPhiSignal,             
			 hChi2Ndof_BdJPsiKstarSignal,           
			 hChi2Ndof_BBackground,                 
			 hChi2Ndof_JpsiOther,
			 hChi2Ndof_ppmumux);                 
  
  drawStandardNormalized("BsVtxProb",
			 hBsVtxProb_BsJPsiPhiSignal,            
			 hBsVtxProb_BdJPsiKstarSignal,          
			 hBsVtxProb_BBackground,                
			 hBsVtxProb_JpsiOther,
			 hBsVtxProb_ppmumux);                
  


  //***********************
  // plots scaled to correct lumi
 drawStandardLumiScaled("JPsiMass",
			 hJPsiMass_BsJPsiPhiSignal,
			 hJPsiMass_BdJPsiKstarSignal,
			 hJPsiMass_BBackground, 
			 hJPsiMass_JpsiOther,
			 hJPsiMass_ppmumux);      



  drawStandardLumiScaled("PhiMass",
			 hPhiMass_BsJPsiPhiSignal,              
			 hPhiMass_BdJPsiKstarSignal,            
			 hPhiMass_BBackground,                  
			 hPhiMass_JpsiOther,
			 hPhiMass_ppmumux);                  
    

  drawStandardLumiScaled("PhiMassFinal",
			 hPhiMassFinal_BsJPsiPhiSignal,         
			 hPhiMassFinal_BdJPsiKstarSignal,       
			 hPhiMassFinal_BBackground,             
			 hPhiMassFinal_JpsiOther,
			 hPhiMassFinal_ppmumux);             


  drawStandardLumiScaled("BsMassFinal",
			 hBsMassFinal_BsJPsiPhiSignal,          
			 hBsMassFinal_BdJPsiKstarSignal,        
			 hBsMassFinal_BBackground,              
			 hBsMassFinal_JpsiOther,
			 hBsMassFinal_ppmumux);              
 
  drawStandardLumiScaled("BsMassFinalAfterFit",
			 hBsMassFinalAfterFit_BsJPsiPhiSignal,  
			 hBsMassFinalAfterFit_BdJPsiKstarSignal,
			 hBsMassFinalAfterFit_BBackground,      
			 hBsMassFinalAfterFit_JpsiOther,
			 hBsMassFinalAfterFit_ppmumux);      

 drawStandardLumiScaled("Time",
			 hTime_BsJPsiPhiSignal,  
			 hTime_BdJPsiKstarSignal,
			 hTime_BBackground,      
			 hTime_JpsiOther,
			 hTime_ppmumux);      

 drawStandardLumiScaled("BsMass_NoTimeCut",
			 hBsMass_NoTimeCut_BsJPsiPhiSignal,  
			 hBsMass_NoTimeCut_BdJPsiKstarSignal,
			 hBsMass_NoTimeCut_BBackground,      
			 hBsMass_NoTimeCut_JpsiOther,
			 hBsMass_NoTimeCut_ppmumux);      

 drawStandardLumiScaled("BsCt",
			hBsCt_BsJPsiPhiSignal,
			hBsCt_BdJPsiKstarSignal,
			hBsCt_BBackground,
			hBsCt_JpsiOther,
			hBsCt_ppmumux);

 drawStandardLumiScaled("BsCtErr",
			hBsCtErr_BsJPsiPhiSignal,
			hBsCtErr_BdJPsiKstarSignal,
			hBsCtErr_BBackground,
			hBsCtErr_JpsiOther,
			hBsCtErr_ppmumux);

drawStandardLumiScaled("BdTime",
			 hBdTime_BsJPsiPhiSignal,  
			 hBdTime_BdJPsiKstarSignal,
			 hBdTime_BBackground,      
			 hBdTime_JpsiOther,
			 hBdTime_ppmumux);      


drawStandardLumiScaled("BdMass_NoTimeCut",
			 hBdMass_NoTimeCut_BsJPsiPhiSignal,  
			 hBdMass_NoTimeCut_BdJPsiKstarSignal,
			 hBdMass_NoTimeCut_BBackground,      
			 hBdMass_NoTimeCut_JpsiOther,
			 hBdMass_NoTimeCut_ppmumux);      

drawStandardLumiScaled("BdMassFinalAfterFit",
			 hBdMassFinalAfterFit_BsJPsiPhiSignal,  
			 hBdMassFinalAfterFit_BdJPsiKstarSignal,
			 hBdMassFinalAfterFit_BBackground,      
			 hBdMassFinalAfterFit_JpsiOther,
			 hBdMassFinalAfterFit_ppmumux);      

drawStandardLumiScaled("KstarMass",
			 hKstarMass_BsJPsiPhiSignal,  
			 hKstarMass_BdJPsiKstarSignal,
			 hKstarMass_BBackground,      
			 hKstarMass_JpsiOther,
			 hKstarMass_ppmumux);      

 TCanvas *cv3 = new TCanvas("Bs_mass_before_after_fit", "Bs_mass_before_after_fit", 750, 600 );
 cv3->cd();
 cv3->SetFillColor(0);


 hBsMassFinal_BsJPsiPhiSignal             ->SetLineColor(1  );
 hBsMassFinalAfterFit_BsJPsiPhiSignal     ->SetLineColor(2  );

 hBsMassFinal_BsJPsiPhiSignal              ->SetLineWidth(3);
 hBsMassFinalAfterFit_BsJPsiPhiSignal     ->SetLineWidth(3);

 TLegend * leg3 = new TLegend(0.6,0.75,0.93,0.91);
 leg3->AddEntry(  hBsMassFinal_BsJPsiPhiSignal   , "Bs mass w/o fit" );
 leg3->AddEntry(  hBsMassFinalAfterFit_BsJPsiPhiSignal    , "Bs mass with fit"	);
 

hBsMassFinalAfterFit_BsJPsiPhiSignal   ->Draw("HIST");
hBsMassFinal_BsJPsiPhiSignal    ->Draw("SAMES HIST");


 leg3->Draw("SAME");

 TCanvas *cv3b = new TCanvas("ResoLxy", "ResoLxy", 750,600);
 cv3b->cd();
 cv3b->SetFillColor(0);
 hResoLxy_BsJPsiPhiSignal ->Draw();

 TCanvas *cv3c = new TCanvas("ResoTime", "ResoTime", 750,600);
 cv3c->cd();
 cv3c->SetFillColor(0);
 hResoTime_BsJPsiPhiSignal->Draw();

//  //---------------
//  TCanvas *cv4 = new TCanvas("BsVtxProb", "BsVtxProb", 900, 600 );
//  cv4->cd();
//  cv4->SetFillColor(0);


// hBsVtxProb_BsJPsiPhiSignal    ->SetLineColor(1  );
// hBsVtxProb_BsJPsiKKSignal     ->SetLineColor(2  );
// hBsVtxProb_BdJPsiKstarSignal  ->SetLineColor(3  );
// hBsVtxProb_BsOther            ->SetLineColor(4  );
// hBsVtxProb_BdOther            ->SetLineColor(5  );
// hBsVtxProb_BOther             ->SetLineColor(6  );
// hBsVtxProb_BsJpsiEta          ->SetLineColor(7  );
// hBsVtxProb_BdJpsiK10          ->SetLineColor(8  );
// hBsVtxProb_BdJpsiK0           ->SetLineColor(9  );
// hBsVtxProb_BpJpsiKp           ->SetLineColor(12 );
// hBsVtxProb_JpsiOther          ->SetLineColor(11 );

// hBsVtxProb_BsJPsiPhiSignal    ->SetLineWidth(3);
// hBsVtxProb_BsJPsiKKSignal     ->SetLineWidth(3);
// hBsVtxProb_BdJPsiKstarSignal  ->SetLineWidth(3);
// hBsVtxProb_BsOther            ->SetLineWidth(3);
// hBsVtxProb_BdOther            ->SetLineWidth(3);
// hBsVtxProb_BOther             ->SetLineWidth(3);
// hBsVtxProb_BsJpsiEta          ->SetLineWidth(3);
// hBsVtxProb_BdJpsiK10          ->SetLineWidth(3);
// hBsVtxProb_BdJpsiK0           ->SetLineWidth(3);
// hBsVtxProb_BpJpsiKp           ->SetLineWidth(3);
// hBsVtxProb_JpsiOther          ->SetLineWidth(3);

//  TLegend * leg4 = new TLegend(0.6,0.75,0.93,0.91);
//  leg4->AddEntry(  hBsVtxProb_BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
//  leg4->AddEntry(  hBsVtxProb_BsJPsiKKSignal    , "Bs -> JPsi KK"	);
//  leg4->AddEntry(  hBsVtxProb_BdJPsiKstarSignal , "Bd -> JPsi K*"	);
//  leg4->AddEntry(  hBsVtxProb_BsJpsiEta         , "Bs -> JPsi eta"	);
//  leg4->AddEntry(  hBsVtxProb_BdJpsiK10         , "Bd -> JPsi K10"	);
//  leg4->AddEntry(  hBsVtxProb_BdJpsiK0          , "Bd -> JPsi K0"	);
//  leg4->AddEntry(  hBsVtxProb_BpJpsiKp          , "B+ -> JPsi K+"	);
//  leg4->AddEntry(  hBsVtxProb_BsOther           , "other Bs"	);
//  leg4->AddEntry(  hBsVtxProb_BdOther           , "other Bd"	);
//  leg4->AddEntry(  hBsVtxProb_BOther            , "other"		);
//  leg4->AddEntry(  hBsVtxProb_JpsiOther         , "prompt JPsi"    );


// hBsVtxProb_BsJPsiPhiSignal   ->DrawNormalized();
// hBsVtxProb_BsJPsiKKSignal    ->DrawNormalized("SAME");
// hBsVtxProb_BdJPsiKstarSignal ->DrawNormalized("SAME");
// hBsVtxProb_BsOther           ->DrawNormalized("SAME");
// hBsVtxProb_BdOther           ->DrawNormalized("SAME");
// hBsVtxProb_BOther            ->DrawNormalized("SAME");
// hBsVtxProb_BsJpsiEta         ->DrawNormalized("SAME");
// hBsVtxProb_BdJpsiK10         ->DrawNormalized("SAME");
// hBsVtxProb_BdJpsiK0          ->DrawNormalized("SAME");
// hBsVtxProb_BpJpsiKp          ->DrawNormalized("SAME");
// hBsVtxProb_JpsiOther         ->DrawNormalized("SAME");


//  leg4->Draw("SAME");

//  //----------------------
//  TCanvas *cv31 = new TCanvas("PhiMass bg merged", "PhiMass bg merged", 900, 600 );
//  cv31->cd();
//  cv31->SetFillColor(0);



// hPhiMass_BsJPsiPhiSignal    ->SetLineColor(1  );
// hPhiMass_BdJPsiKstarSignal  ->SetLineColor(2  );
// hPhiMass_BBackground        ->SetLineColor(3  );
// hPhiMass_JpsiOther          ->SetLineColor(4 );

// hPhiMass_BsJPsiPhiSignal    ->SetLineWidth(3  );
// hPhiMass_BdJPsiKstarSignal  ->SetLineWidth(3  );
// hPhiMass_BBackground        ->SetLineWidth(3  );
// hPhiMass_JpsiOther          ->SetLineWidth(3 );


//  TLegend * leg31 = new TLegend(0.6,0.75,0.93,0.91);
//  leg31->AddEntry(  hPhiMass_BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
//  leg31->AddEntry(  hPhiMass_BdJPsiKstarSignal , "Bd -> JPsi K*"	);
//  leg31->AddEntry(  hPhiMass_BBackground       , "otherB -> JPsi X");
//  leg31->AddEntry(  hPhiMass_JpsiOther         , "prompt JPsi"    );


//  hPhiMass_BsJPsiPhiSignal   ->DrawNormalized();
//  hPhiMass_BdJPsiKstarSignal ->DrawNormalized("SAME");
//  hPhiMass_BBackground       ->DrawNormalized("SAME");
//  hPhiMass_JpsiOther         ->DrawNormalized("SAME");
 

//  leg31->Draw("SAME");



  }



//------------------------------------
void ScaleHistograms(TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, TH1F* BdJpsiK0, TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F *ppmumux){


BsJPsiPhiSignal   ->Sumw2();
BsJPsiKKSignal    ->Sumw2();
BdJPsiKstarSignal ->Sumw2();
BsOther           ->Sumw2();
BdOther           ->Sumw2();
BOther            ->Sumw2();
BsJpsiEta         ->Sumw2();
BdJpsiK10         ->Sumw2();
BdJpsiK0          ->Sumw2();
BpJpsiKp          ->Sumw2();
JpsiOther         ->Sumw2();
 ppmumux          ->Sumw2();


// BsJPsiPhiSignal      ->Scale(2.*10./ 48.); 
// BsJPsiKKSignal       ->Scale(2.*10./ 38. ); 
// BdJPsiKstarSignal    ->Scale(2.*10./ 99.);  
// BsOther              ->Scale(2.*10./ 48. ); 
// BdOther              ->Scale(2.*10./ 83. ); 
// BOther               ->Scale(2.*10./ 69.);  
// BsJpsiEta            ->Scale(2.*10./ 16.);  
// BdJpsiK10            ->Scale(2.*10./ 95. ); 
// BdJpsiK0             ->Scale(2.*10./ 33. ); 
// BpJpsiKp             ->Scale(2.*10./ 17.);  
// JpsiOther            ->Scale(10./ 9.);   
//  ppmumux              ->Scale(10./0.9);

BsJPsiPhiSignal      ->Scale(10./ 48.); 
BsJPsiKKSignal       ->Scale(10./ 38. ); 
BdJPsiKstarSignal    ->Scale(10./ 83.);  
BsOther              ->Scale(10./ 48. ); 
BdOther              ->Scale(10./ 83. ); 
BOther               ->Scale(10./ 69.);  
BsJpsiEta            ->Scale(10./ 16.);  
BdJpsiK10            ->Scale(10./ 93. ); 
BdJpsiK0             ->Scale(10./ 52. ); 
BpJpsiKp             ->Scale(10./ 69.);  
JpsiOther            ->Scale(10./ 9.);   
 ppmumux              ->Scale(10./0.9);

//  // old scaling
// BsJPsiPhiSignal      ->Scale(100./ 63. ); 
// BsJPsiKKSignal       ->Scale(100./ 63. ); 
// BdJPsiKstarSignal    ->Scale(100./ 80.);  
// BsOther              ->Scale(100./ 63. ); 
// BdOther              ->Scale(100./ 80. ); 
// BOther               ->Scale(100./ 80.);  
// BsJpsiEta            ->Scale(100./ 63.);  
// BdJpsiK10            ->Scale(100./ 80. ); 
// BdJpsiK0             ->Scale(100./ 80. ); 
// BpJpsiKp             ->Scale(100./ 80.);  
// JpsiOther            ->Scale(100./ 9.);   
// ppmumux              ->Scale(100./1.);
}


//------------------------------------
TH1F* addBackgrounds(TString name, TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, TH1F* BdJpsiK0, TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F* ppmumux){

 TH1F* BBackground = (TH1F*) BsJPsiKKSignal->Clone(name);
 BBackground->Sumw2();
 BBackground->Add( BsOther );
 BBackground->Add( BdOther );
 BBackground->Add( BOther );
 BBackground->Add( BsJpsiEta );
 BBackground->Add( BdJpsiK10 );
 BBackground->Add( BdJpsiK0 );
 BBackground->Add( BpJpsiKp );

 return BBackground;
}




//--------------------------------------
void drawStandardNormalized(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux){


 TCanvas *cv = new TCanvas(name, name, 750, 600 );
 cv->cd();
 cv->SetFillColor(0);


 BsJPsiPhiSignal    ->SetLineColor(1 );
 BdJPsiKstarSignal  ->SetLineColor(2 );
 BBackground        ->SetLineColor(3);
 JpsiOther          ->SetLineColor(4 );
 ppmumux            ->SetLineColor(5 );

 BsJPsiPhiSignal    ->SetLineWidth(3 );
 BdJPsiKstarSignal  ->SetLineWidth(3 );
 BBackground        ->SetLineWidth(3);
 JpsiOther          ->SetLineWidth(3 );
 ppmumux            ->SetLineWidth(3 );


 TLegend * leg3 = new TLegend(0.6,0.75,0.93,0.91);
 leg3->AddEntry(  BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
 leg3->AddEntry(  BdJPsiKstarSignal , "Bd -> JPsi K*"	);
 leg3->AddEntry(  BBackground       , "other B->JPsi X"	);
 leg3->AddEntry(  JpsiOther         , "prompt JPsi"    );
 if(usePpMuMux) leg3->AddEntry(  ppmumux         , "pp-> mu mu X"    );


 JpsiOther         ->DrawNormalized("HIST");
 BsJPsiPhiSignal   ->DrawNormalized("SAME HIST");
 BdJPsiKstarSignal ->DrawNormalized("SAME HIST");
 BBackground       ->DrawNormalized("SAME HIST");
 if(usePpMuMux) ppmumux           ->DrawNormalized("SAME");
 

 leg3->Draw("SAME");

 }



void drawStandardLumiScaled(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux){
 TCanvas *cv = new TCanvas(name, name, 750, 600 );
 cv->cd();
 cv->SetFillColor(0);


 BsJPsiPhiSignal    ->SetLineColor(2 );
 BdJPsiKstarSignal  ->SetLineColor(3 );
 BBackground        ->SetLineColor(4);
 JpsiOther          ->SetLineColor(6 );
 ppmumux            ->SetLineColor(7);
 
 BsJPsiPhiSignal    ->SetLineWidth(3 );
 BdJPsiKstarSignal  ->SetLineWidth(3 );
 BBackground        ->SetLineWidth(3);
 JpsiOther          ->SetLineWidth(3 );
 ppmumux            ->SetLineWidth(3);


 // sum of everything to look like data:
 TString sumName = name;
 sumName += "sum";
 TH1F * hsum =  (TH1F*) BsJPsiPhiSignal->Clone(sumName);
 hsum->Add(BdJPsiKstarSignal);
 hsum->Add(BBackground);
 hsum->Add(JpsiOther);
 if(usePpMuMux) hsum->Add(ppmumux);

 // loop over bins of hsum histogram to set the errors correctly
 for(int i = 1; i<=hsum->GetNbinsX(); i++){

   hsum->SetBinError(i, sqrt(hsum->GetBinContent(i) ) );
   //  std::cout<< name << " bin content " << i << "  == " << hsum->GetBinContent(i) << " err = " << hsum->GetBinError(i)<< std::endl;
 }
   
 hsum->SetLineColor(1);
 hsum->SetLineWidth(1);
 hsum->SetMarkerColor(1);
 hsum->SetMarkerStyle(21);
 hsum->SetMarkerSize(0.6);

 TLegend * leg3 = new TLegend(0.6,0.75,0.93,0.91);
 leg3->AddEntry(  BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
 leg3->AddEntry(  BdJPsiKstarSignal , "Bd -> JPsi K*"	);
 leg3->AddEntry(  BBackground       , "other B->JPsi X"	);
 leg3->AddEntry(  JpsiOther         , "prompt JPsi"    );
 if(usePpMuMux) leg3->AddEntry(  ppmumux         , "pp-> mu mu X"    );
 leg3->AddEntry( hsum             , "sum sig + bg" );

 hsum->Draw();
 BsJPsiPhiSignal   ->Draw("SAME HIST");
 BdJPsiKstarSignal ->Draw("SAME HIST");
 BBackground       ->Draw("SAME HIST");
 JpsiOther         ->Draw("SAME HIST"); 
 if(usePpMuMux) ppmumux           ->Draw("SAME HIST");
 
 leg3->Draw("SAME");

}
