// $Id: DTMDisplayPlugins.h,v 1.1 2007/03/29 07:20:09 $

#ifndef  DTMDisplayPlugins_H
# define DTMDisplayPlugins_H

/*!
  \file DTMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.1 $
  \date $Date: 2007/03/29 07:20:09 $
*/

# include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
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

  std::string preDrawTH2F( DisplayData *data );
  std::string preDrawTH1F( DisplayData *data );

};

#endif //  DTMDisplayPlugins_H
