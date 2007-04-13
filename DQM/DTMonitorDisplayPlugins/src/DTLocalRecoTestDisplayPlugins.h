#ifndef  DTLocalRecoTestDisplayPlugins_H
# define DTLocalRecoTestDisplayPlugins_H

/*!
  \file DTLocalRecoTestDisplayPlugins
  \brief Display Plugin for Local Reconstruction Client test Quality Histograms
  \author G. Mila
  \version $Revision: 1.1 $
  \date $Date: 2007/04/03 09:30:39 $
*/

# include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
# include <string>
# include <TH2C.h>


class DTLocalRecoTestDisplayPlugins {

public:
  
  DTLocalRecoTestDisplayPlugins ();
  
  std::string preDraw (VisDQMDisplayPlugin::DisplayData *data);

  void postDraw( VisDQMDisplayPlugin::DisplayData *data );

  bool isLocalRecoTestME (std::string name);

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

#endif //  DTLocalRecoTestDisplayPlugins_H
