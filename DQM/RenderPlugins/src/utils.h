#ifndef dqm_utils_H
#define dqm_utils_H

// $Id: utils.h,v 1.2 2009/05/22 19:05:24 lat Exp $

/*!
  \file utils.h
  \version $Revision: 1.2 $
  \date $Date: 2009/05/22 19:05:24 $
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
