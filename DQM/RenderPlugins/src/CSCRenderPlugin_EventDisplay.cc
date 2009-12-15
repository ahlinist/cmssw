/*
 * =====================================================================================
 *
 *       Filename:  EventDisplay.cc
 *
 *    Description:  draw event display based on encoded historgam chamber 
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

#include "CSCRenderPlugin_EventDisplay.h"

/**
 * @brief  Constructor
 */
EventDisplay::EventDisplay() { 

  p_objs = 0;

  gPad->SetLeftMargin(0.0);
  gPad->SetRightMargin(0.0);
  gPad->SetTopMargin(0.0);
  gPad->SetBottomMargin(0.0);

  pad0 = new TPad("pad0", "Title Pad", 0.00, 0.95, 1.0, 1.00, 0);
  pad1 = new TPad("pad1", "Top pad",   0.00, 0.70, 1.0, 0.95, 0);
  pad2 = new TPad("pad2", "Mid pad",   0.00, 0.25, 1.0, 0.50, 0);
  pad3 = new TPad("pad3", "Bot pad",   0.00, 0.00, 1.0, 0.25, 0);
  pad4 = new TPad("pad4", "Cover pad", 0.00, 0.00, 1.0, 1.00, 0);

}

/**
 * @brief  Destructor
 */
EventDisplay::~EventDisplay() {

  for (unsigned int i = 0; i < p_objs; i++) {
    //delete objs[i];
  }

  //delete pad0;
  //delete pad1;
  //delete pad2;
  //delete pad3;

}

/**
 * @brief  Draw a single chamber event based on encoded histogram
 * @param  me oncoded histogram
 */
void EventDisplay::drawSingleChamber(TH2*& data) {

  pad4->Draw();
  pad0->Draw();
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();

  int endcap  = (int) data->GetBinContent(1, 1);
  int station = (int) data->GetBinContent(1, 2);
  int ring    = (int) data->GetBinContent(1, 3);
  int chamber = (int) data->GetBinContent(1, 4);
  //int crate   = (int) data->GetBinContent(1, 5);
  //int dmb     = (int) data->GetBinContent(1, 6);
  int event   = (int) data->GetBinContent(1, 7);

  // *************************************
  // Anode hits and ALCTs
  // *************************************
  {
    pad1->cd();

    gPad->SetLeftMargin(0.05);
    gPad->SetRightMargin(0.08);
    gPad->SetTopMargin(0.08);
    gPad->SetBottomMargin(0.12);

    drawEventDisplayGrid(data, 4, 2, 3,
                     countWiregroups(station, ring), 0.0f, -5.0f, 5.0f, 0, -3, -8,
                     "Anode Hit Timing and Quality", "wiregroup #");
  }

  // *************************************
  // Cathode hits and CLCTs
  // *************************************
  {
    pad2->cd();

    gPad->SetLeftMargin(0.05);
    gPad->SetRightMargin(0.08);
    gPad->SetTopMargin(0.08);
    gPad->SetBottomMargin(0.12);

    drawEventDisplayGrid(data, 12, 10, 11,
                     (countStrips(station, ring) + countStripsNose(station, ring)) * 2, (countStripsNose(station, ring) > 0 ? 0.0f : 1.0f), -5.0f, 5.0f, (countStripsNose(station, ring) > 0 ? countStrips(station, ring) * 2 : 0), 155, -7,
                     "Comparator Hit Timing and Quality", "half-strip #");
  }

  // *************************************
  // SCA Charges
  // *************************************
  {
    pad3->cd();

    gPad->SetLeftMargin(0.05);
    gPad->SetRightMargin(0.08);
    gPad->SetTopMargin(0.08);
    gPad->SetBottomMargin(0.12);

    drawEventDisplayGrid(data, 18, -1, -1,
                     (countStrips(station, ring) + countStripsNose(station, ring)), (countStripsNose(station, ring) > 0 ? 0.0f : 0.5f), 0.0f, 1000.0f, (countStripsNose(station, ring) > 0 ? countStrips(station, ring) : 0), 0, 0,
                     "SCA Charges", "strip #");
  }

  pad0->cd();

  TString t = Form("Chamber ME%s%d/%d/%d Event #%d", (endcap == 1 ? "+" : "-"), station, ring, chamber, event);
  TText *title = new TText(0.02, 0.30, t);
  title->SetTextAlign(11);
  title->SetTextFont(62);
  title->SetTextSize(0.6);
  title->Draw();
  objs[p_objs++] = title;

}

