#include <stdexcept>
#include <string>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/ESCondDB/interface/ODESFEPedestalOffsetsDat.h"

using namespace std;
using namespace oracle::occi;

ODESFEPedestalOffsetsDat::ODESFEPedestalOffsetsDat()
{
  m_env = NULL;
  m_conn = NULL;
  m_writeStmt = NULL;
  m_readStmt = NULL;
  
  m_ID= 0;
  m_fed_id= 0;
  m_fiber_id= 0;
  m_kchip_id= 0;
  m_pace_id= 0;
  m_strip_id= 0;
  m_rx_id= 0;
  m_gain= 0;
  m_pedestal= 0;
  m_masked= 0;
  m_zs= 0;
  m_cm_masked= 0;
  m_cm_range= 0;  
}
 
/******************************/

ODESFEPedestalOffsetsDat::~ODESFEPedestalOffsetsDat()
{ 
}

/******************************/

void ODESFEPedestalOffsetsDat::prepareWrite()
  throw(runtime_error)
{
  this->checkConnection();

  try {
    m_writeStmt = m_conn->createStatement();
    m_writeStmt->setSQL("INSERT INTO "+getTable()+" (rec_id, fed_id, fiber_id, kchip_id, pace_id, strip_id, rx_id, gain, pedestal, masked, zs, cm_masked, cm_range) "
			"VALUES (:1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12, :13 )");
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetsDat::prepareWrite():  "+e.getMessage()));
  }
}

/******************************/

void ODESFEPedestalOffsetsDat::writeDB(const ODESFEPedestalOffsetsDat* item, ODESFEPedestalOffsetInfo* iov )
  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();
  
  try {
    m_writeStmt->setInt(1, item->getId());
    m_writeStmt->setInt(2, item->getFedId() );
    m_writeStmt->setInt(3, item->getFiberId());
    m_writeStmt->setInt(4, item->getKchipId() );
    m_writeStmt->setInt(5, item->getPaceId() );
    m_writeStmt->setInt(6, item->getStripId() );
    m_writeStmt->setInt(7, item->getRxId() );
    m_writeStmt->setInt(8, item->getGain() );
    m_writeStmt->setInt(9, item->getPedestal() );
    m_writeStmt->setInt(10, item->getMasked() );
    m_writeStmt->setInt(11, item->getZs() );
    m_writeStmt->setInt(12, item->getCmMasked() );
    m_writeStmt->setInt(13, item->getCmRange() );
    
    m_writeStmt->executeUpdate();
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetsDat::writeDB():  "+e.getMessage()));
  }
}

//  ************************************************************************   // 

