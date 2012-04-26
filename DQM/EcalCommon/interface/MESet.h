#ifndef MESet_H
#define MESet_H

#include <string>
#include <vector>

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DQM/EcalCommon/interface/EcalDQMBinningService.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/EcalDetId/interface/EcalElectronicsId.h"

typedef EcalDQMBinningService BinService; // prepare for expansion into more than one binning service

namespace ecaldqm
{
  struct MEData {
    std::string pathName;
    BinService::ObjectType otype;
    BinService::BinningType btype;
    MonitorElement::Kind kind;
    BinService::AxisSpecs *xaxis;
    BinService::AxisSpecs *yaxis;
    BinService::AxisSpecs *zaxis;
    MEData() :
      pathName(""), otype(BinService::nObjType), btype(BinService::nBinType), kind(MonitorElement::DQM_KIND_INVALID),
      xaxis(0), yaxis(0), zaxis(0)
    {}
    MEData(std::string const& _pathName, BinService::ObjectType _otype, BinService::BinningType _btype, MonitorElement::Kind _kind,
	   BinService::AxisSpecs const* _xaxis = 0, BinService::AxisSpecs const* _yaxis = 0, BinService::AxisSpecs const* _zaxis = 0) :
      pathName(_pathName), otype(_otype), btype(_btype), kind(_kind),
      xaxis(_xaxis ? new BinService::AxisSpecs(*_xaxis) : 0),
      yaxis(_yaxis ? new BinService::AxisSpecs(*_yaxis) : 0),
      zaxis(_zaxis ? new BinService::AxisSpecs(*_zaxis) : 0)
    {}
    MEData(MEData const& _orig) :
      pathName(_orig.pathName), otype(_orig.otype), btype(_orig.btype), kind(_orig.kind),
      xaxis(_orig.xaxis ? new BinService::AxisSpecs(*_orig.xaxis) : 0),
      yaxis(_orig.yaxis ? new BinService::AxisSpecs(*_orig.yaxis) : 0),
      zaxis(_orig.zaxis ? new BinService::AxisSpecs(*_orig.zaxis) : 0)
    {}
    ~MEData()
    {
      delete xaxis;
      delete yaxis;
      delete zaxis;
    }

    MEData& operator=(MEData const& _rhs)
    {
      pathName = _rhs.pathName;
      otype = _rhs.otype;
      btype = _rhs.btype;
      kind = _rhs.kind;
      xaxis = _rhs.xaxis ? new BinService::AxisSpecs(*_rhs.xaxis) : 0;
      yaxis = _rhs.yaxis ? new BinService::AxisSpecs(*_rhs.yaxis) : 0;
      zaxis = _rhs.zaxis ? new BinService::AxisSpecs(*_rhs.zaxis) : 0;
      return *this;
    }
  };

  class MESet {
  public :
    MESet(std::string const&, MEData const&, bool _readOnly = false);
    virtual ~MESet();

    virtual void book();
    virtual bool retrieve() const;
    virtual void clear() const;

    // default values are necessary (otherwise fill(DetId) will be interpreted as fill(uint32_t)!!)
    virtual void fill(DetId const&, float _wx = 1., float _wy = 1., float _w = 1.);
    virtual void fill(EcalElectronicsId const&, float _wx = 1., float _wy = 1., float _w = 1.);
    virtual void fill(unsigned, float _wx = 1., float _wy = 1., float _w = 1.);
    virtual void fill(float, float _wy = 1., float _w = 1.);

    virtual void setBinContent(DetId const&, float, float _err = 0.);
    virtual void setBinContent(EcalElectronicsId const&, float, float _err = 0.);
    virtual void setBinContent(unsigned, float, float _err = 0.);

    virtual void setBinEntries(DetId const&, float);
    virtual void setBinEntries(EcalElectronicsId const&, float);
    virtual void setBinEntries(unsigned, float);

    virtual float getBinContent(DetId const&, int _bin = 0) const;
    virtual float getBinContent(EcalElectronicsId const&, int _bin = 0) const;
    virtual float getBinContent(unsigned, int _bin = 0) const;

    virtual float getBinError(DetId const&, int _bin = 0) const;
    virtual float getBinError(EcalElectronicsId const&, int _bin = 0) const;
    virtual float getBinError(unsigned, int _bin = 0) const;

    virtual float getBinEntries(DetId const&, int _bin = 0) const;
    virtual float getBinEntries(EcalElectronicsId const&, int _bin = 0) const;
    virtual float getBinEntries(unsigned, int _bin = 0) const;

    virtual void setAxisTitle(std::string const&, int _axis = 1);
    virtual void setBinLabel(unsigned, int, std::string const&, int _axis = 1);

    virtual void reset(float _content = 0., float _err = 0., float _entries = 0.);
    virtual void resetAll(float _content = 0., float _err = 0., float _entries = 0.);

    std::string const& getDir() const { return dir_; }
    void setDir(std::string const& _dir) { dir_ = _dir; }
    std::string const& getName() const { return name_; }
    void setName(std::string const& _name) { name_ = _name; }
    void name(std::map<std::string, std::string> const&) const;
    BinService::ObjectType getObjType() const { return data_->otype; }
    BinService::BinningType getBinType() const { return data_->btype; }
    bool isActive() const { return active_; }

    virtual MonitorElement const* getME(unsigned _offset) const { return (_offset < mes_.size() ? mes_[_offset] : 0); }

  protected:
    virtual void fill_(unsigned, int, float);
    virtual void setBinContent_(unsigned, int, float, float);
    virtual void setBinEntries_(unsigned, int, float);
    virtual float getBinContent_(unsigned, int) const;
    virtual float getBinError_(unsigned, int) const;
    virtual float getBinEntries_(unsigned, int) const;

    static BinService const* binService_;
    static DQMStore* dqmStore_;

    mutable std::vector<MonitorElement*> mes_;

    mutable std::string dir_;
    mutable std::string name_;
    MEData const* data_;

    mutable bool active_;
    bool readOnly_;
  };

}

#endif
