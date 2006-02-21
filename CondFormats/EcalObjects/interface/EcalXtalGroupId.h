#ifndef CondFormats_EcalObjects_EcalXtalGroupId_H
#define CondFormats_EcalObjects_EcalXtalGroupId_H


class EcalXtalGroupId {
  public:
    EcalXtalGroupId();
    EcalXtalGroupId(const unsigned int& id);
    virtual ~EcalXtalGroupId();
    bool operator>(const EcalXtalGroupId& rhs) const{ return ( id_>rhs.id() ); }
    bool operator>=(const EcalXtalGroupId& rhs) const { return ( id_>=rhs.id() ); }
    bool operator==(const EcalXtalGroupId& rhs) const { return ( id_==rhs.id() ); }
    bool operator<(const EcalXtalGroupId& rhs) const { return ( id_<rhs.id() ); }
    bool operator<=(const EcalXtalGroupId& rhs) const { return ( id_<=rhs.id() ); }

    const unsigned int id() const { return id_; }
  private:
    unsigned int id_;

};
#endif
