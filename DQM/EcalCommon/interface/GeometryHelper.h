#ifndef DQM_ECALCOMMON_GeomHelperFunctions_H
#define DQM_ECALCOMMON_GeomHelperFunctions_H

/*!
  \file GeomHelperFunctions.h
  \brief Ecal Monitor Utility functions
  \author Yutaro Iiyama
  \version $Revision: 1.0 $
  \date $Date: 2011/08/05 02:37:00 $
*/

#include <string>
#include <map>

#include "TClass.h"

class DQMStore;
class MonitorElement;
class EEDetId;
class EcalScDetId;
class EcalTrigTowerDetId;

namespace ecaldqm {

  enum ObjectType {
    kFullEE,
    kEEp,
    kEEm,
    kEEpFar, kEEpNear,
    kEEmFar, kEEmNear,
    kSM
  };

  enum BinningType {
    kCrystal,
    kSuperCrystal,
    kTriggerTower
  };

  struct MeInfo {
    TClass *isa;
    ObjectType otype;
    BinningType btype;
    int ism;
  };

  class MeInfoMap {

  public:
    static void set(MonitorElement *me, TClass *cl, ObjectType otype, BinningType btype, int ism);
    static const MeInfo *get(MonitorElement *me);

  private:
    static std::map<std::string, MeInfo> infos;
  };

  // dqmStore must be cd'ed to the desired directory before passing
  MonitorElement *bookME(DQMStore *dqmStore, const std::string &name, const std::string &title, const std::string &className, ObjectType otype, BinningType btype = kCrystal, int ism = 0, double lowZ = 0., double highZ = 0., const char *option = "s");

  void fillME(MonitorElement *me, const EEDetId &id, double wz = 1., double wprof = 1.);
  void fillME(MonitorElement *me, const EcalScDetId &id, double wz = 1., double wprof = 1.);
  void fillME(MonitorElement *me, const EcalTrigTowerDetId &id, double wz = 1., double wprof = 1.);

}

#endif
