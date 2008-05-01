#ifndef __Quaero_simulate__
#define __Quaero_simulate__

extern "C" int sortecms_(char* inputHepevtFilename, int inputHepevtFilenameLength);
extern "C" double hepevt2ascii_(char* inputHepevtFilename, char* outputTextFilename, double& generatedLumi, int inputHepevtFilenameLength, int outputTextFilenameLength);
extern "C" double stdhep2ascii_(char* inputHepevtFilename, char* outputTextFilename, double& generatedLumi, int inputHepevtFilenameLength, int outputTextFilenameLength);

#endif
