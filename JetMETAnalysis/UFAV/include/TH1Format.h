#ifndef TH1FORMAT_H
#define TH1FORMAT_H

#include "TH1F.h"
#include "TH1D.h"
#include "TString.h"
#include "TFile.h"
#include <iomanip>
#include <iostream>
using namespace std;

struct TH1Format {
 public:
  Color_t linecolor;
  Color_t fillcolor;
  Color_t markercolor;
  Style_t linestyle;
  Style_t fillstyle;
  Style_t markerstyle;
  Width_t linewidth;
  Size_t  markersize;
  TString drawoption;
  
TH1Format() : 
  linecolor(kBlack), fillcolor(kNone), markercolor(kNone), 
    linestyle(1), fillstyle(0), markerstyle(21), linewidth(2), markersize(1), drawoption(""){}
};


#endif
