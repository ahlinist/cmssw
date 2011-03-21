// Original Author:  Ignazio Lazzizzera
//         Created:  Fri Oct 16 15:44:58 CEST 2009
// $Id: DTL1slhcPlots.cc,v 1.1 2010/03/03 13:09:38 arose Exp $
//
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcPlots.h"

#include "TExec.h" 
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/RootStyles.h"

void DTL1slhcPlots::magnetic_field_global_map()
{
  if(theField == 0)
    return;
  GlobalPoint P = GlobalPoint(0.,0.,0.);
  TH2F *hBField = new TH2F("magnetic_filed","",
			   1500,-1500,0,750,0,750);
  double y =0.0, z=0.0;
  for (Int_t iz = 0; iz < 1500; iz++) {
    for (Int_t iy = 0; iy < 750; iy++) {
      y = iy + 0.5;
      z = -(iz + 0.5);
      P = GlobalPoint(0., y, z);	
      hBField->Fill(-iz, iy, theField->inTesla(P).z());
    }
  }

  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);  
  hBField->GetXaxis()->SetTitle("CMS Z axis (cm)");
  hBField->GetXaxis()->SetTitleOffset(0.95);
  hBField->GetXaxis()->SetTitleSize(0.07);
  hBField->GetXaxis()->CenterTitle();
  hBField->GetXaxis()->SetTitleColor(kBlack);
  hBField->GetYaxis()->SetTitle("CMS Y axis (cm)");
  hBField->GetYaxis()->SetTitleOffset(0.75);
  hBField->GetYaxis()->SetTitleSize(0.07);
  hBField->GetYaxis()->CenterTitle();
  hBField->GetYaxis()->SetTitleColor(kBlack);

  hBField->Draw("COLZ");
  TExec *ex1 = new TExec("ex1","palette();");
  ex1->Draw();
 
  hBField->Draw("COLZ same");
}



void DTL1slhcPlots::magnetic_field_barrelYaxis_profile()
{
  if(theField == 0)
    return;

  GlobalPoint P = GlobalPoint(0.,0.,0.);
  TH1F *hBField = new TH1F("magnetic_filed_barrelYaxis", "", 750,0,750);

  double y =0.0;
  for (Int_t iy = 0; iy < 750; iy++) {
    y = iy + 0.5;
    P = GlobalPoint(0., y, 0.);	
    hBField->Fill(iy, theField->inTesla(P).z());
  }

  hBField->SetLineWidth(2);
  hBField->SetLineColor(kBlack);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);  
  hBField->GetXaxis()->SetTitle("CMS Y axis (cm)");
  hBField->GetXaxis()->SetTitleOffset(0.95);
  hBField->GetXaxis()->SetTitleSize(0.07);
  hBField->GetXaxis()->CenterTitle();
  hBField->GetXaxis()->SetTitleColor(kBlack);
  hBField->GetYaxis()->SetTitle("CMS B Field (Tesla)");
  hBField->GetYaxis()->SetTitleOffset(0.7);
  hBField->GetYaxis()->SetTitleSize(0.07);
  hBField->GetYaxis()->CenterTitle();
  hBField->GetYaxis()->SetTitleColor(kBlack);

  hBField->Draw();
}
