// $Id: DTMDisplayPlugins.h,v 1.2 2007/04/03 09:27:27 gmasetti Exp $

#ifndef  DTMDisplayPlugins_H
# define DTMDisplayPlugins_H

/*!
  \file DTMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.2 $
  \date $Date: 2007/04/03 09:27:27 $
*/

# include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTDigiDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTDataIntegrityDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTtTrigTestDisplayPlugins.h"
# include "DQM/DTMonitorDisplayPlugins/src/DTLocalRecoDisplayPlugins.h"
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
  DTLocalRecoDisplayPlugins LocalRecoME;

};

#endif //  DTMDisplayPlugins_H
