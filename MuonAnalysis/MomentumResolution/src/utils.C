#include "utils.h"
#define MMuon 0.105658
void Tutils::draw_label(t_label l[], double x1, double y1,double dx, double dy,int n){
   TLegend *label  = new TLegend(x1,y1,x1+dx,y1+dy,NULL,"brNDC");
  label->SetBorderSize(0); label->SetFillColor(0); label->SetTextSize(0.042);
  for(int i=0;i<n;i++){
    label->AddEntry(l[i].h,((l[i].text).str()).c_str(),"l");
  }
  label->Draw("same");
}


double Tutils::modpt(double pt, double pt_gen, double r,double rp, const double * sigma, double eta,int mode){
  double ptmod = pt;
  // mode == 0 does not work when MC distribution is wider than data one. This is solved with mode == 1
  if(mode==1){
    if(fabs(eta)<0.9){
      ptmod = 1/( sigma[2]/pt + (1-sigma[2])/pt_gen );
      ptmod = ptmod + ptmod*(sigma[0] + r*sigma[1] + sigma[3]*(1./1000.)*ptmod);
    }else if(fabs(eta)<2.1){
      ptmod = 1/( sigma[4]/pt + (1-sigma[4])/pt_gen );
      ptmod = ptmod + ptmod*(sigma[0] + r*sigma[1] + sigma[5]*(1./1000.)*ptmod);
    }else{
      ptmod = 1/( sigma[6]/pt + (1-sigma[6])/pt_gen );
      ptmod = ptmod + ptmod*(sigma[0] + r*sigma[1] + sigma[7]*(1./1000.)*ptmod);    }
    //ptmod = pt_gen + pt_gen*(sigma[0] + r*sigma[1] + sigma[3]*(1./1000.)*pt_gen) + (1/( sigma[2]/pt + (1-sigma[2])/pt_gen )-pt_gen);
  }else if(mode == 0){
    if(fabs(eta)<0.9){
      ptmod = pt + pt*(sigma[0] + r*sigma[1] + rp*sigma[2]*(1./1000.)*pt_gen + sigma[3]*(1./1000.)*pt_gen);}
    else if(fabs(eta)<2.1)
      ptmod = pt + pt*(sigma[0] + r*sigma[1] + rp*sigma[4]*(1./1000.)*pt_gen + sigma[5]*(1./1000.)*pt_gen);
    else 
      ptmod = pt + pt*(sigma[0] + r*sigma[1] + rp*sigma[6]*(1./1000.)*pt_gen + sigma[7]*(1./1000.)*pt_gen);
  }
  return ptmod;
}
double Tutils::modpt(double pt, double pt_gen, double r,double rp, const double * sigma, int mode){
  double ptmod = pt;
  // mode == 0 does not work when MC distribution is wider than data one. This is solved with mode == 1
  if(mode==1){
    ptmod = 1/( sigma[2]/pt + (1-sigma[2])/pt_gen );
    ptmod = ptmod + ptmod*(sigma[0] + r*sigma[1] + sigma[3]*(1./1000.)*ptmod);
  }else if(mode == 0){
    ptmod = pt + pt*(sigma[0] + r*sigma[1] + rp*sigma[2]*(1./1000.)*pt_gen + sigma[3]*(1./1000.)*pt_gen);}
  
  return ptmod;
}

double Tutils::computeMass(t_data& data,double r1, double r2,double rp1, double rp2,const double sigma[],int mode,bool etabin) {
  double ptmod1; 
  double ptmod2;
  if(etabin) ptmod1 = modpt(data.pt1,data.pt1_gen,r1,rp1,sigma,data.eta1,mode);
  else ptmod1 = modpt(data.pt1,data.pt1_gen,r1,rp1,sigma,mode);
  if(etabin) ptmod2 = modpt(data.pt2,data.pt2_gen,r2,rp2,sigma,data.eta2,mode);
  else ptmod2 = modpt(data.pt2,data.pt2_gen,r2,rp2,sigma,mode);
  double px1 = ptmod1*data.px_pt1;
  double py1= ptmod1*data.py_pt1;
  double pz1 = ptmod1*data.pz_pt1;
  double E1= ptmod1*data.p_pt1;
  double px2 = ptmod2*data.px_pt2;
  double py2= ptmod2*data.py_pt2;
  double pz2 = ptmod2*data.pz_pt2;
  double E2= ptmod2*data.p_pt2;
  double px, py, pz, E;
  px = px1 + px2; py = py1 + py2; pz = pz1 + pz2; E = E1 + E2;
  return sqrt(E*E - px*px - py*py - pz*pz);
}
double Tutils::computeMass(t_data& data,const double sigma[],int mode,bool etabin) {
  double ptmod1;
  double ptmod2; 
  if(etabin) ptmod1 = modpt(data.pt1,data.pt1_gen,data.r1,data.rp1,sigma,data.eta1,mode); 
  else ptmod1 = modpt(data.pt1,data.pt1_gen,data.r1,data.rp1,sigma,mode); 
  if(etabin) ptmod2= modpt(data.pt2,data.pt2_gen,data.r2,data.rp2,sigma,data.eta2,mode);
  else ptmod2= modpt(data.pt2,data.pt2_gen,data.r2,data.rp2,sigma,mode);
  double px1 = ptmod1*data.px_pt1;
  double py1= ptmod1*data.py_pt1;
  double pz1 = ptmod1*data.pz_pt1;
  double E1= ptmod1*data.p_pt1;
  double px2 = ptmod2*data.px_pt2;
  double py2= ptmod2*data.py_pt2;
  double pz2 = ptmod2*data.pz_pt2;
  double E2= ptmod2*data.p_pt2;
  double px, py, pz, E;
  px = px1 + px2; py = py1 + py2; pz = pz1 + pz2; E = E1 + E2;
  return sqrt(E*E - px*px - py*py - pz*pz);
}

double Tutils::likelihood(TH1D * hZmassVar, TH1D * hZmassModel){
  double lhood=0;
  for(int i=0; i<=hZmassVar->GetXaxis()->GetNbins()+1; i++){
    if(hZmassVar->GetBinContent(i)!=0) lhood += hZmassVar->GetBinContent(i) - hZmassModel->GetBinContent(i)*log(hZmassVar->GetBinContent(i));
  }
  return lhood;
}
void Tutils::setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(400); //Height of canvas
  tdrStyle->SetCanvasDefW(400); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  //  tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  //tdrStyle->SetPadRightMargin(0.02);
  tdrStyle->SetPadRightMargin(0.08);

// For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:
  //tdrStyle->SetLabelColor(2, "XYZ"); //tdr??
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetNdivisions(506, "X");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  //tdrStyle->SetPadGridX(true);
  //tdrStyle->SetPadGridY(true);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

}
