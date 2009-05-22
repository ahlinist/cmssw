#ifndef dqm_utils_H
#define dqm_utils_H

// $Id: utils.h,v 1.1 2008/05/28 12:41:29 dellaric Exp $

/*!
  \file utils.h
  \version $Revision: 1.1 $
  \date $Date: 2008/05/28 12:41:29 $
*/

class TH2F;

namespace dqm
{
  class utils
  {
  public:
    static void reportSummaryMapPalette(TH2F* obj);

  private:
    static bool init;
  };
}

#endif // dqm_utils_H
