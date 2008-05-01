

#include <string>

void parseOptions(int argc, char* argv[]);
void parseRequestFile();
void generateModel(std::string colliderRun, std::string pythiaFilename="", std::string sigFilename="");
void produceEmailResult(double logLikelihood);
void produceEmailError(std::string errormessage);
std::string getFilename(std::string type, std::string colliderRun="", std::string experiment="", std::string finalstate="");
void makeTmpDirectory(int iSystematicShift=-3);
void removeTmpDirectory(int iSystematicShift=-3);
void informOurParentProcessThatWeHaveFinishedSuccessfully();