void EventDisplay::drawEventDisplayGrid(TH2* data, int data_first_col, int data_time_col, int data_quality_col,
                     int count_x, float shift_x, float min_z, float max_z, int split_after_x, int time_corr, int d_corr,
                     const char* title, const char* title_x) {

  TObject *post_draw[160 * 2];
  int p_post_draw = 0;

  TH2F *h1 = new TH2F("h1", title, HISTO_WIDTH, 0, HISTO_WIDTH, 6, 0, 6);
  h1->GetXaxis()->SetLabelSize(0.0);
  h1->GetXaxis()->SetTickLength(0.0);
  h1->GetYaxis()->SetLabelSize(0.0);
  h1->GetYaxis()->SetTickLength(0.0);
  h1->SetStats(kFALSE);
  h1->SetBinContent(1, 1, min_z);
  h1->SetBinContent(1, 2, max_z);
  h1->Draw("colz");
  objs[p_objs++] = h1;

  TBox *bBlank = new TBox(-1.0, -1.0, HISTO_WIDTH + 4, 7);
  bBlank->SetFillColor(0);
  bBlank->SetLineColor(0);
  bBlank->SetLineStyle(0);
  bBlank->Draw("l");
  objs[p_objs++] = bBlank;

  float w = (float) HISTO_WIDTH / (count_x + (split_after_x == 0 ? 0 : 2 ));
  if (split_after_x == 0) split_after_x = count_x;

  for (int l = 0; l < 6; l++) {

    int y = 6 - l;

    for (int xg = 0; xg < count_x; xg++) {

      int section_shift = (xg + 1 > split_after_x ? 2 : 0);

      float x = (shift_x * w * ((l + 1) % 2)) + (float) xg * w + section_shift * w;
      int d = (int) data->GetBinContent(data_first_col + l, xg + 1);

      int time = 0;
      if (data_time_col >= 0) {
        time = (int) data->GetBinContent(data_time_col, xg + 1);
      }

      int quality = 0;
      if (data_quality_col >= 0) {
        quality = (int) data->GetBinContent(data_quality_col, xg + 1);
      }

      int color = 0;
      if (d > 0) {
        if (data_time_col >= 0) {
          d = d - 1;
        }
        d += d_corr;
        if (d > max_z) {
          d = (int) max_z;
        }
        color = 51 + (int) (((float) d / (float) (max_z - min_z)) * 49.0);
      }

      TBox* b = new TBox(x, y, x + w, y - 1);
      b->SetFillColor(color);
      b->SetLineColor(15);
      b->SetLineStyle(1);
      b->Draw("l");
      objs[p_objs++] = b;
        
      TString td1 = Form("%d", d);
      TText* td = new TText(x + w / 2, y - 0.53, td1);
      td->SetTextAlign(22);
      td->SetTextFont(42);
      td->SetTextSize(0.05);
      td->Draw();

      if (l == 2 && quality > 0) {

        color = 51 + (int) (((float) (time + time_corr) / (float) (max_z - min_z)) * 49.0);
        TBox* b1 = new TBox(x - w * 0.25, y - 0.25, x + w * 1.25, y - 1 + 0.25);
        b1->SetFillColor(color);
        b1->SetLineColor(1);
        b1->SetLineStyle(1);
        post_draw[p_post_draw++] = b1;
        objs[p_objs++] = b1;
        
        TString h = Form("%d", quality);
        TText* t = new TText(x + w / 2, y - 0.53, h);
        t->SetTextAlign(22);
        t->SetTextFont(42);
        t->SetTextSize(0.05);
        post_draw[p_post_draw++] = t;
        objs[p_objs++] = t;

      }

      if (l == 5 && (count_x < 100 || (count_x > 100 && (xg + 1) % 2))) {
        TString ts = Form("%d", (section_shift > 0 ? xg - split_after_x + 1 : xg + 1));
        TText* t1 = new TText(x + w / 2, y - 1.2, ts);
        t1->SetTextAlign(count_x >= 100 ? 32 : 22);
        t1->SetTextFont(42);
        t1->SetTextSize(0.03);
        t1->SetTextAngle(count_x >= 100 ? 90 : 0);
        t1->Draw();
        objs[p_objs++] = t1;
      }

    }

    TString ts = Form("%d", l + 1);
    TText* t2 = new TText(-2, y - 0.5, ts);
    t2->SetTextAlign(22);
    t2->SetTextFont(42);
    t2->SetTextSize(0.04);
    t2->Draw();
    objs[p_objs++] = t2;

  }

  TText* t3 = new TText(-7, 3, "layer");
  t3->SetTextAlign(22);
  t3->SetTextFont(42);
  t3->SetTextSize(0.05);
  t3->SetTextAngle(90.0);
  t3->Draw();
  objs[p_objs++] = t3;

  TText* t4 = new TText(HISTO_WIDTH, -0.7, title_x);
  t4->SetTextAlign(32);
  t4->SetTextFont(42);
  t4->SetTextSize(0.05);
  t4->Draw();
  objs[p_objs++] = t4;

  for (int i = 0; i < p_post_draw; i++) {
    post_draw[i]->Draw("l");
  }

}

/**
 * @brief  Wiregroup number on chamber by station and ring
 * @param  station station
 * @param  ring ring
 * @return wiregroups per chamber
 */
int EventDisplay::countWiregroups(int station, int ring) const {
  if (station == 1) {
    if (ring == 1) return 48;
    if (ring == 2) return 32;
    if (ring == 3) return 112;
  }
  if (station == 2) {
    if (ring == 1) return 112;
  }
  if (ring == 1) return 96;
  return 64;
}

/**
 * @brief  Number of strips per chamber by station and ring 
 * @param  station station
 * @param  ring ring
 * @return number of strips in chamber
 */
int EventDisplay::countStrips(int station, int ring) const {
  if (station == 1 && (ring == 1 || ring == 3)) return 64;
  return 80;
}

/**
 * @brief  Number of strips in inner corner (exception for ME1/1)  
 * @param  station station
 * @param  ring ring 
 * @return number of strips in inner corner
 */
int EventDisplay::countStripsNose(int station, int ring) const {
  if (station == 1 && ring == 1) return 16;
  return 0;
}

