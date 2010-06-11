#include "TauAnalysis/FittingTools/plugins/UnitTestModule.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/FittingTools/interface/InterpolGridTable.h"
#include "TauAnalysis/FittingTools/interface/InterpolGridVector.h"

#include <iostream>

UnitTestModule::UnitTestModule(const edm::ParameterSet& cfg)
{
  std::cout << "<UnitTestModule::UnitTestModule>:" << std::endl;
}

UnitTestModule::~UnitTestModule()
{
  std::cout << "<UnitTestModule::~UnitTestModule>:" << std::endl;
}

void UnitTestModule::endJob()
{
  std::cout << "<UnitTestModule::endJob>:" << std::endl;

  InterpolGridTable* interpolGridTable = new InterpolGridTable();
  interpolGridTable->loadXML("AHbb_xSection.xml");
  interpolGridTable->print(std::cout);
  std::cout << "interpolated value(x = 1.25, y = 1.25) = " << interpolGridTable->getValue(1.25, 1.25) << std::endl;
  std::cout << "interpolated value(x = 1.00, y = 1.75) = " << interpolGridTable->getValue(1.00, 1.75) << std::endl;
  std::cout << "interpolated value(x = 1.00, y = 2.00) = " << interpolGridTable->getValue(1.00, 2.00) << std::endl;
  std::cout << "interpolated value(x = 1.50, y = 2.00) = " << interpolGridTable->getValue(1.50, 2.00) << std::endl;
  std::cout << "interpolated value(x = 1.50, y = 2.25) = " << interpolGridTable->getValue(1.50, 2.25) << std::endl;
  std::cout << "interpolated value(x = 0.50, y = 0.50) = " << interpolGridTable->getValue(0.50, 0.50) << std::endl;
  delete interpolGridTable;

  InterpolGridVector* interpolGridVector = new InterpolGridVector();
  interpolGridVector->loadXML("AHbb_acceptance.xml");
  interpolGridVector->print(std::cout);
  std::cout << "interpolated value(x = 0.50) = " << interpolGridVector->getValue(0.50) << std::endl;
  std::cout << "interpolated value(x = 1.00) = " << interpolGridVector->getValue(1.00) << std::endl;
  std::cout << "interpolated value(x = 1.75) = " << interpolGridVector->getValue(1.75) << std::endl;
  std::cout << "interpolated value(x = 2.00) = " << interpolGridVector->getValue(2.00) << std::endl;
  std::cout << "interpolated value(x = 2.25) = " << interpolGridVector->getValue(2.25) << std::endl;
  delete interpolGridVector;

  std::cout << "done." << std::endl;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(UnitTestModule);
