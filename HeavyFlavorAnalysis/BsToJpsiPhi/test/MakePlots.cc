#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <iostream>

// ***************main function*****************************
void MakePlots() {


  TFile * BtoJPsiMuMuHistoFile = new TFile ( "BtoJPsiMuMuHistoFile.root" , "READ");
  TFile * JPsiMuMuHistoFile  = new TFile ( "JPsiMuMuHistoFile.root", "READ");
//   TFile * ppmuXHistoFile   = new TFile ("ppmuXHistoFile.root", "READ");
//   TFile * ppmumuXHistoFile = new TFile ("ppmumuXHistoFile.root", "READ");

  TH1F * hPhiMass_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsJpsiSignal");
  TH1F * hPhiMass_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsOther");
  TH1F * hPhiMass_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_Other");
  TH1F * hPhiMass_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hPhiMass_Other");
//   TH1F * hPhiMass_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hPhiMass_Other");
//   TH1F * hPhiMass_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hPhiMass_Other");

  TH1F * hPhiMassFinal_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsJpsiSignal");
  TH1F * hPhiMassFinal_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsOther");
  TH1F * hPhiMassFinal_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_Other");
  TH1F * hPhiMassFinal_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hPhiMassFinal_Other");
  //  TH1F * hPhiMassFinal_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hPhiMassFinal_Other");
  // TH1F * hPhiMassFinal_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hPhiMassFinal_Other");



  TH1F * hPointingAngle_BsJpsiSignal = (TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsJpsiSignal");
  TH1F * hPointingAngle_BsOther = (TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsOther");
  TH1F * hPointingAngle_BOther = (TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_Other");
  TH1F * hPointingAngle_JPsiMuMu = (TH1F*) JPsiMuMuHistoFile->Get("hPointingAngle_Other");

  TH1F * hDistSign3D_BsJpsiSignal = (TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsJpsiSignal");
  TH1F * hDistSign3D_BsOther = (TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsOther");
  TH1F * hDistSign3D_BOther = (TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_Other");
  TH1F * hDistSign3D_JPsiMuMu = (TH1F*) JPsiMuMuHistoFile->Get("hDistSign3D_Other");

  TH1F * hDistSign1D_BsJpsiSignal = (TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsJpsiSignal");
  TH1F * hDistSign1D_BsOther = (TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsOther");
  TH1F * hDistSign1D_BOther = (TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_Other");
  TH1F * hDistSign1D_JPsiMuMu = (TH1F*) JPsiMuMuHistoFile->Get("hDistSign1D_Other");


  TH1F * hJPsiMass_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsJpsiSignal");
  TH1F * hJPsiMass_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsOther");
  TH1F * hJPsiMass_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_Other");
  TH1F * hJPsiMass_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hJPsiMass_Other");
//   TH1F * hJPsiMass_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hJPsiMass_Other");
//   TH1F * hJPsiMass_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hJPsiMass_Other");

  TH1F * hBsVtxProb_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsJpsiSignal");
  TH1F * hBsVtxProb_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsOther");
  TH1F * hBsVtxProb_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_Other");
  TH1F * hBsVtxProb_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hBsVtxProb_Other");
//   TH1F * hBsVtxProb_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hBsVtxProb_Other");
//   TH1F * hBsVtxProb_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hBsVtxProb_Other");

  TH1F * hBsMassFinal_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsJpsiSignal");
  TH1F * hBsMassFinal_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsOther");
  TH1F * hBsMassFinal_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_Other");
  TH1F * hBsMassFinal_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hBsMassFinal_Other");
//   TH1F * hBsMassFinal_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hBsMassFinal_Other");
//   TH1F * hBsMassFinal_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hBsMassFinal_Other");

  TH1F * hBsMassFinalAfterFit_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJpsiSignal");
  TH1F * hBsMassFinalAfterFit_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsOther");
  TH1F * hBsMassFinalAfterFit_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_Other");
  TH1F * hBsMassFinalAfterFit_BsJpsiKK  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJpsiKK");
  TH1F * hBsMassFinalAfterFit_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_Other");
//   TH1F * hBsMassFinalAfterFit_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hBsMassFinalAfterFit_Other");
//   TH1F * hBsMassFinalAfterFit_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hBsMassFinalAfterFit_Other");


  TH1F * hK1Pt_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsJpsiSignal");
  TH1F * hK1Pt_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsOther");
  TH1F * hK1Pt_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_Other");
  TH1F * hK1Pt_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hK1Pt_Other");
//   TH1F * hK1Pt_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hK1Pt_Other");
//   TH1F * hK1Pt_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hK1Pt_Other");

  TH1F * hK2Pt_BsJpsiSignal =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsJpsiSignal");
  TH1F * hK2Pt_BsOther =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsOther");
  TH1F * hK2Pt_BOther  =     (TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_Other");
  TH1F * hK2Pt_JPsiMuMU=     (TH1F*) JPsiMuMuHistoFile->Get("hK2Pt_Other");
//   TH1F * hK2Pt_ppmuX =       (TH1F*) ppmuXHistoFile->Get("hK2Pt_Other");
//   TH1F * hK2Pt_ppmumuX =       (TH1F*) ppmumuXHistoFile->Get("hK2Pt_Other");

  


  hBsMassFinal_BsJpsiSignal ->Scale(100./ 63.);
  hBsMassFinal_BsOther     ->Scale(100. / 63. );
  hBsMassFinal_BOther     ->Scale( 100. / 80.);
  hBsMassFinal_JPsiMuMU   ->Scale(100. / 9.);
 //  hBsMassFinal_ppmuX     ->Scale( 100. / 0.09);     
//   hBsMassFinal_ppmumuX   ->Scale(100. / 0.9);
  
  hBsMassFinalAfterFit_BsJpsiSignal ->Scale(100./ 63.);
  hBsMassFinalAfterFit_BsOther     ->Scale(100. / 63. );
  hBsMassFinalAfterFit_BsJpsiKK     ->Scale(100. / 63. );
  hBsMassFinalAfterFit_BOther     ->Scale( 100. / 80.);
  hBsMassFinalAfterFit_JPsiMuMU   ->Scale(100. / 9.);
//   hBsMassFinalAfterFit_ppmuX     ->Scale( 100. / 0.09);     
//   hBsMassFinalAfterFit_ppmumuX   ->Scale(100. / 0.9);
  
  hPhiMassFinal_BsJpsiSignal ->Scale(100./ 63.);
  hPhiMassFinal_BsOther     ->Scale(100. / 63. );
  hPhiMassFinal_BOther     ->Scale( 100. / 80.);
  hPhiMassFinal_JPsiMuMU   ->Scale(100. / 9.);
  //  hPhiMassFinal_ppmuX     ->Scale( 100. / 0.09);     
  // hBsMassFinal_ppmumuX   ->Scale(100. / 0.9);



 TCanvas *cv3 = new TCanvas("PhiMass", "PhiMass", 900, 600);
  cv3 ->cd();
  cv3->SetFillColor(0);
  
  hPhiMass_BOther      ->SetLineColor(1);  
  hPhiMass_BsJpsiSignal->SetLineColor(2);  
  hPhiMass_BsOther  ->SetLineColor(3);     
  hPhiMass_JPsiMuMU ->SetLineColor(4);  
  // hPhiMass_ppmuX    ->SetLineColor(5);  
  //  hPhiMass_ppmumuX  ->SetLineColor(6);  

  hPhiMass_BOther      ->SetLineWidth(3);  
  hPhiMass_BsJpsiSignal->SetLineWidth(3);  
  hPhiMass_BsOther  ->SetLineWidth(3);     
  hPhiMass_JPsiMuMU ->SetLineWidth(3);  
  //  hPhiMass_ppmuX    ->SetLineWidth(3);  
  //  hPhiMass_ppmumuX  ->SetLineWidth(3);  



 TLegend * leg3 = new TLegend(0.6,0.75,0.93,0.91);
 leg3->AddEntry( hPhiMass_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg3->AddEntry( hPhiMass_BOther, "B->JPsi X" );
 leg3->AddEntry( hPhiMass_BsOther, "Bs -> JPsi X" );
 leg3->AddEntry( hPhiMass_JPsiMuMU, "prompt JPsi" );
 // leg3->AddEntry( hPhiMass_ppmuX, "pp mu X");
 // leg3->AddEntry( hPhiMass_ppmumuX, "pp mumu X");

  hPhiMass_BsJpsiSignal->DrawNormalized();  
  hPhiMass_BOther   ->DrawNormalized("SAME");  
  hPhiMass_BsOther  ->DrawNormalized("SAME");     
  hPhiMass_JPsiMuMU ->DrawNormalized("SAME");  
  //  hPhiMass_ppmuX    ->DrawNormalized("SAME");  
  //  hPhiMass_ppmumuX  ->DrawNormalized("SAME");  


 leg3->Draw("SAME");




 TCanvas *cv4 = new TCanvas("BsVtxProb", "BsVtxProb", 900, 600);
  cv4 ->cd();
  cv4->SetFillColor(0);
  
  hBsVtxProb_BOther   ->SetLineColor(1);  
  hBsVtxProb_BsJpsiSignal->SetLineColor(2);  
  hBsVtxProb_BsOther  ->SetLineColor(3);     
  hBsVtxProb_JPsiMuMU ->SetLineColor(4);  
 //  hBsVtxProb_ppmuX    ->SetLineColor(5);  
//   hBsVtxProb_ppmumuX  ->SetLineColor(6);  

  hBsVtxProb_BOther   ->SetLineWidth(3);  
  hBsVtxProb_BsJpsiSignal->SetLineWidth(3);  
  hBsVtxProb_BsOther  ->SetLineWidth(3);     
  hBsVtxProb_JPsiMuMU ->SetLineWidth(3);  
 //  hBsVtxProb_ppmuX    ->SetLineWidth(3);  
//   hBsVtxProb_ppmumuX  ->SetLineWidth(3);  


//  hBsVtxProb_ppmuX    ->DrawNormalized();  
  hBsVtxProb_BOther   ->DrawNormalized();   

  //  hBsVtxProb_ppmumuX  ->DrawNormalized("SAME");  

  hBsVtxProb_BsOther  ->DrawNormalized("SAME");     
  hBsVtxProb_JPsiMuMU ->DrawNormalized("SAME");  
 hBsVtxProb_BsJpsiSignal->DrawNormalized("SAME");  



 TLegend * leg4 = new TLegend(0.6,0.75,0.93,0.91);
 leg4->AddEntry( hBsVtxProb_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg4->AddEntry( hBsVtxProb_BOther, "B->JPsi X" );
 leg4->AddEntry( hBsVtxProb_BsOther, "Bs -> JPsi X" );
 leg4->AddEntry( hBsVtxProb_JPsiMuMU, "prompt JPsi" );
//  leg4->AddEntry( hBsVtxProb_ppmuX, "pp muX");
//  leg4->AddEntry( hBsVtxProb_ppmumuX, "pp mumuX");
 leg4->Draw("SAME");


 TCanvas *cv5 = new TCanvas("BsMassFinal", "BsMassFinal", 900, 600);
  cv5 ->cd();
  cv5->SetFillColor(0);
  
  hBsMassFinal_BOther   ->SetLineColor(1);  
  hBsMassFinal_BsJpsiSignal->SetLineColor(2);  
  hBsMassFinal_BsOther  ->SetLineColor(3);     
  hBsMassFinal_JPsiMuMU ->SetLineColor(4);  
//   hBsMassFinal_ppmuX    ->SetLineColor(5);  
//   hBsMassFinal_ppmumuX  ->SetLineColor(6);  

  hBsMassFinal_BOther   ->SetLineWidth(3);  
  hBsMassFinal_BsJpsiSignal->SetLineWidth(3);  
  hBsMassFinal_BsOther  ->SetLineWidth(3);     
  hBsMassFinal_JPsiMuMU ->SetLineWidth(3);  
//   hBsMassFinal_ppmuX    ->SetLineWidth(3);  
//   hBsMassFinal_ppmumuX  ->SetLineWidth(3);  



  //  hBsMassFinal_ppmumuX  ->Draw();  
  //   hBsMassFinal_ppmuX    ->Draw("SAME");  

 hBsMassFinal_JPsiMuMU ->Draw();  

 hBsMassFinal_BsOther  ->Draw("SAME");    
 hBsMassFinal_BsJpsiSignal->Draw("SAME");  
  hBsMassFinal_BOther   ->Draw("SAME");   
  
 
 



 TLegend * leg5 = new TLegend(0.6,0.75,0.93,0.91);
 leg5->AddEntry( hBsMassFinal_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg5->AddEntry( hBsMassFinal_BOther, "B->JPsi X" );
 leg5->AddEntry( hBsMassFinal_BsOther, "Bs -> JPsi X" );
 leg5->AddEntry( hBsMassFinal_JPsiMuMU, "prompt JPsi" );
 // leg5->AddEntry( hBsMassFinal_ppmuX, "pp muX");
 // leg5->AddEntry( hBsMassFinal_ppmumuX, "pp mumuX");
 leg5->Draw("SAME");



 TCanvas *cv6 = new TCanvas("K1Pt", "K1Pt", 900, 600);
  cv6 ->cd();
  cv6->SetFillColor(0);
  
  hK1Pt_BOther   ->SetLineColor(1);  
  hK1Pt_BsJpsiSignal->SetLineColor(2);  
  hK1Pt_BsOther  ->SetLineColor(3);     
  hK1Pt_JPsiMuMU ->SetLineColor(4);  
//   hK1Pt_ppmuX    ->SetLineColor(5);  
//   hK1Pt_ppmumuX  ->SetLineColor(6);  

  hK1Pt_BOther   ->SetLineWidth(3);  
  hK1Pt_BsJpsiSignal->SetLineWidth(3);  
  hK1Pt_BsOther  ->SetLineWidth(3);     
  hK1Pt_JPsiMuMU ->SetLineWidth(3);  
//   hK1Pt_ppmuX    ->SetLineWidth(3);  
//   hK1Pt_ppmumuX  ->SetLineWidth(3);  


  // hK1Pt_ppmuX    ->DrawNormalized();  
  hK1Pt_BOther   ->DrawNormalized();   

  //  hK1Pt_ppmumuX  ->DrawNormalized("SAME");  

  hK1Pt_BsOther  ->DrawNormalized("SAME");     
  hK1Pt_JPsiMuMU ->DrawNormalized("SAME");  
 hK1Pt_BsJpsiSignal->DrawNormalized("SAME");  



 TLegend * leg6 = new TLegend(0.6,0.75,0.93,0.91);
 leg6->AddEntry( hK1Pt_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg6->AddEntry( hK1Pt_BOther, "B->JPsi X" );
 leg6->AddEntry( hK1Pt_BsOther, "Bs -> JPsi X" );
 leg6->AddEntry( hK1Pt_JPsiMuMU, "prompt JPsi" );
 // leg6->AddEntry( hK1Pt_ppmuX, "pp muX");
 // leg6->AddEntry( hK1Pt_ppmumuX, "pp mumuX");
 leg6->Draw("SAME");




 TCanvas *cv7 = new TCanvas("K2Pt", "K2Pt", 900, 600);
  cv7 ->cd();
  cv7->SetFillColor(0);
  
  hK2Pt_BOther   ->SetLineColor(1);  
  hK2Pt_BsJpsiSignal->SetLineColor(2);  
  hK2Pt_BsOther  ->SetLineColor(3);     
  hK2Pt_JPsiMuMU ->SetLineColor(4);  
//   hK2Pt_ppmuX    ->SetLineColor(5);  
//   hK2Pt_ppmumuX  ->SetLineColor(6);  

  hK2Pt_BOther   ->SetLineWidth(3);  
  hK2Pt_BsJpsiSignal->SetLineWidth(3);  
  hK2Pt_BsOther  ->SetLineWidth(3);     
  hK2Pt_JPsiMuMU ->SetLineWidth(3);  
//   hK2Pt_ppmuX    ->SetLineWidth(3);  
//   hK2Pt_ppmumuX  ->SetLineWidth(3);  


//  hK2Pt_ppmuX    ->DrawNormalized();  
  hK2Pt_BOther   ->DrawNormalized();   

  // hK2Pt_ppmumuX  ->DrawNormalized("SAME");  

  hK2Pt_BsOther  ->DrawNormalized("SAME");     
  hK2Pt_JPsiMuMU ->DrawNormalized("SAME");  
 hK2Pt_BsJpsiSignal->DrawNormalized("SAME");  



 TLegend * leg7 = new TLegend(0.6,0.75,0.93,0.91);
 leg7->AddEntry( hK2Pt_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg7->AddEntry( hK2Pt_BOther, "B->JPsi X" );
 leg7->AddEntry( hK2Pt_BsOther, "Bs -> JPsi X" );
 leg7->AddEntry( hK2Pt_JPsiMuMU, "prompt JPsi" );
 // leg7->AddEntry( hK2Pt_ppmuX, "pp muX");
 // leg7->AddEntry( hK2Pt_ppmumuX, "pp mumuX");
 leg7->Draw("SAME");



 TCanvas *cv8 = new TCanvas("BsMassFinalAfterFit", "BsMassFinalAfterFit", 900, 600);
  cv8 ->cd();
  cv8->SetFillColor(0);
  
  hBsMassFinalAfterFit_BOther   ->SetLineColor(1);  
  hBsMassFinalAfterFit_BsJpsiSignal->SetLineColor(2);  
  hBsMassFinalAfterFit_BsOther  ->SetLineColor(3);     
  hBsMassFinalAfterFit_JPsiMuMU ->SetLineColor(4);  
  hBsMassFinalAfterFit_BsJpsiKK->SetLineColor(5);
//   hBsMassFinalAfterFit_ppmuX    ->SetLineColor(5);  
//   hBsMassFinalAfterFit_ppmumuX  ->SetLineColor(6);  

  hBsMassFinalAfterFit_BOther   ->SetLineWidth(3);  
  hBsMassFinalAfterFit_BsJpsiSignal->SetLineWidth(3);  
  hBsMassFinalAfterFit_BsOther  ->SetLineWidth(3);     
  hBsMassFinalAfterFit_JPsiMuMU ->SetLineWidth(3);  
  hBsMassFinalAfterFit_BsJpsiKK->SetLineWidth(3);
//   hBsMassFinalAfterFit_ppmuX    ->SetLineWidth(3);  
//   hBsMassFinalAfterFit_ppmumuX  ->SetLineWidth(3);  



  //  hBsMassFinalAfterFit_ppmumuX  ->Draw();  
  //   hBsMassFinalAfterFit_ppmuX    ->Draw("SAME");  


  hBsMassFinalAfterFit_BsJpsiSignal->Draw();

 hBsMassFinalAfterFit_JPsiMuMU ->Draw("SAME");  

 hBsMassFinalAfterFit_BsOther  ->Draw("SAME");    

  hBsMassFinalAfterFit_BOther   ->Draw("SAME");   
  hBsMassFinalAfterFit_BsJpsiKK->Draw("SAME");
 
 



 TLegend * leg8 = new TLegend(0.6,0.75,0.93,0.91);
 leg8->AddEntry( hBsMassFinalAfterFit_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg8->AddEntry( hBsMassFinalAfterFit_BOther, "B->JPsi X" );
 leg8->AddEntry( hBsMassFinalAfterFit_BsOther, "Bs -> JPsi X" );
 leg8->AddEntry( hBsMassFinalAfterFit_JPsiMuMU, "prompt JPsi" );
 leg8->AddEntry( hBsMassFinalAfterFit_BsJpsiKK, "Bs-> JPsi KK");
 // leg5->AddEntry( hBsMassFinalAfterFit_ppmuX, "pp muX");
 // leg5->AddEntry( hBsMassFinalAfterFit_ppmumuX, "pp mumuX");
 leg8->Draw("SAME");



  TCanvas *cv9 = new TCanvas("PointingAngle", "PointingAngle", 900, 600);
  cv9 ->cd();
  cv9->SetFillColor(0);
  
  hPointingAngle_BOther      ->SetLineColor(1);  
  hPointingAngle_BsJpsiSignal->SetLineColor(2);  
  hPointingAngle_BsOther  ->SetLineColor(3);     
  hPointingAngle_JPsiMuMu ->SetLineColor(4);  
  // hPointingAngle_ppmuX    ->SetLineColor(5);  
  //  hPointingAngle_ppmumuX  ->SetLineColor(6);  

  hPointingAngle_BOther      ->SetLineWidth(3);  
  hPointingAngle_BsJpsiSignal->SetLineWidth(3);  
  hPointingAngle_BsOther  ->SetLineWidth(3);     
  hPointingAngle_JPsiMuMu ->SetLineWidth(3);  
  //  hPointingAngle_ppmuX    ->SetLineWidth(3);  
  //  hPointingAngle_ppmumuX  ->SetLineWidth(3);  



 TLegend * leg9 = new TLegend(0.6,0.75,0.93,0.91);
 leg9->AddEntry( hPointingAngle_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg9->AddEntry( hPointingAngle_BOther, "B->JPsi X" );
 leg9->AddEntry( hPointingAngle_BsOther, "Bs -> JPsi X" );
 leg9->AddEntry( hPointingAngle_JPsiMuMu, "prompt JPsi" );
 // leg9->AddEntry( hPointingAngle_ppmuX, "pp mu X");
 // leg9->AddEntry( hPointingAngle_ppmumuX, "pp mumu X");

  hPointingAngle_BsJpsiSignal->DrawNormalized();  
  hPointingAngle_BOther   ->DrawNormalized("SAME");  
  hPointingAngle_BsOther  ->DrawNormalized("SAME");     
  hPointingAngle_JPsiMuMu ->DrawNormalized("SAME");  
  //  hPointingAngle_ppmuX    ->DrawNormalized("SAME");  
  //  hPointingAngle_ppmumuX  ->DrawNormalized("SAME");  


 leg9->Draw("SAME");

 TCanvas *cv10 = new TCanvas("DistSign3D", "DistSign3D", 900, 600);
  cv10 ->cd();
  cv10->SetFillColor(0);
  
  hDistSign3D_BOther      ->SetLineColor(1);  
  hDistSign3D_BsJpsiSignal->SetLineColor(2);  
  hDistSign3D_BsOther  ->SetLineColor(3);     
  hDistSign3D_JPsiMuMu ->SetLineColor(4);  
  // hDistSign3D_ppmuX    ->SetLineColor(5);  
  //  hDistSign3D_ppmumuX  ->SetLineColor(6);  

  hDistSign3D_BOther      ->SetLineWidth(3);  
  hDistSign3D_BsJpsiSignal->SetLineWidth(3);  
  hDistSign3D_BsOther  ->SetLineWidth(3);     
  hDistSign3D_JPsiMuMu ->SetLineWidth(3);  
  //  hDistSign3D_ppmuX    ->SetLineWidth(3);  
  //  hDistSign3D_ppmumuX  ->SetLineWidth(3);  



 TLegend * leg10 = new TLegend(0.6,0.75,0.93,0.91);
 leg10->AddEntry( hDistSign3D_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg10->AddEntry( hDistSign3D_BOther, "B->JPsi X" );
 leg10->AddEntry( hDistSign3D_BsOther, "Bs -> JPsi X" );
 leg10->AddEntry( hDistSign3D_JPsiMuMu, "prompt JPsi" );
 // leg10->AddEntry( hDistSign3D_ppmuX, "pp mu X");
 // leg10->AddEntry( hDistSign3D_ppmumuX, "pp mumu X");

  hDistSign3D_JPsiMuMu ->DrawNormalized();  
  hDistSign3D_BsJpsiSignal->DrawNormalized("SAME");  
  hDistSign3D_BOther   ->DrawNormalized("SAME");  
  hDistSign3D_BsOther  ->DrawNormalized("SAME");     

  //  hDistSign3D_ppmuX    ->DrawNormalized("SAME");  
  //  hDistSign3D_ppmumuX  ->DrawNormalized("SAME");  


 leg10->Draw("SAME");


 TCanvas *cv11 = new TCanvas("DistSign1D", "DistSign1D", 900, 600);
  cv11 ->cd();
  cv11->SetFillColor(0);
  
  hDistSign1D_BOther      ->SetLineColor(1);  
  hDistSign1D_BsJpsiSignal->SetLineColor(2);  
  hDistSign1D_BsOther  ->SetLineColor(3);     
  hDistSign1D_JPsiMuMu ->SetLineColor(4);  
  // hDistSign1D_ppmuX    ->SetLineColor(5);  
  //  hDistSign1D_ppmumuX  ->SetLineColor(6);  

  hDistSign1D_BOther      ->SetLineWidth(3);  
  hDistSign1D_BsJpsiSignal->SetLineWidth(3);  
  hDistSign1D_BsOther  ->SetLineWidth(3);     
  hDistSign1D_JPsiMuMu ->SetLineWidth(3);  
  //  hDistSign1D_ppmuX    ->SetLineWidth(3);  
  //  hDistSign1D_ppmumuX  ->SetLineWidth(3);  



 TLegend * leg11 = new TLegend(0.6,0.75,0.93,0.91);
 leg11->AddEntry( hDistSign1D_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg11->AddEntry( hDistSign1D_BOther, "B->JPsi X" );
 leg11->AddEntry( hDistSign1D_BsOther, "Bs -> JPsi X" );
 leg11->AddEntry( hDistSign1D_JPsiMuMu, "prompt JPsi" );
 // leg11->AddEntry( hDistSign1D_ppmuX, "pp mu X");
 // leg11->AddEntry( hDistSign1D_ppmumuX, "pp mumu X");

  hDistSign1D_JPsiMuMu ->DrawNormalized();  
  hDistSign1D_BsJpsiSignal->DrawNormalized("SAME");  
  hDistSign1D_BOther   ->DrawNormalized("SAME");  
  hDistSign1D_BsOther  ->DrawNormalized("SAME");     

  //  hDistSign1D_ppmuX    ->DrawNormalized("SAME");  
  //  hDistSign1D_ppmumuX  ->DrawNormalized("SAME");  


 leg11->Draw("SAME");


 TCanvas *cv12 = new TCanvas("BsMassBeforeAfterFit", "BsMassBeforeAfterFit", 900, 600);
  cv12 ->cd();
  cv12->SetFillColor(0);
  
  hBsMassFinal_BsJpsiSignal ->SetLineColor(1);  
  hBsMassFinalAfterFit_BsJpsiSignal->SetLineColor(2);  
 

  hBsMassFinal_BsJpsiSignal      ->SetLineWidth(3);  
  hBsMassFinalAfterFit_BsJpsiSignal->SetLineWidth(3);  



 TLegend * leg12 = new TLegend(0.6,0.75,0.93,0.91);
 leg12->AddEntry(  hBsMassFinal_BsJpsiSignal, "without fit" );
 leg12->AddEntry( hBsMassFinalAfterFit_BsJpsiSignal, "with fit" );
 

 hBsMassFinalAfterFit_BsJpsiSignal  ->Draw();  
   hBsMassFinal_BsJpsiSignal->Draw("SAME");  


 leg12->Draw("SAME");



 TCanvas *cv32 = new TCanvas("PhiMassFinal", "PhiMassFinal", 900, 600);
  cv32 ->cd();
  cv32->SetFillColor(0);
  
  hPhiMassFinal_BOther      ->SetLineColor(1);  
  hPhiMassFinal_BsJpsiSignal->SetLineColor(2);  
  hPhiMassFinal_BsOther  ->SetLineColor(3);     
  hPhiMassFinal_JPsiMuMU ->SetLineColor(4);  
  // hPhiMassFinal_ppmuX    ->SetLineColor(5);  
  //  hPhiMassFinal_ppmumuX  ->SetLineColor(6);  

  hPhiMassFinal_BOther      ->SetLineWidth(3);  
  hPhiMassFinal_BsJpsiSignal->SetLineWidth(3);  
  hPhiMassFinal_BsOther  ->SetLineWidth(3);     
  hPhiMassFinal_JPsiMuMU ->SetLineWidth(3);  
  //  hPhiMassFinal_ppmuX    ->SetLineWidth(3);  
  //  hPhiMassFinal_ppmumuX  ->SetLineWidth(3);  



 TLegend * leg32 = new TLegend(0.6,0.75,0.93,0.91);
 leg32->AddEntry( hPhiMassFinal_BsJpsiSignal, "Bs -> JPsi Phi" );
 leg32->AddEntry( hPhiMassFinal_BOther, "B->JPsi X" );
 leg32->AddEntry( hPhiMassFinal_BsOther, "Bs -> JPsi X" );
 leg32->AddEntry( hPhiMassFinal_JPsiMuMU, "prompt JPsi" );
 // leg32->AddEntry( hPhiMassFinal_ppmuX, "pp mu X");
 // leg32->AddEntry( hPhiMassFinal_ppmumuX, "pp mumu X");

  hPhiMassFinal_BsJpsiSignal->Draw();  
  hPhiMassFinal_BOther   ->Draw("SAME");  
  hPhiMassFinal_BsOther  ->Draw("SAME");     
  hPhiMassFinal_JPsiMuMU ->Draw("SAME");  
  //  hPhiMassFinal_ppmuX    ->DrawNormalized("SAME");  
  //  hPhiMassFinal_ppmumuX  ->DrawNormalized("SAME");  


 leg32->Draw("SAME");



}
