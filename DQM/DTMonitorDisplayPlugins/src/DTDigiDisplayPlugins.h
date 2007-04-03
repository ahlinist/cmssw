// $Id: DTDigiDisplayPlugins.h,v 1.1 2007/03/29 09:08:57 gmasetti Exp $

#ifndef  DTDigiDisplayPlugins_H
# define DTDigiDisplayPlugins_H

/*!
  \file DTDigiDisplayPlugins
  \brief Display Plugin for Digi Task Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.1 $
  \date $Date: 2007/03/29 09:08:57 $
*/

# include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
# include <string>
# include <TH2C.h>


class DTDigiDisplayPlugins {

public:
  
  DTDigiDisplayPlugins ();
  
  std::string preDraw (VisDQMDisplayPlugin::DisplayData *data);

  void postDraw( VisDQMDisplayPlugin::DisplayData *data );

  bool isDigiME (std::string name);

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

#endif //  DTDigiDisplayPlugins_H
