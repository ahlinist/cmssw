#ifndef dqm_utils_H
#define dqm_utils_H

// $Id: utils.h,v 1.3 2009/11/09 11:05:21 emanuele Exp $

/*!
  \file utils.h
  \version $Revision: 1.3 $
  \date $Date: 2009/11/09 11:05:21 $
*/

class TH2;

namespace dqm
{
  class utils
  {
  public:
    static void reportSummaryMapPalette(TH2* obj);

  private:
    static bool init;
  };
}

#endif // dqm_utils_H
