// $Id: DTMDisplayPlugins.cc,v 1.2 2007/04/03 09:27:53 gmasetti Exp $

/*!
  \file DTMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.2 $
  \date $Date: 2007/04/03 09:27:53 $
*/

#include "DQM/DTMonitorDisplayPlugins/interface/DTMDisplayPlugins.h"
//#include "DQM/DTMonitorDisplayPlugins/interface/ColorPalette.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include <iostream>
#include <TROOT.h>
#include <TGraph.h>
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TVirtualPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>



DTMDisplayPlugins::DTMDisplayPlugins( IgState *state ) : VisDQMDisplayPlugin( state ) {

}

bool DTMDisplayPlugins::applies( DisplayData *data ) {
 
  name = (data->object)->GetName();
  
  if (DigiME.isDigiME(name)) {
    return true;
  }

  if (DataIntegrityME.isDataIntegrityME(name)) {
    return true;
  }

  if (tTrigTestME.istTrigTestME(name)) {
    return true;
  }

  if (LocalRecoME.isLocalRecoME(name)) {
    return true;
  }

  return false;

}

std::string DTMDisplayPlugins::preDraw( DisplayData *data ) {

  name = (data->object)->GetName();
  
  
  if (DigiME.isDigiME(name)) {
    
    return DigiME.preDraw(data);
    
  }

  if (DataIntegrityME.isDataIntegrityME(name)) {
    
    return DataIntegrityME.preDraw(data);
    
  }
 
  if (tTrigTestME.istTrigTestME(name)) {
    
    return tTrigTestME.preDraw(data);
    
  }

  if (LocalRecoME.isLocalRecoME(name)) {
    
    return LocalRecoME.preDraw(data);
    
  }


  return "";
  
}


void DTMDisplayPlugins::postDraw( DisplayData *data ) {

  name = (data->object)->GetName();
    
  if (DigiME.isDigiME(name)) {
    
    return DigiME.postDraw(data);
    
  }

  if (DataIntegrityME.isDataIntegrityME(name)) {
    
    return DataIntegrityME.postDraw(data);
    
  }

  if (tTrigTestME.istTrigTestME(name)) {
    
    return tTrigTestME.postDraw(data);
    
  }

  if (LocalRecoME.isLocalRecoME(name)) {
    
    return LocalRecoME.postDraw(data);
    
  }

  return ;    

}

