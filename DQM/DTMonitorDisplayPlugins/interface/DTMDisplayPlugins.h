// $Id: DTMDisplayPlugins.h,v 1.4 2007/04/13 15:12:40 giorgia Exp $

#ifndef  DTMDisplayPlugins_H
# define DTMDisplayPlugins_H

/*!
  \file DTMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.4 $
  \date $Date: 2007/04/13 15:12:40 $
*/

# include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTDigiDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTDataIntegrityDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTtTrigTestDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTLocalTriggerDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTLocalRecoDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTLocalRecoTestDisplayPlugins.h"
# include <string>
# include <TH2C.h>


class DTMDisplayPlugins : public VisDQMDisplayPlugin {

 public:

  static const char * catalogLabel( void ) {
    return "DT Monitor Plots";	    
  }	
    
  DTMDisplayPlugins( IgState *state );

  virtual bool applies( DisplayData *data );

  virtual std::string preDraw( DisplayData *data );

  virtual void postDraw( DisplayData *data );

 private:

  std::string name;

  DTDigiDisplayPlugins DigiME;
  DTDataIntegrityDisplayPlugins DataIntegrityME;
  DTtTrigTestDisplayPlugins tTrigTestME;
  DTLocalTriggerDisplayPlugins LocalTriggerME;
  DTLocalRecoDisplayPlugins LocalRecoME;
  DTLocalRecoTestDisplayPlugins LocalRecoTestME;

};

#endif //  DTMDisplayPlugins_H
