// $Id: EEMDisplayPlugins.h,v 1.2 2007/06/11 12:41:18 dellaric Exp $

#ifndef  EEMDisplayPlugins_H
# define EEMDisplayPlugins_H

/*!
  \file EEMDisplayPlugins
  \brief Display Plugin for Quality Histograms (2D)
  \author B. Gobbo 
  \version $Revision: 1.2 $
  \date $Date: 2007/06/11 12:41:18 $
*/

#include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
#include <string>
#include <TH2C.h>

class EEMDisplayPlugins : public VisDQMDisplayPlugin {

 public:

  static const char * catalogLabel( void ) {
    return "Ecal Barrel Monitor Plots";	    
  }	
    
  EEMDisplayPlugins( IgState *state );

  virtual bool applies( DisplayData *data );

  virtual std::string preDraw( DisplayData *data );

  virtual void postDraw( DisplayData *data );

 private:

  int nbx;
  int nby;

  std::string name;

  // Temporary workaround due to erroneous multiple instantiation of this class...
  //static bool first;
  //static TH2C* t1;
  //static TH2C* t2;
  //static TH2C* t3;
  //static TH2C* t4;
  //static TH2C* t5;

  TH2C* text1;
  TH2C* text2;
  TH2C* text3;
  TH2C* text4;
  TH2C* text5;
  TH2C* text6;

  int pCol3[6]; 
  int pCol4[10];
    
  // private functions...
  std::string preDrawTProfile2D( DisplayData *data );
  std::string preDrawTProfile( DisplayData *data );
  std::string preDrawTH2( DisplayData *data );
  std::string preDrawTH1( DisplayData *data );
  void postDrawTProfile2D( DisplayData *data );
  void postDrawTH2( DisplayData *data );
  template<class T> void adjustRange( T obj );

};

#endif //  EEMDisplayPlugins_H
