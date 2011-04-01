#include <iostream>
#include <TProfile.h>
#include <TF1.h>
#include <vector>

#include "Riostream.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TGraphErrors.h"
#include "TDecompChol.h"
#include "TDecompSVD.h"

using namespace std;


void fullDecode (unsigned int id, unsigned int& subdet, 
		  unsigned int& layer, int & part, vector<unsigned int> & specific, unsigned int& stereo)
{
  subdet = (id>>25) & 0x7;
  if ( subdet==1 ) {  // PXE
    layer = (id>>16) & 0xF;
    part  = 0;
    specific.push_back((id>>8) & 0xFF);        // ladder
    specific.push_back((id>>2) & 0x3F); // nbr   
  } else if ( subdet==2 ) {  // PXB
    layer = (id>>16) & 0xF;
    part  = ((id>>23) & 0x3) == 1 ? +1 : -1 ;
    specific.push_back((id>>10) & 0x3F);        // blade
    specific.push_back((id>>8) & 0x3);        // pannel
    specific.push_back((id>>2) & 0x3F); // nbr   
  } else if ( subdet==3 ) {  // TIB
    layer = (id>>14) & 0x7;
    part  = ((id>>12) & 0x3) == 1 ? +1 : -1 ;
    specific.push_back((id>>10) & 0x3);        // side  
    specific.push_back((id>>4) & 0x3F);        // string
    specific.push_back((id>>2) & 0x3); // nbr   
  } else if ( subdet==4 ) { // TID
    layer = (id>>11) & 0x3;
    part = ((id>>13) & 0x3) == 1 ? +1 : -1 ;
    specific.push_back((id>>9) & 0x3); // ring 
    specific.push_back((id>>7) & 0x3); // side 
    specific.push_back((id>>2) & 0x1F);        // nbr  
  } else if ( subdet==5 ) { // TOB
    layer = (id>>14) & 0x7;
    part = ((id>>12) & 0x3) == 1 ? +1 : -1 ;
    specific.push_back((id>>5) & 0x7F);        // ROD 
    specific.push_back((id>>2) & 0x7); // Nbr 
  } else if ( subdet==6 ) { // TEC
    layer = (id>>14) & 0xF;
    part = ((id>>18) & 0x3) == 1 ? +1 : -1 ;
    specific.push_back((id>>12) & 0x3);     // side 
    specific.push_back((id>>8) & 0xF);	   // petal
    specific.push_back((id>>5) & 0x7);	   // ring 
    specific.push_back((id>>2) & 0x7);	   // nbr  
  } else {
    layer = 0;
  }
  stereo = id&0x3;
}


void printId(unsigned int id)
{
  unsigned int subdet, layer, stereo;
  int part;
  vector<unsigned int> info;
  fullDecode (id, subdet, layer, part,info, stereo);
  if ( subdet==1 ) {
  cout << "PXB " << layer //layer
       << " Ladder " <<info[0]
       << " Nbr " <<info[1];
  }
  else if ( subdet==2 ) {
  cout << "PXE" << (part == +1 ? " + " : " - ") << layer //Wheel
       << " Blade " <<info[0]
       << " Pannel " <<info[1]
       << " Nbr " <<info[2];
  }
  else if ( subdet==3 ) {	// TIB
  cout << "TIB" << (((id>>12) & 0x3) == 1 ? " + " : " - ") << ((id>>14) & 0x7) //layer
       << " Side " <<((id>>10) & 0x3)
       << " String " << ((id>>4) & 0x3F)
       << " Nbr "<< ((id>>2) & 0x3)
       << (stereo == 1 ? " Stereo " : " rPhi");
  }
  else if ( subdet==4 ) {
  cout << "TID" << (part == +1 ? " + " : " - ") << layer //Wheel
       << " Side " <<info[1]
       << " Ring " <<info[0]
       << " Nbr " <<info[2]
       << (stereo == 1 ? " Stereo " : " rPhi");
  }
  else if ( subdet==5 ) {
  cout << "TOB" << (((id>>12) & 0x3) == 1 ? " + " : " - ") << ((id>>14) & 0x7) //layer
       << " ROD " <<((id>>5) & 0x7F)
       << " Nbr "<< ((id>>2) & 0x7)
       << (stereo == 1 ? " Stereo " : " rPhi");
  }
  else if ( subdet==6 ) {
  cout << "TEC" << (part == +1 ? " + " : " - ") << layer //Wheel
       << " Side " <<info[0]
       << " Petal " <<info[1]
       << " Ring " <<info[2]
       << " Nbr " <<info[3]
       << (stereo == 1 ? " Stereo " : " rPhi");
  }
  else {
  
  cout << "Det "<<subdet;
//     layer = 0;
  }
  //cout << endl;
}

