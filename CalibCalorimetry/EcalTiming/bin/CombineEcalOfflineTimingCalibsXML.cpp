#include "CondFormats/EcalObjects/interface/EcalTimeCalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalTimeCalibErrors.h"
#include "CondTools/Ecal/interface/EcalTimeCalibConstantsXMLTranslator.h"
#include "CondTools/Ecal/interface/EcalTimeCalibErrorsXMLTranslator.h"
#include "CondTools/Ecal/interface/EcalCondHeader.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include <iostream>

void usage()
{
  std::cout
    << "Usage: CombineEcalOfflineTimingCalibsXML [originalCalibsFile] [originalCalibErrorsFile] [fileWithChanges] [fileWithChangesErrors]"
    << std::endl
    << "\tThe values in fileWithChanges will be added to those in originalCalibsFile."
    << " If only one file contains a good calibration, that file's calibration will be used."
    << std::endl;
}

// ****************************************************************
int main(int argc, char* argv[])
{
  //
  // Binary to combine 2 sets of timing calibrations, using original as the baseline
  // Seth Cooper
  // November 8th 2010
  // Usage: ShiftEcalOfflineTimingCalibsXML <originalCalibsFile> <originalCalibErrorsFile> <fileWithChanges> <fileWithChangesErrors>
  //   The calibs in the file to shift will be (intelligently) added to those in the original file.
  //

  using namespace std;

  char* calibFile = argv[1];
  if (!calibFile)
  {
    cout << "Error: Missing input file." << endl;
    usage();
    return -1;
  }
  char* calibErrorFile = argv[2];
  if (!calibErrorFile)
  {
    cout << "Error: Missing input file." << endl;
    usage();
    return -1;
  }

  char* shiftFile = argv[3];
  if (!shiftFile)
  {
    cout << "Error: Missing input file." << endl;
    usage();
    return -1;
  }
  char* shiftErrorFile = argv[4];
  if (!shiftErrorFile)
  {
    cout << "Error: Missing input file." << endl;
    usage();
    return -1;
  }

  EcalCondHeader calibFileHeader;
  EcalCondHeader calibErrorFileHeader;
  EcalCondHeader shiftFileHeader;
  EcalCondHeader shiftErrorFileHeader;

  string calibFileStr(calibFile);
  string calibErrorFileStr(calibErrorFile);
  string shiftFileStr(shiftFile);
  string shiftErrorFileStr(shiftErrorFile);

  EcalTimeCalibConstants calibConstants;
  EcalTimeCalibErrors calibErrors;
  EcalTimeCalibConstants shiftConstants;
  EcalTimeCalibErrors shiftErrors;

  // Populate the EcalTimeCalibConstants objects by reading in the files
  int ret = EcalTimeCalibConstantsXMLTranslator::readXML(calibFileStr,calibFileHeader,calibConstants);
  if(ret)
  {
    cout << "Error reading calibration XML file" << endl;
    return -2;
  }
  ret = EcalTimeCalibConstantsXMLTranslator::readXML(calibErrorFileStr,calibErrorFileHeader,calibErrors);
  if(ret)
  {
    cout << "Error reading calib error XML file" << endl;
    return -2;
  }
  ret = EcalTimeCalibConstantsXMLTranslator::readXML(shiftFileStr,shiftFileHeader,shiftConstants);
  if(ret)
  {
    cout << "Error reading shift XML file" << endl;
    return -2;
  }
  ret = EcalTimeCalibConstantsXMLTranslator::readXML(shiftErrorFileStr,shiftErrorFileHeader,shiftErrors);
  if(ret)
  {
    cout << "Error reading shift error XML file" << endl;
    return -2;
  }

  float EBavgShift = 0;
  int numEBcrys = 0;
  for(int hash = 0; hash < EBDetId::MAX_HASH; ++hash)
  {
    EBDetId thisDet = EBDetId::unhashIndex(hash);
    if(thisDet==EBDetId())
      continue;

    if(shiftErrors[thisDet.rawId()]!=999)
    {
      EBavgShift+=shiftConstants[thisDet.rawId()];
      ++numEBcrys;
    }
  }
  EBavgShift/=numEBcrys;

  float EEavgShift = 0;
  int numEEcrys = 0;
  for(int hash = 0; hash < EEDetId::kSizeForDenseIndexing; ++hash)
  {
    EEDetId thisDet = EEDetId::unhashIndex(hash);
    if(thisDet==EEDetId())
      continue;

    if(shiftErrors[thisDet.rawId()]!=999)
    {
      EEavgShift+=shiftConstants[thisDet.rawId()];
      ++numEEcrys;
    }
  }
  EEavgShift/=numEEcrys;

  // Loop over the calib constants and apply any necessary shifts
  for(int hash = 0; hash < EBDetId::MAX_HASH; ++hash)
  {
    EBDetId thisDet = EBDetId::unhashIndex(hash);
    if(thisDet==EBDetId())
      continue;

    bool origCalibFound = true;
    bool shiftCalibFound = true;

    if(calibConstants.find(thisDet.rawId())==calibConstants.end())
      origCalibFound = false;
    if(calibConstants[thisDet.rawId()]==0)
      origCalibFound = false;
    if(shiftConstants.find(thisDet.rawId())==shiftConstants.end())
      shiftCalibFound = false;
    if(shiftConstants[thisDet.rawId()]==0)
      shiftCalibFound = false;
    if(calibErrors[thisDet.rawId()]==999)
      origCalibFound = false;
    if(shiftErrors[thisDet.rawId()]==999)
      shiftCalibFound = false;

    float newCalib = 0;
    float newCalibError = 0;
    if(origCalibFound && shiftCalibFound) // Calib found and there is one to shift as well. Shift it.
    {
      newCalib = calibConstants[thisDet.rawId()]+shiftConstants[thisDet.rawId()];
      newCalibError = sqrt(pow(calibErrors[thisDet.rawId()],2)+pow(shiftErrors[thisDet.rawId()],2));
    }
    if(origCalibFound && !shiftCalibFound) // good original, but no shifted calib -- use original + new avg shift
    {
      newCalib = calibConstants[thisDet.rawId()] + EBavgShift;
      newCalibError = calibErrors[thisDet.rawId()];
    }
    if(!origCalibFound && shiftCalibFound) // good shifted calib, but no original calib -- use new shifted calib
    {
      newCalib = shiftConstants[thisDet.rawId()];
      newCalibError = shiftErrors[thisDet.rawId()];
    }
    if(!origCalibFound && !shiftCalibFound) // if neither has a good one, use the (new) average shift - global shift
    {
      newCalib = EBavgShift - 2.0833;
      newCalibError = 999; // set error to 999 to indicate avg calib used
    }

    //cout << "Crystal " << thisDet << " calibration now " << newCalib
    //  << " = " << calibConstants[thisDet.rawId()]
    //  << " + " << shiftConstants[thisDet.rawId()]
    //  << endl;
    calibConstants[thisDet.rawId()] = newCalib;
    calibErrors[thisDet.rawId()] = newCalibError;
  }
  for(int hash = 0; hash < EEDetId::kSizeForDenseIndexing; ++hash)
  {
    EEDetId thisDet = EEDetId::unhashIndex(hash);
    if(thisDet==EEDetId())
      continue;

    bool origCalibFound = true;
    bool shiftCalibFound = true;

    if(calibConstants.find(thisDet.rawId())==calibConstants.end())
      origCalibFound = false;
    if(calibConstants[thisDet.rawId()]==0)
      origCalibFound = false;
    if(shiftConstants.find(thisDet.rawId())==shiftConstants.end())
      shiftCalibFound = false;
    if(shiftConstants[thisDet.rawId()]==0)
      shiftCalibFound = false;
    if(calibErrors[thisDet.rawId()]==999)
      origCalibFound = false;
    if(shiftErrors[thisDet.rawId()]==999)
      shiftCalibFound = false;

    float newCalib = 0;
    float newCalibError = 0;
    if(origCalibFound && shiftCalibFound) // Calib found and there is one to shift as well. Shift it.
    {
      newCalib = calibConstants[thisDet.rawId()]+shiftConstants[thisDet.rawId()];
      newCalibError = sqrt(pow(calibErrors[thisDet.rawId()],2)+pow(shiftErrors[thisDet.rawId()],2));
    }
    if(origCalibFound && !shiftCalibFound) // good original, but no shifted calib -- use original + new avg shift
    {
      newCalib = calibConstants[thisDet.rawId()] + EEavgShift;
      newCalibError = calibErrors[thisDet.rawId()];
    }
    if(!origCalibFound && shiftCalibFound) // good shifted calib, but no original calib -- use new shifted calib
    {
      newCalib = shiftConstants[thisDet.rawId()];
      newCalibError = shiftErrors[thisDet.rawId()];
    }
    if(!origCalibFound && !shiftCalibFound) // if neither has a good one, use the (new) average shift - global shift
    {
      newCalib = EEavgShift - 2.0833;
      newCalibError = 999; // set error to 999 to indicate avg calib used
    }

    //cout << "Crystal " << thisDet << " calibration now " << newCalib
    //  << " = " << calibConstants[thisDet.rawId()]
    //  << " + " << shiftConstants[thisDet.rawId()]
    //  << endl;
    calibConstants[thisDet.rawId()] = newCalib;
    calibErrors[thisDet.rawId()] = newCalibError;
  }

  // Write new XML file
  string outputFile = "ecalOfflineTimingCalibsCombined.xml";
  string outputErrorsFile = "ecalOfflineTimingErrorsCombined.xml";
  EcalTimeCalibConstantsXMLTranslator::writeXML(outputFile,calibFileHeader,calibConstants);
  EcalTimeCalibErrorsXMLTranslator::writeXML(outputErrorsFile,calibErrorFileHeader,calibErrors);


}