void ODESFEPedestalOffsetsDat::fetchData(std::vector< ODESFEPedestalOffsetsDat >* p, ODESFEPedestalOffsetInfo* iov)
  throw(std::runtime_error)
{

  cout << "	fetchCurData Get data" << endl;
  this->checkConnection();
  
  iov->setConnection(m_env, m_conn); 
 
  int iovID = iov->fetchID();
  if (!iovID) { 
    std::cout <<"ID not in the DB"<< endl; 
    return;
  }

  int rp=iov->getRunIov_P();
  int rm=iov->getRunIov_M();
  cout << "RPlus=" <<rp << " RMinus=" << rm << endl;
  cout << "Fetch IOV=" << iovID << endl;

  try {
  m_readStmt->setSQL("select distinct :1 REC_ID,v.sourcefed FED_ID, v.fibernb FIBER_ID,v.kchipid KCHIP_ID,ascii(substr(v.module_type,2,1))-96 PACE_ID,to_number(substr(to_char(p.logic_id),9,2)) STRIP_ID,v.rxid RX_ID,0 GAIN, to_number(p.ped_mean) PEDESTAL,0 MASKED,0 ZS,0 CM_MASKED,0 CM_RANGE, to_number(p.ped_rms) RMS from cms_es_conf.es_fe_conf_info a, cms_es_conf.viewall v, cms_es_cond.es_mon_run_iov b inner join cms_es_cond.es_mon_run_dat r on r.iov_id=b.iov_id inner join cms_es_cond.es_mon_pedestals_dat p on p.iov_id= b.iov_id inner join cms_es_cond.channelview c on c.logic_id= p.logic_id  where  v.x=c.id2 and v.y=c.id3 and c.name=:2 and v.tagid=:3 and ((((v.plane=:4 and v.zside=:5 and to_char(c.logic_id) like :6) or (v.plane=:7 and v.zside=:8 and to_char(c.logic_id) like :9)) and r.logic_id=:10 and b.run_iov_id=:11) or ((((v.plane=:12 and v.zside=:13 and to_char(c.logic_id) like :14) or (v.plane=:15 and v.zside=:16 and to_char(c.logic_id) like :17)) and r.logic_id=:18 and b.run_iov_id=:19))) order by FED_ID,KCHIP_ID");
  m_readStmt->setInt(1,iovID);
  m_readStmt->setString(2,"ES_readout_strip");
  m_readStmt->setInt(3,1);

  m_readStmt->setInt(4,2);
  m_readStmt->setInt(5,1);
  m_readStmt->setString(6,"1031%");
  m_readStmt->setInt(7,1);
  m_readStmt->setInt(8,1);
  m_readStmt->setString(9,"1032%");
  m_readStmt->setInt(10,1100000000);
  m_readStmt->setInt(11,rp);

  m_readStmt->setInt(12,2);
  m_readStmt->setInt(13,-1);
  m_readStmt->setString(14,"1033%");
  m_readStmt->setInt(15,1);
  m_readStmt->setInt(16,-1);
  m_readStmt->setString(17,"1034%");
  m_readStmt->setInt(18,1200000000);
  m_readStmt->setInt(19,rm);

  ResultSet* rset = m_readStmt->executeQuery();
  ODESFEPedestalOffsetsDat dat;
  while(rset->next()) {
      dat.setId( rset->getInt(1) );
      dat.setFedId( rset->getInt(2) );
      dat.setFiberId( rset->getInt(3) );      
      dat.setKchipId( rset->getInt(4) );
      dat.setPaceId( rset->getInt(5) );
      dat.setStripId( rset->getInt(6) );
      dat.setRxId( rset->getInt(7) );
      dat.setGain( rset->getInt(8) );
      dat.setPedestal( rset->getInt(9) );
      dat.setMasked( rset->getInt(10) );
      dat.setZs( rset->getInt(11) );
      dat.setCmMasked( rset->getInt(12) );
      dat.setCmRange( rset->getInt(13) );
      dat.setRms( rset->getInt(14) );
      p->push_back( dat);
    }
  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetsDat::fetchData():  "+e.getMessage()));
  }
  cout << "End Query" << endl;
}

//  ************************************************************************   // 