bool checkAdjacent(unsigned int id1, unsigned int id2, float path)
{
  unsigned int subdet1, layer1, stereo1, subdet2, layer2, stereo2;
  int part1, part2;
  vector<unsigned int> info1, info2;
  fullDecode (id1, subdet1, layer1, part1,info1, stereo1);
  fullDecode (id2, subdet2, layer2, part2,info2, stereo2);
 // subdet, layer and part check:
  if (subdet1 != subdet2) return false;
  if (layer1 != layer2) return false;
  // if (part1 != part2) return false;
  if (stereo1 != stereo2) return false;


  if ( subdet1==1 ) {	// PXB
      if (fabs(path)>3.) return false;
      return true;
  } else if ( subdet1==2 ) {	// PXE
      if (fabs(path)>10.) return false;
      return true;
  } else if ( subdet1==3 ) {	// TIB
    // First, check on which side both dets are
    if (info1[0]==info2[0]) {
      // Here we are on the same side, interior or exterior
      // The modules can then only be on adjacent strings, and have the same module number
      if ( (info1[1]!=(info2[1]+1)) && (info1[1]!=(info2[1]-1)) ) {
	if (info1[1]!=1) return false;
	// Check for case where String1 == 1
	// interior or exterior have different number of strings
	if ((info1[0] == 1) && (layer1 == 1) && (info2[1] != 26)) return false;
	if ((info1[0] == 1) && (layer1 == 2) && (info2[1] != 34)) return false;
	if ((info1[0] == 1) && (layer1 == 3) && (info2[1] != 44)) return false;
	if ((info1[0] == 1) && (layer1 == 4) && (info2[1] != 52)) return false;
	if ((info1[0] == 2) && (layer1 == 1) && (info2[1] != 30)) return false;
	if ((info1[0] == 2) && (layer1 == 2) && (info2[1] != 38)) return false;
	if ((info1[0] == 2) && (layer1 == 3) && (info2[1] != 46)) return false;
	if ((info1[0] == 2) && (layer1 == 4) && (info2[1] != 56)) return false;
      }
      if (info1[2]!=info2[2]) return false;
      return true;
    } else {
      // So we are on different sides...
      // The problem here is that there are not the same numbers of string on both sides!
      // So cut on the path length...
      if (fabs(path)>10.) return false;
      return true;
//       if ( (info1[1]!=(info2[1]+1)) && (info1[1]!=(info2[1]-1)) && (info1[1]!=info2[1]) ) return false;
//       if ( (info1[2]!=(info2[2]+1)) && (info1[2]!=(info2[2]-1)) && (info1[2]!=info2[2]) ) return false;
//       return true;
    }
  }
  else if ( subdet1==5 ) {	// TOB
    // First, check on the RODs
    if ( (info1[0]!=(info2[0]+1)) && (info1[0]!=(info2[0]-1)) && (info1[0]!=info2[0]) ) {
      if (info1[0]!=1) return false;
      //Check for case where ROD1 == 1
      if ((layer1 == 1) && (info2[0] != 42)) return false;
      if ((layer1 == 2) && (info2[0] != 48)) return false;
      if ((layer1 == 3) && (info2[0] != 54)) return false;
      if ((layer1 == 4) && (info2[0] != 60)) return false;
      if ((layer1 == 5) && (info2[0] != 66)) return false;
      if ((layer1 == 6) && (info2[0] != 74)) return false;
    }
    // Then, Check on the modules
    if ( (info1[1]!=(info2[1]+1)) && (info1[1]!=(info2[1]-1)) && (info1[1]!=info2[1]) ) return false;
    return true;
  } else if ( subdet1==4 ) {	// TID
      if (fabs(path)>20.) return false;
      return true;
  } else if ( subdet1==6 ) {	// TEC
      if (fabs(path)>20.) return false;
      return true;
  }
  return false;
}

enum AdjacentType {notAdjacent, parallelToStrips, perpendicularToStrips };

