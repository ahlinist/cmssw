#include "CondTools/CSC/interface/CSCFakeDBNoiseMatrixHandler.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include<iostream>

#include "CondFormats/CSCObjects/interface/CSCDBNoiseMatrix.h"
#include "CondTools/CSC/interface/CSCFakeDBNoiseMatrix.h"

popcon::CSCFakeDBNoiseMatrixImpl::CSCFakeDBNoiseMatrixImpl(const edm::ParameterSet& pset): m_name(pset.getUntrackedParameter<std::string>("name","CSCFakeDBNoiseMatrixImpl"))
{}

popcon::CSCFakeDBNoiseMatrixImpl::~CSCFakeDBNoiseMatrixImpl()
{
}

void popcon::CSCFakeDBNoiseMatrixImpl::getNewObjects()
{

  std::cout << "------- CSC src - > getNewObjects\n"<<m_name;
  
  // fill object from file
  CSCDBNoiseMatrix * cnmatrix = CSCFakeDBNoiseMatrix::prefillDBNoiseMatrix();
  //std::cout << "matrix size " << cnmatrix->matrix.size() << std::endl;
  
  //check whats already inside of database
  
  std::cerr<<"got offlineInfo"<<std::endl;
  std::cerr << tagInfo().name << " , last object valid since " 
	    << tagInfo().lastInterval.first << std::endl; 
  
  unsigned int snc;
  
  std::cout << "NoiseMatrix source implementation test ::getNewObjects : enter since ? \n"<< std::endl;
  //std::cin >> snc;
  snc =1 ;
  
  m_to_transfer.push_back(std::make_pair(cnmatrix,snc));
  
  std::cout << "------- " << m_name << "CSC src - > getNewObjects -----------\n"<< std::endl;
}