void ODESFEPedestalOffsetsDat::writeArrayDB(const std::vector< ODESFEPedestalOffsetsDat > data, ODESFEPedestalOffsetInfo* iov)
    throw(std::runtime_error)
{
  this->checkConnection();

  int iovID = iov->fetchID();
  if (!iovID) { throw(runtime_error("ODFEPedestalOffsetDat::writeArrayDB:  ODFEPedestalOffsetInfo not in DB")); }

  int nrows=data.size(); 
  int* ids= new int[nrows];
  int* xx= new int[nrows];
  int* yy= new int[nrows];
  int* zz= new int[nrows];
  int* st= new int[nrows];
  int* xx1= new int[nrows];
  int* yy1= new int[nrows];
  int* zz1= new int[nrows];
  int* zz2= new int[nrows];
  int* zz3= new int[nrows];
  int* zz4= new int[nrows];
  int* zz5= new int[nrows];
  int* zz6= new int[nrows];    


  ub2* ids_len= new ub2[nrows];
  ub2* x_len= new ub2[nrows];
  ub2* y_len= new ub2[nrows];
  ub2* z_len= new ub2[nrows];
  ub2* st_len= new ub2[nrows];
  ub2* x1_len= new ub2[nrows];
  ub2* y1_len= new ub2[nrows];
  ub2* z1_len= new ub2[nrows];
  ub2* z2_len= new ub2[nrows];
  ub2* z3_len= new ub2[nrows];
  ub2* z4_len= new ub2[nrows];
  ub2* z5_len= new ub2[nrows];
  ub2* z6_len= new ub2[nrows];

  ODESFEPedestalOffsetsDat dataitem;
  
  int n_data= (int) data.size();
  for (int count = 0; count <n_data ; count++) {
    dataitem=data[count];
    ids[count]=iovID;
    xx[count]=dataitem.getFedId();
    yy[count]=dataitem.getFiberId();
    zz[count]=dataitem.getKchipId();
    st[count]=dataitem.getPaceId();
    xx1[count]=dataitem.getStripId();
    yy1[count]=dataitem.getRxId();
    zz1[count]=dataitem.getGain();
    zz2[count]=dataitem.getPedestal();
    zz3[count]=dataitem.getMasked();
    zz4[count]=dataitem.getZs();
    zz5[count]=dataitem.getCmMasked();
    zz6[count]=dataitem.getCmRange();


    ids_len[count]=sizeof(ids[count]);
    x_len[count]=sizeof(xx[count]);
    y_len[count]=sizeof(yy[count]);
    z_len[count]=sizeof(zz[count]);
    st_len[count]=sizeof(st[count]);
    x1_len[count]=sizeof(xx1[count]);
    y1_len[count]=sizeof(yy1[count]);
    z1_len[count]=sizeof(zz1[count]);
    z2_len[count]=sizeof(zz1[count]);
    z3_len[count]=sizeof(zz1[count]);
    z4_len[count]=sizeof(zz1[count]);
    z5_len[count]=sizeof(zz1[count]);
    z6_len[count]=sizeof(zz1[count]);
  }

  try {
    m_writeStmt->setDataBuffer(1, (dvoid*)ids, OCCIINT, sizeof(ids[0]),ids_len);
    m_writeStmt->setDataBuffer(2, (dvoid*)xx, OCCIINT , sizeof(xx[0]), x_len );
    m_writeStmt->setDataBuffer(3, (dvoid*)yy, OCCIINT , sizeof(yy[0]), y_len );
    m_writeStmt->setDataBuffer(4, (dvoid*)zz, OCCIINT , sizeof(zz[0]), z_len );
    m_writeStmt->setDataBuffer(5, (dvoid*)st, OCCIINT , sizeof(st[0]), st_len );
    m_writeStmt->setDataBuffer(6, (dvoid*)xx1, OCCIINT , sizeof(xx1[0]), x1_len );
    m_writeStmt->setDataBuffer(7, (dvoid*)yy1, OCCIINT , sizeof(yy1[0]), y1_len );
    m_writeStmt->setDataBuffer(8, (dvoid*)zz1, OCCIINT , sizeof(zz1[0]), z1_len );
    m_writeStmt->setDataBuffer(9, (dvoid*)zz2, OCCIINT , sizeof(zz2[0]), z2_len );
    m_writeStmt->setDataBuffer(10, (dvoid*)zz3, OCCIINT , sizeof(zz3[0]), z3_len );
    m_writeStmt->setDataBuffer(11, (dvoid*)zz4, OCCIINT , sizeof(zz4[0]), z4_len );
    m_writeStmt->setDataBuffer(12, (dvoid*)zz5, OCCIINT , sizeof(zz5[0]), z5_len );
    m_writeStmt->setDataBuffer(13, (dvoid*)zz6, OCCIINT , sizeof(zz6[0]), z6_len );
    
    m_writeStmt->executeArrayUpdate(nrows);

    delete [] ids;
    delete [] xx;
    delete [] yy;
    delete [] zz;
    delete [] st;
    delete [] xx1;
    delete [] yy1;
    delete [] zz1;
    delete [] zz2;
    delete [] zz3;
    delete [] zz4;
    delete [] zz5;
    delete [] zz6;
    
    delete [] ids_len;
    delete [] x_len;
    delete [] y_len;
    delete [] z_len;
    delete [] st_len;
    delete [] x1_len;
    delete [] y1_len;
    delete [] z1_len;
    delete [] z2_len;
    delete [] z3_len;
    delete [] z4_len;
    delete [] z5_len;
    delete [] z6_len;

  } catch (SQLException &e) {
    throw(runtime_error("ODESFEPedestalOffsetsDat::writeArrayDB():  "+e.getMessage()));
  }
}
