/*
 * =====================================================================================
 *
 *       Filename:  EmuEventDisplay.cc
 *
 *    Description:  draw event display based on encoded histogram 
 *
 *        Version:  1.0
 *        Created:  12/12/2009 08:57:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#include "CSCRenderPlugin_EmuEventDisplay.h"

/**
 * @brief  Constructor
 */
EmuEventDisplay::EmuEventDisplay() {

  histo_zr = new TH2F("h1", "CSC event #x", 22, -11.0, 11.0, 7, 0.0, 7.0);
  histo_zr->GetXaxis()->SetTitle("#Zeta * 1000");
  histo_zr->GetXaxis()->SetTitleOffset(1.2);
  histo_zr->GetXaxis()->CenterTitle(true);
  histo_zr->GetXaxis()->SetLabelSize(0.02);
  histo_zr->GetXaxis()->SetTicks("+-");
  histo_zr->GetYaxis()->SetTitle("R * 1000");
  histo_zr->GetYaxis()->SetTitleOffset(-1.2);
  histo_zr->GetYaxis()->SetTicks("+-");
  histo_zr->GetYaxis()->CenterTitle(true);
  histo_zr->GetYaxis()->SetLabelSize(0.02);
  histo_zr->SetStats(kFALSE);

  histo_zphi = new TH2F("h2", "CSC events #x", 22, -11.0, 11.0, PI * 2 + 0.2, -0.2, PI * 2);
  histo_zphi->GetXaxis()->SetTitle("#Zeta * 1000");
  histo_zphi->GetXaxis()->SetTitleOffset(1.2);
  histo_zphi->GetXaxis()->CenterTitle(true);
  histo_zphi->GetXaxis()->SetLabelSize(0.02);
  histo_zphi->GetXaxis()->SetTicks("+-");
  histo_zphi->GetYaxis()->SetTitle("#phi");
  histo_zphi->GetYaxis()->SetTitleOffset(-1.2);
  histo_zphi->GetYaxis()->SetTicks("+-");
  histo_zphi->GetYaxis()->CenterTitle(true);
  histo_zphi->GetYaxis()->SetLabelSize(0.02);
  histo_zphi->SetStats(kFALSE);

  histo_xy = new TH2F("h3", "CSC event #x", 14, -7.0, 7.0, 14, -7.0, 7.0);
  histo_xy->GetXaxis()->SetTitle("X * 1000");
  histo_xy->GetXaxis()->SetTitleOffset(1.2);
  histo_xy->GetXaxis()->CenterTitle(true);
  histo_xy->GetXaxis()->SetLabelSize(0.02);
  histo_xy->GetXaxis()->SetTicks("+-");
  histo_xy->GetYaxis()->SetTitle("Y * 1000");
  histo_xy->GetYaxis()->SetTitleOffset(-1.2);
  histo_xy->GetYaxis()->SetTicks("+-");
  histo_xy->GetYaxis()->CenterTitle(true);
  histo_xy->GetYaxis()->SetLabelSize(0.02);
  histo_xy->SetStats(kFALSE);

  for (unsigned int side = 1; side <= N_SIDES; side++) {
    for (unsigned int station = 1; station <= N_STATIONS; station++) {
      for (unsigned int ring = 1; ring <= detector.NumberOfRings(station); ring++) {
        
        double rd = (detector.NumberOfStrips(station, ring) * detector.stripDPhiRad(station, ring)) / 2;

        for (unsigned int chamber = 1; chamber <= detector.NumberOfChambers(station, ring); chamber++) {

          if (detector.isChamberInstalled(side, station, ring, chamber)) {

            for (int ipart = 1; ipart <= detector.NumberOfChamberParts(station, ring); ipart++) {
              std::string part = detector.ChamberPart(ipart);
  
              double z1 = detector.Z_mm(side, station, ring, chamber, 1);
              z1 = z1 / 1000.0;
              double z2 = detector.Z_mm(side, station, ring, chamber, N_LAYERS);
              z2 = z2 / 1000.0;

              {
                double r1 = detector.R_mm(side, station, ring, part, 1, 1, 1);
                r1 = r1 / 1000.0;
                double r2 = detector.R_mm(side, station, ring, part, N_LAYERS, detector.NumberOfHalfstrips(station, ring, part), detector.NumberOfWiregroups(station, ring));
                r2 = r2 / 1000.0;
  
                Double_t x[5] = { z1, z1, z2, z2, z1 };
                Double_t y[5] = { r1, r2, r2, r1, r1 };

                TPolyLine *pline = new TPolyLine(5, x, y);
                pline->SetLineColor(kRed - 10);
                pline->SetLineStyle(1);
                pline->SetLineWidth(1);
                zrChambers.push_back(pline);

              }

              {
                double r = detector.PhiRadChamberCenter(station, ring, chamber);

                Double_t x[5] = { z1, z1, z2, z2, z1 };
                Double_t y[5] = { r + rd, r - rd, r - rd, r + rd, r + rd };

                TPolyLine *pline = new TPolyLine(5, x, y);
                pline->SetLineColor(kRed - 10);
                pline->SetLineStyle(1);
                pline->SetLineWidth(1);
                zpChambers.push_back(pline);

              }

              {
                double x1 = detector.X_mm(side, station, ring, part, chamber, 1, 1, 1);
                x1 = x1 / 1000.0;
                double x2 = detector.X_mm(side, station, ring, part, chamber, 1, 1, detector.NumberOfWiregroups(station, ring));
                x2 = x2 / 1000.0;
                double x3 = detector.X_mm(side, station, ring, part, chamber, 1, detector.NumberOfHalfstrips(station, ring, part), 1);
                x3 = x3 / 1000.0;
                double x4 = detector.X_mm(side, station, ring, part, chamber, 1, detector.NumberOfHalfstrips(station, ring, part), detector.NumberOfWiregroups(station, ring));
                x4 = x4 / 1000.0;

                double y1 = detector.Y_mm(side, station, ring, part, chamber, 1, 1, 1);
                y1 = y1 / 1000.0;
                double y2 = detector.Y_mm(side, station, ring, part, chamber, 1, 1, detector.NumberOfWiregroups(station, ring));
                y2 = y2 / 1000.0;
                double y3 = detector.Y_mm(side, station, ring, part, chamber, 1, detector.NumberOfHalfstrips(station, ring, part), 1);
                y3 = y3 / 1000.0;
                double y4 = detector.Y_mm(side, station, ring, part, chamber, 1, detector.NumberOfHalfstrips(station, ring, part), detector.NumberOfWiregroups(station, ring));
                y4 = y4 / 1000.0;

                Double_t x[5] = { x1, x2, x4, x3, x1 };
                Double_t y[5] = { y1, y2, y4, y3, y1 };

                TPolyLine *pline = new TPolyLine(5, x, y);
                pline->SetLineColor(kRed - 10);
                pline->SetLineStyle(1);
                pline->SetLineWidth(1);
                xyChambers.push_back(pline);
  
              }

            }

          }

        }
      }
    }
  }
}

