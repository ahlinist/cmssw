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
    // We are on same ladder, so overlap parallel to strips.
    if (info1[0]==info2[0]) return parallelToStrips;
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
