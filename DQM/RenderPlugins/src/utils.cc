// $Id: utils.cc,v 1.2 2008/06/02 18:09:55 dellaric Exp $

/*!
  \file utils.cc
  \version $Revision: 1.2 $
  \date $Date: 2008/06/02 18:09:55 $
*/

#include "utils.h"

#include "TH2F.h"
#include "TStyle.h"
#include "TColor.h"
#include "TROOT.h"

using namespace dqm;
bool utils::init = false;
void utils::reportSummaryMapPalette(TH2F* obj)
{
  static int pcol[20];

  if( ! utils::init )
  {
    utils::init = true;

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