/**
 * @brief  Destructor
 */
EmuEventDisplay::~EmuEventDisplay() {
  
  delete histo_zr;
  delete histo_zphi;
  delete histo_xy;

  for (std::vector<TPolyLine*>::iterator it = zrChambers.begin(); it != zrChambers.end(); it++) {
    delete (*it);
  }

  for (std::vector<TPolyLine*>::iterator it = zpChambers.begin(); it != zpChambers.end(); it++) {
    delete (*it);
  }

  for (std::vector<TPolyLine*>::iterator it = xyChambers.begin(); it != xyChambers.end(); it++) {
    delete (*it);
  }

}

void EmuEventDisplay::drawEventDisplay_ZR(TH2* data) {

  data->GetBinContent(1,1);
  histo_zr->Draw();
  
  for (std::vector<TPolyLine*>::iterator it = zrChambers.begin(); it != zrChambers.end(); it++) {
    (*it)->Draw("l");
  }

}

void EmuEventDisplay::drawEventDisplay_ZPhi(TH2* data) {
  
  data->GetBinContent(1,1);
  histo_zphi->Draw();

  for (std::vector<TPolyLine*>::iterator it = zpChambers.begin(); it != zpChambers.end(); it++) {
    (*it)->Draw("l");
  }

}

void EmuEventDisplay::drawEventDisplay_XY(TH2* data) {
  
  data->GetBinContent(1,1);
  histo_xy->Draw();

  for (std::vector<TPolyLine*>::iterator it = xyChambers.begin(); it != xyChambers.end(); it++) {
    (*it)->Draw("l");
  }

}