AdjacentType checkAdjacentType(unsigned int id1, unsigned int id2)
{
  unsigned int subdet1, layer1, stereo1, subdet2, layer2, stereo2;
  int part1, part2;
  vector<unsigned int> info1, info2;
  fullDecode (id1, subdet1, layer1, part1,info1, stereo1);
  fullDecode (id2, subdet2, layer2, part2,info2, stereo2);

  if ( subdet1==3 ) {	// TIB
    // We are on same side, and module nbr are equal, so overlap parallel to strips.
    if ((info1[2]==info2[2]) && (info1[0]==info2[0])) return parallelToStrips;
    else return perpendicularToStrips;
  }
  else if ( subdet1==5 ) {	// TOB
    // We are on same ROD, so overlap perpendicular to strips.
    if (info1[0]==info2[0]) return perpendicularToStrips;
    // We are NOT on same ROD, so overlap parallel to strips.
    return parallelToStrips;
  }
  else if ( subdet1==1 ) {	// PXE
    // We are on same ladder, so overlap perpendicularToStrips to strips.
//     cout << "checking ";
//   cout << id1<< " ( ";
//   printId(id1);
//   cout <<" ) " << id2 << " ( ";
//   printId(id2);
//   cout <<" ) - "<<
//     info1[0]<<" "<<info2[0]<<endl;

    if (info1[0]==info2[0]) return perpendicularToStrips;
    return parallelToStrips;
  }
  return notAdjacent;
}


// void detector (unsigned int id, unsigned int& subdet, 
// 		  unsigned int& sign)
// {
//   subdet = (id>>25) & 0x7;
//   sign = (id>>12) & 0x3;
// }

// enum SubDetector {
//   UNKNOWN = 0, PXB = 1, PXF = 2, TIB = 3, TID = 4, TOB = 5, TEC = 6
// }

void decode (unsigned int id, unsigned int& subdet, 
		  unsigned int& layer, int & part, unsigned int& stereo)
{
  subdet = (id>>25) & 0x7;
  if ( subdet==1 ) {	// PXB
    layer = (id>>16) & 0xF;
    part = 0;
  }
  else if ( subdet==2 ) {	// PXF
    layer = (id>>16) & 0xF;
    part = ((id>>23) & 0x3) == 1 ? +1 : -1 ;
  }
  else if ( subdet==3 ) {	// TIB
    layer = (id>>14) & 0x7;
    part = ((id>>12) & 0x3) == 1 ? +1 : -1 ;
  }
  else if ( subdet==5 ) {	// TOB
    layer = (id>>14) & 0x7;
    part = ((id>>12) & 0x3) == 1 ? +1 : -1 ;
  }
  else if ( subdet==4 ) {	// TID
    layer = (id>>11) & 0x3;
    part = ((id>>13) & 0x3) == 1 ? +1 : -1 ;
  }
  else if ( subdet==6 ) {	// TID
    layer = (id>>14) & 0xf;
    part = ((id>>18) & 0x3) == 1 ? +1 : -1 ;
  }
  else {
    layer = 0;
  }
  stereo = id&0x3;
}


void
fillMean (int ibin, TH1* resultHisto, TH1* inputHisto,
		    float scale = 1.) 
{
  double sum = inputHisto->Integral();
  if ( sum>0.00001 ) {
    resultHisto->SetBinContent(ibin,scale*inputHisto->GetMean());
    resultHisto->SetBinError(ibin,scale*inputHisto->GetRMS());
  }
}


void
fillMeanWithSyst (int ibin, TH1* resultHisto, TH1* inputHisto,
		           TH1* inputSystHisto, float scale) 
{
  double sum = inputHisto->Integral();
  double systEntries = inputSystHisto->GetEntries();
  if ( sum>0.00001 ) {
    resultHisto->SetBinContent(ibin,scale*inputHisto->GetMean());
    //    resultHisto->SetBinError(ibin,scale*(sqrt(((inputHisto->GetRMS()*inputHisto->GetRMS())/sum)+((inputSystHisto->GetRMS()*inputSystHisto->GetRMS())/systEntries)+0.0000168)));
    //the momentum estimation gives an uncertainty of 45% on 1/p, which scales
    //as the uncertainty in the multiple scattering uncertainty
    resultHisto->SetBinError(ibin,scale*(sqrt(((inputHisto->GetRMS()*inputHisto->GetRMS())/sum)+((inputSystHisto->GetRMS()*inputSystHisto->GetRMS())/systEntries)+(0.45*inputHisto->GetMean()*0.45*inputHisto->GetMean()))));
  }
}

