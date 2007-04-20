// $Id: DTLocalTriggerDisplayPlugins.h,v 1.1 2007/04/03 09:29:24 gmasetti Exp $

#ifndef  DTLocalTriggerDisplayPlugins_H
# define DTLocalTriggerDisplayPlugins_H

/*!
  \file DTLocalTriggerDisplayPlugins
  \brief Display Plugin for LocalTrigger Task Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.1 $
  \date $Date: 2007/04/03 09:29:24 $
*/

# include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
# include <string>
# include <TH2C.h>


class DTLocalTriggerDisplayPlugins {

public:
  
  DTLocalTriggerDisplayPlugins ();
  
  std::string preDraw (VisDQMDisplayPlugin::DisplayData *data);

  void postDraw( VisDQMDisplayPlugin::DisplayData *data );

  bool isLocalTriggerME (std::string name);

 private:

  std::string name;

  std::string preDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data );
  std::string preDrawTProfile( VisDQMDisplayPlugin::DisplayData *data );
  std::string preDrawTH2F (VisDQMDisplayPlugin::DisplayData *data);
  std::string preDrawTH1F (VisDQMDisplayPlugin::DisplayData *data);
  void postDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data );
  void postDrawTProfile( VisDQMDisplayPlugin::DisplayData *data );
  void postDrawTH2F( VisDQMDisplayPlugin::DisplayData *data );
  void postDrawTH1F( VisDQMDisplayPlugin::DisplayData *data );


};

#endif //  DTLocalTriggerDisplayPlugins_H
