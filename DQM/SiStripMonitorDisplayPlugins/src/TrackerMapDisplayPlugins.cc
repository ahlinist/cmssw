//<<<<<< INCLUDES                                                       >>>>>>
#include "DQM/SiStripMonitorDisplayPlugins/interface/TrackerMapDisplayPlugins.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "VisMonitoring/VisDQMBase/interface/VisDQMOps.h"
#include "VisMonitoring/VisDQMBase/interface/VisDQMGroupTwig.h"
#include <iostream>
#include <TObject.h>
#include <TH1F.h>

#include <cassert>
#include <string>

TrackerMapDisplayPlugins::TrackerMapDisplayPlugins (IgState *state)
    : VisDQMDisplayPlugin (state)
{
    // Here you can setup plugin configuration GUIs, if needed.
  // Create Tracker Map Box
  tkMapBox= new VisTrackerMapBox ( 0,"TrackerMap");
//  igDocumentData = IgDocumentData::get (state);
  updateCounter = 0;
}

bool
TrackerMapDisplayPlugins::applies (DisplayData *data)
{
  if (!data->object) return false;

  std::string name = (data->object)->GetName();
  
  updateCounter++;
  // SiStripTracker DQM related MEs start with ""
  if( name.find( "TrackerMap_for_" ) == 0 && (updateCounter%30 == 1) ) {
    return true;
  }
  return false;
}

std::string
TrackerMapDisplayPlugins::preDraw (DisplayData *data)
{
  if (!(data->object)) return "failed";
 //creation of QtTrackerMap
  tkMapBox->show ();
  
  //to get adress of svg trackermap
  TrackerMap *tm = tkMapBox->getTrackerMap();
  
  //fill the trackermap
  //  map<const int,TmModule*>::iterator imod;
  TH1F* th_tkmap = dynamic_cast<TH1F *> (data->object);
  if (!th_tkmap) return "failed";
  int nbin = th_tkmap->GetNbinsX();
  int idet;
  int rval = 0;
  int gval = 0;
  int bval = 0;
  for (int i = 1; i < nbin+1; i++) {
    idet = atoi(th_tkmap->GetXaxis()->GetBinLabel(i));
    int status = int(th_tkmap->GetBinContent(i));
    if (status == dqm::qstatus::STATUS_OK) { 
      rval = 0;   gval = 255;   bval = 0; 
    } else if (status == dqm::qstatus::WARNING) { 
      rval = 255; gval = 255; bval = 0;
    } else if (status == dqm::qstatus::ERROR) { 
      rval = 255; gval = 0;  bval = 0;
    }
    tm->fillc(idet, rval, gval, bval);    
  }
// Get the twig associated to the monitorable you want to look at.
/*  ASSERT (igDocumentData);
  IgTwig *twig = igDocumentData->root ()->lookup ("/Some/Path/in/DQM");
  
  // Create a VisDQMGroupTwig with your subscription.
  VisDQMOps *ops = VisDQMOps::get (m_state);
  ASSERT (ops);
  VisDQMGroupTwig *groupTwig = ops->subscribe (twig, "Some Label");
  
  // Create a browser watching only the selected subbranch
  IgPage *page = igDocumentData->document ()->currentPage ();    
  IgState *state = new IgState (m_state);
  new IgSelectionService (state);    
  VisRootBrowser *browser = new VisRootBrowser (state, page->workspace ());
  browser->browse (groupTwig);
  browser->parentWidget ()->setCaption (groupTwig->name ());*/

  //  for (imod=IdModuleMap::imoduleMap.begin();imod != IdModuleMap::imoduleMap.end(); imod++){

    //    TmModle* mod = imod->second;
    //    tm->fill(imod->first, ((float)rand()/RAND_MAX) );
    //    tm->fill(imod->first, ((float)rand()/RAND_MAX) );
    //    tm->fill(imod->first, ((float)rand()/RAND_MAX) );
    //    if(((float)rand()/RAND_MAX)<.001){
    //      tm->fillc(imod->first,255,0,0);
    //      tm->setText(imod->first,"This module is red!");
    //    }
  //  }
  
  //setup paint options defines the same variables that where used before
  //calling the print method of trackermap
  tkMapBox->setPaintOptions(false);
  
  //to be called when you want to repaint the trackemap
  tkMapBox->update();
  return "success";
}

void
TrackerMapDisplayPlugins::postDraw (DisplayData */*data*/)
{
    // No post action required.
    return;    
}