TProfile* fillSlopeNew (TH2* inputHisto)  {
  //inputHisto->Rebin2D(2,2);
  TProfile *prof = inputHisto->ProfileX();
  double largest = 0;
  for (int i =1;i<prof->GetNbinsX();++i)
    if (prof->GetBinError(i)>largest) largest = prof->GetBinError(i);

  for (int i =1;i<prof->GetNbinsX();++i)
    if ((prof->GetBinError(i)>0.0) && prof->GetBinError(i)<1E7) prof->SetBinError(i,largest/2);
  return prof;
}

float fillSlope (int ibin, TH1* resultSlopeHisto, TH1* resultOffsetHisto, TH2* inputHisto)  {
  double sum = inputHisto->Integral();
  if ( sum>50 ) {
    TProfile *prof = fillSlopeNew(inputHisto);
    prof->Fit("pol1","Q0","");
//    prof->GetFunction("pol1")->ResetBit(kNotDraw);
    resultSlopeHisto->SetBinContent(ibin,prof->GetFunction("pol1")->GetParameter(1));
    resultSlopeHisto->SetBinError(ibin,prof->GetFunction("pol1")->GetParError(1));
    resultOffsetHisto->SetBinContent(ibin,prof->GetFunction("pol1")->GetParameter(0));
    resultOffsetHisto->SetBinError(ibin,prof->GetFunction("pol1")->GetParError(0));
    return prof->GetFunction("pol1")->GetParameter(1);
  }
  return 999.;
}


void
fillWidth (int ibin, TH1* resultHisto, TH1* inputHisto,
		    float scale) 
{
  double sum = inputHisto->Integral();
  if ( sum>0.00001 ) {
    inputHisto->Fit("gaus","Q0");
    double sigma = inputHisto->GetFunction("gaus")->GetParameter(2);
    double error_sigma = inputHisto->GetFunction("gaus")->GetParError(2);
    resultHisto->SetBinContent(ibin,scale*sigma);
    resultHisto->SetBinError(ibin,scale*error_sigma);
  }
}



float fillSlope (int ibin, TH1* resultSlopeHisto, TH1* resultOffsetHisto,
	vector<double>& vx , vector<double>& vxe, vector<double>& vy, vector<double>& vye)  {

  const Int_t nrPnts = vx.size();
  int pts=0;
  for (int i=0;i<nrPnts;++i){
    if (fabs(vy[i])<0.2) ++pts;
  }
  TVectorD x(pts);
  TVectorD y(pts);
  TVectorD xe(pts);
  TVectorD ye(pts);
  pts=0;
  for (int i=0;i<nrPnts;++i){
    if (fabs(vy[i])<0.2) {
      x[pts]=vx[i];
      y[pts]=vy[i];
      xe[pts]=vxe[i];
      ye[pts]=vye[i];
      ++pts;
    }
  }
  
  if (pts!=nrPnts) cout << "Outlies: Points: "<<pts<<" / "<<nrPnts<<endl;
  TGraphErrors *gr = new TGraphErrors(x,y,xe,ye);
  gr->SetName("gr");
  TF1 *f1 = new TF1("f1","pol1",0,5);
  f1->SetParameter(1, 0.);
  f1->SetParameter(0, 0.);
  gr->Fit("f1","Q");
  //if (ibin==62) 
//  gr->Draw("ALP");
  // cout << f1->GetChisquare()<< " "<< f1->GetNDF()<< " "<<f1->GetChisquare()/f1->GetNDF()<< " "<< f1->GetParameter(0)<< " "<< f1->GetParameter(1)<<endl;

  resultSlopeHisto->SetBinContent(ibin,f1->GetParameter(1));
  resultSlopeHisto->SetBinError(ibin,f1->GetParError(1));
  resultOffsetHisto->SetBinContent(ibin,f1->GetParameter(0));
  resultOffsetHisto->SetBinError(ibin,f1->GetParError(0));
 delete f1;
 delete gr;
  return 0;//f1->GetParameter(1);
}

