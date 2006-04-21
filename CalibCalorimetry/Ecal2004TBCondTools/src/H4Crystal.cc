/*!
 \class H4Crystal
 \brief Class which is able to read in crystal info from the
 MySQL database. This class is written out to a root-database
 by the H4DBManager class.

 $Date: 2003/09/24 09:10:18 $
 $Author: nrl $

 The index and subindex which you give to H4DBManager::update have the
 following meaning: The index denotes the database in MySQL/the module
 you are interested in:
  -# H4
  -# M0prime
  -# M0prime_40202
  -# lab11
  -# proto2001
  -# proto2002
  -# proto2003
  -# prototest
  -# simuM0

 The subindex is the linear number of the crystal you are interested 
 in.

 The interface for the management by H4DBManager is inherited form
 H4RootDBEntry.
*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Crystal.h"

ClassImp(H4Crystal)

#include "TSQLResult.h"
#include "TSQLRow.h"
#include <stdlib.h>
#include <string>
#include <strstream>
#include <iostream>

using namespace std;

const char *H4Crystal::tableNames[]={
  "Unknown", "H4", "M0prime", "M0prime_40202", "lab11", "proto2001",
  "proto2002", "proto2003", "prototest", "simuM0"
};


H4Crystal::H4Crystal(){
  matchMethod=exact;
  valid=false;
}

int H4Crystal::getIndex() const{
  return table;
}

const char * H4Crystal::getTableName() const{
  return tableNames[table];
}

int H4Crystal::getSubIndex() const{
  return linCh;
}

/*
  mysql> describe proto2002;
  +---------+----------------------+------+-----+---------+----------------+
  | Field   | Type                 | Null | Key | Default | Extra          |
  +---------+----------------------+------+-----+---------+----------------+
  | lin_ch  | smallint(5) unsigned | YES  |     | NULL    |                |
  | eta     | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | phi     | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | sm_id   | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | sm_ch   | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | rose_id | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | rose_ch | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | hv_id   | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | hv_ch   | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | lv_id   | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | lv_ch   | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | ID      | smallint(6)          |      | PRI | NULL    | auto_increment |
  +---------+----------------------+------+-----+---------+----------------+
  12 rows in set (0.00 sec)
  
  mysql> describe proto2003;
  +--------+----------------------+------+-----+---------+----------------+
  | Field  | Type                 | Null | Key | Default | Extra          |
  +--------+----------------------+------+-----+---------+----------------+
  | lin_ch | smallint(5) unsigned | YES  |     | NULL    |                |
  | eta    | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | phi    | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | sm_id  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | sm_ch  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | FE_id  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | FE_st  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | FE_ch  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | FE_lin | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | hv_id  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | hv_ch  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | lv_id  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | lv_ch  | tinyint(3) unsigned  | YES  |     | NULL    |                |
  | ID     | smallint(6)          |      | PRI | NULL    | auto_increment |
  +--------+----------------------+------+-----+---------+----------------+
*/

#ifdef WITH_MYSQL
int H4Crystal::fillFromMySQL(TSQLServer *db, int tab, int crystal,
			     IndexMatch match)
{

  if(db->SelectDataBase("mappings")){
    std::cerr << "Could not open mappings DB." << std::endl;
  }

  table=tab;

  // Compose query to MySQL database:
  ostrstream query;

  if(string(getTableName())=="proto2003"){
    query <<"select lin_ch, eta, phi, sm_id, sm_ch, FE_id, FE_ch, "
	  <<"hv_id, hv_ch, lv_id, lv_ch, ID from " 
	  << getTableName() << " where lin_ch = "
	  << crystal << ";" << '\0';
  }else{
    query <<"select * from " << getTableName() << " where lin_ch = "
	  << crystal << ";" << '\0';
  }
#ifdef DEBUG
  std::cout << "Using query string " << query.str() << std::endl;
#endif

  TSQLResult *res = db->Query( query.str() ); 
  int nrows   = res->GetRowCount();
  int nfields = res->GetFieldCount();

  if (nfields!=12) {
    std::cout << "H4Crystal::fillFromMySQL: Unkown table format!" << std::endl;
    std::cout << "Fields: " << nfields<< std::endl;
    abort();
  }

  if(nrows < 1){ // No valid entry found
    valid=false;
    return -1;
  }
  if(nrows >1){ // More than one entry found???
    valid=false;
    return nrows;
  }

  TSQLRow *row = res->Next();
  
  std::cout << "Field 0: " << row->GetField(0) << std::endl;

  linCh=(atoi(row->GetField(0)));

  eta=0;
  if(row->GetField(1))
    eta=(atoi(row->GetField(1)));

  phi=0;
  if(row->GetField(2))
    phi=(atoi(row->GetField(2)));

  smID=0;
  if(row->GetField(3))
    smID=(atoi(row->GetField(3)));

  smCh=0;
  if(row->GetField(4))
   smCh=(atoi(row->GetField(4)));

  roseID=0;
  if(row->GetField(5))
    roseID=(atoi(row->GetField(5)));

  roseCh=0;
  if(row->GetField(6))
    roseCh=(atoi(row->GetField(6)));

  hvID=0;
  if(row->GetField(7))
    hvID=(atoi(row->GetField(7)));

  hvCh=0;
  if(row->GetField(8))
    hvCh=(atoi(row->GetField(8)));

  lvID=0;
  if(row->GetField(9))
    lvID=(atoi(row->GetField(9)));
  
  lvCh=0;
  if(row->GetField(10))
    lvCh=(atoi(row->GetField(10)));

  delete row;
  delete res;
  valid = true;
  return nrows;
}
#endif

void H4Crystal::print(Option_t *option) const{
  std::cout << getTableName() << " crystal " << linCh << ":  eta: " << eta 
	    << "  phi: " << phi << std::endl;
}  
