// $Id: utils.h,v 1.23 2008/04/07 11:30:23 dellaric Exp $

/*!
  \file utils.h
  \version $Revision: 1.23 $
  \date $Date: 2008/04/07 11:30:23 $
*/

#ifndef dqm_utils_H
#define dqm_utils_H

class TH2F;

namespace dqm {

class utils {

 public:

  static void reportSummaryMapPalette(TH2F* obj);

private:

  static bool init;

};

}
#endif // dqm_utils_H
