// $Id: DTMDisplayPlugins.cc,v 1.3 2007/04/13 10:46:24 giorgia Exp $

/*!
  \file DTMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.3 $
  \date $Date: 2007/04/13 10:46:24 $
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
  
  if (LocalRecoTestME.isLocalRecoTestME(name)) {
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

  if (LocalRecoTestME.isLocalRecoTestME(name)) {
    
    return LocalRecoTestME.preDraw(data);
    
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

  if (LocalRecoTestME.isLocalRecoTestME(name)) {
    
    return LocalRecoTestME.postDraw(data);
    
  }


  return ;    

}