bool slopeOK(AdjacentType type, float slope)
{
  if (type == parallelToStrips) {
    if (fabs(slope) > 0.002) {
      cout << "Large rotation/Slope, Gaussian fit of DD unreliable: ";
      return false;
    }
  } else {
    if (fabs(slope) > 0.05) {
      cout << "Large rotation/Slope, Gaussian fit of DD unreliable: ";
      return false;
    }
  }
  return true;
}

  /**
   * Print the full module info for the pair
   */
void idPrint(DetIdPair idPair)
{
  cout << idPair.first<< " ( ";
  printId(idPair.first);
  cout <<" ) " << idPair.second << " ( ";
  printId(idPair.second);
  cout <<" ) - ";
  AdjacentType adj = checkAdjacentType(idPair.first, idPair.second);
  switch ( adj ) {
    case notAdjacent : 
      cout << "NotAdjacant\n";
    break;
    case parallelToStrips : 
      cout << "Ovrlp parallel in Z\n";
    break;
    case perpendicularToStrips : 
      cout << "Ovrlp perpendicular in Z\n";
  }

}

bool halfBarrelOverlap(DetIdPair idPair)
{
  unsigned int subdet1, layer1, stereo1, subdet2, layer2, stereo2;
  int part1, part2;
  vector<unsigned int> info1, info2;
  fullDecode (idPair.first, subdet1, layer1, part1,info1, stereo1);
  fullDecode (idPair.second, subdet2, layer2, part2,info2, stereo2);
  if (subdet1!=1) return false;
  if (layer1==1) {
    if (info1[0]==5 && info2[0]==6) return true;
    if (info1[0]==6 && info2[0]==5) return true;
    if (info1[0]==15 && info2[0]==16) return true;
    if (info1[0]==16 && info2[0]==15) return true;
  } else if (layer1==2) {
    if (info1[0]==8 && info2[0]==9) return true;
    if (info1[0]==9 && info2[0]==8) return true;
    if (info1[0]==25 && info2[0]==24) return true;
    if (info1[0]==24 && info2[0]==25) return true;
  } else if (layer1==3) {
    if (info1[0]==11 && info2[0]==12) return true;
    if (info1[0]==12 && info2[0]==11) return true;
    if (info1[0]==33 && info2[0]==34) return true;
    if (info1[0]==34 && info2[0]==33) return true;
  }
//   if (layer1==1) {
//     if (info1[0]==0 && info2[0]==19) return true;
//     if (info1[0]==19 && info2[0]==0) return true;
//     if (info1[0]==9 && info2[0]==10) return true;
//     if (info1[0]==10 && info2[0]==9) return true;
//   } else if (layer1==2) {
//     if (info1[0]==0 && info2[0]==31) return true;
//     if (info1[0]==31 && info2[0]==0) return true;
//     if (info1[0]==15 && info2[0]==16) return true;
//     if (info1[0]==16 && info2[0]==15) return true;
//   } else if (layer1==3) {
//     if (info1[0]==0 && info2[0]==41) return true;
//     if (info1[0]==41 && info2[0]==0) return true;
//     if (info1[0]==21 && info2[0]==22) return true;
//     if (info1[0]==22 && info2[0]==21) return true;
//   }
  return false;
}

bool halfBarrelOverlap(unsigned int id)
{
  unsigned int subdet1, layer1, stereo1;
  int part1;
  vector<unsigned int> info1;
  fullDecode (id, subdet1, layer1, part1,info1, stereo1);
  if (subdet1!=1) return false;
  if (layer1==1) {
    if (info1[0]==5 || info1[0]==6 || info1[0]==15 ||info1[0]==16) return true;
  } else if (layer1==2) {
    if (info1[0]==8 || info1[0]==9 || info1[0]==24 ||info1[0]==25) return true;
  } else if (layer1==3) {
    if (info1[0]==11|| info1[0]==12|| info1[0]==33 ||info1[0]==34) return true;
  }
  return false;
}

TStyle* setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
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

  tdrStyle->SetEndErrorSize(15);
//   tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(1);
  
  tdrStyle->SetMarkerStyle(21);
  tdrStyle->SetMarkerSize(1.);

//For the fit/function:
  tdrStyle->SetOptFit(0);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
//  tdrStyle->SetOptFile(1111);
//  tdrStyle->SetOptStat(1111111); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.2);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.02);

// For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleW(0.8); // Set the width of the title box

  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
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
  tdrStyle->SetTitleYOffset(1.);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(0);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(0);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
//   tdrStyle->SetPaperSize(21.,28.);
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
  return tdrStyle;
}
