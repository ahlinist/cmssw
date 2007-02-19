#include "SimDataFormats/GlobalDigiValidation/interface/PGlobalDigi.h"

void PGlobalDigi::putEBCalDigis(std::vector<int> maxpos,
				std::vector<double> aee,
				std::vector<float> she)
{
  nEBCalDigis = maxpos.size();
  EBCalDigis.resize(nEBCalDigis);
  for (int i = 0; i < nEBCalDigis; ++i) {
    EBCalDigis[i].maxPos = maxpos[i];
    EBCalDigis[i].AEE = aee[i];
    EBCalDigis[i].SHE = she[i];
  }

  return;
}

void PGlobalDigi::putEECalDigis(std::vector<int> maxpos,
				std::vector<double> aee,
				std::vector<float> she)
{
  nEECalDigis = maxpos.size();
  EECalDigis.resize(nEECalDigis);
  for (int i = 0; i < nEECalDigis; ++i) {
    EECalDigis[i].maxPos = maxpos[i];
    EECalDigis[i].AEE = aee[i];
    EECalDigis[i].SHE = she[i];
  }

  return;
}

