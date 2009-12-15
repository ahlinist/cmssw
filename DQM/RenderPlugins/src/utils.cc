// $Id: utils.cc,v 1.4 2009/11/09 11:05:21 emanuele Exp $

/*!
  \file utils.cc
  \version $Revision: 1.4 $
  \date $Date: 2009/11/09 11:05:21 $
*/

#include "utils.h"

#include "TH2.h"
#include "TStyle.h"
#include "TColor.h"
#include "TROOT.h"

using namespace dqm;
bool utils::initsmp = false;
bool utils::initrgp = false;
void utils::reportSummaryMapPalette(TH2* obj)
{
  static int pcol[20];

  if( ! utils::initsmp )
  {
    utils::initsmp = true;

    float rgb[20][3];

    for( int i=0; i<20; i++ )
    {
      if ( i < 17 )
      {
        rgb[i][0] = 0.80+0.01*i;
        rgb[i][1] = 0.00+0.03*i;
        rgb[i][2] = 0.00;
      }
      else if ( i < 19 )
      {
        rgb[i][0] = 0.80+0.01*i;
        rgb[i][1] = 0.00+0.03*i+0.15+0.10*(i-17);
        rgb[i][2] = 0.00;
      }
      else if ( i == 19 )
      {
        rgb[i][0] = 0.00;
        rgb[i][1] = 0.80;
        rgb[i][2] = 0.00;
      }
      pcol[i] = 901+i;
      TColor* color = gROOT->GetColor( 901+i );
      if( ! color ) color = new TColor( 901+i, 0, 0, 0, "" );
      color->SetRGB( rgb[i][0], rgb[i][1], rgb[i][2] );
    }
  }

  gStyle->SetPalette(20, pcol);

  if( obj )
  {
    obj->SetMinimum(-1.e-15);
    obj->SetMaximum(+1.0);
    obj->SetOption("colz");
  }

}

void utils::redGreenPalette(TH2* obj)
{
  static int pcol[2];

  if( ! utils::initrgp )
  {
    utils::initrgp = true;

    float rgb[2][2];
    rgb[0][0] = 0.80;
    rgb[0][1] = 0.00;
    rgb[1][0] = 0.00;
    rgb[1][1] = 0.80;

    for( int i=0; i<2; i++ )
    {
      pcol[i] = 901+i;
      TColor* color = gROOT->GetColor( 901+i );
      if( ! color ) color = new TColor( 901+i, 0, 0, 0, "" );
      color->SetRGB( rgb[i][0], rgb[i][1], 0. );
    }

    gStyle->SetPalette(2, pcol);
  }

  if( obj )
  {
    obj->SetMinimum(-1.e-15);
    obj->SetMaximum(+1.0);
    obj->SetOption("colz");
  }
}

