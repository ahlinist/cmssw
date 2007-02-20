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

void PGlobalDigi::putESCalDigis(std::vector<float> adc0,
				std::vector<float> adc1,
				std::vector<float> adc2,
				std::vector<float> she)
{
  nESCalDigis = adc0.size();
  ESCalDigis.resize(nESCalDigis);
  for (int i = 0; i < nESCalDigis; ++i) {
    ESCalDigis[i].ADC0 = adc0[i];
    ESCalDigis[i].ADC1 = adc1[i];
    ESCalDigis[i].ADC2 = adc2[i];
    ESCalDigis[i].SHE = she[i];
  }

  return;
}

void PGlobalDigi::putHBCalDigis(std::vector<float> aee,
				std::vector<float> she)
{
  nHBCalDigis = aee.size();
  HBCalDigis.resize(nHBCalDigis);
  for (int i = 0; i < nHBCalDigis; ++i) {
    HBCalDigis[i].AEE = aee[i];
    HBCalDigis[i].SHE = she[i];
  }

  return;
}

void PGlobalDigi::putHECalDigis(std::vector<float> aee,
				std::vector<float> she)
{
  nHECalDigis = aee.size();
  HECalDigis.resize(nHECalDigis);
  for (int i = 0; i < nHECalDigis; ++i) {
    HECalDigis[i].AEE = aee[i];
    HECalDigis[i].SHE = she[i];
  }

  return;
}

void PGlobalDigi::putHOCalDigis(std::vector<float> aee,
				std::vector<float> she)
{
  nHOCalDigis = aee.size();
  HOCalDigis.resize(nHOCalDigis);
  for (int i = 0; i < nHOCalDigis; ++i) {
    HOCalDigis[i].AEE = aee[i];
    HOCalDigis[i].SHE = she[i];
  }

  return;
}

void PGlobalDigi::putHFCalDigis(std::vector<float> aee,
				std::vector<float> she)
{
  nHFCalDigis = aee.size();
  HFCalDigis.resize(nHFCalDigis);
  for (int i = 0; i < nHFCalDigis; ++i) {
    HFCalDigis[i].AEE = aee[i];
    HFCalDigis[i].SHE = she[i];
  }

  return;
}

void PGlobalDigi::putTIBL1Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTIBL1Digis = adc.size();
  TIBL1Digis.resize(nTIBL1Digis);
  for (int i = 0; i < nTIBL1Digis; ++i) {
    TIBL1Digis[i].ADC = adc[i];
    TIBL1Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTIBL2Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTIBL2Digis = adc.size();
  TIBL2Digis.resize(nTIBL2Digis);
  for (int i = 0; i < nTIBL2Digis; ++i) {
    TIBL2Digis[i].ADC = adc[i];
    TIBL2Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTIBL3Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTIBL3Digis = adc.size();
  TIBL3Digis.resize(nTIBL3Digis);
  for (int i = 0; i < nTIBL3Digis; ++i) {
    TIBL3Digis[i].ADC = adc[i];
    TIBL3Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTIBL4Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTIBL4Digis = adc.size();
  TIBL4Digis.resize(nTIBL4Digis);
  for (int i = 0; i < nTIBL4Digis; ++i) {
    TIBL4Digis[i].ADC = adc[i];
    TIBL4Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTOBL1Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTOBL1Digis = adc.size();
  TOBL1Digis.resize(nTOBL1Digis);
  for (int i = 0; i < nTOBL1Digis; ++i) {
    TOBL1Digis[i].ADC = adc[i];
    TOBL1Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTOBL2Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTOBL2Digis = adc.size();
  TOBL2Digis.resize(nTOBL2Digis);
  for (int i = 0; i < nTOBL2Digis; ++i) {
    TOBL2Digis[i].ADC = adc[i];
    TOBL2Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTOBL3Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTOBL3Digis = adc.size();
  TOBL3Digis.resize(nTOBL3Digis);
  for (int i = 0; i < nTOBL3Digis; ++i) {
    TOBL3Digis[i].ADC = adc[i];
    TOBL3Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTOBL4Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTOBL4Digis = adc.size();
  TOBL4Digis.resize(nTOBL4Digis);
  for (int i = 0; i < nTOBL4Digis; ++i) {
    TOBL4Digis[i].ADC = adc[i];
    TOBL4Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTIDW1Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTIDW1Digis = adc.size();
  TIDW1Digis.resize(nTIDW1Digis);
  for (int i = 0; i < nTIDW1Digis; ++i) {
    TIDW1Digis[i].ADC = adc[i];
    TIDW1Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTIDW2Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTIDW2Digis = adc.size();
  TIDW2Digis.resize(nTIDW2Digis);
  for (int i = 0; i < nTIDW2Digis; ++i) {
    TIDW2Digis[i].ADC = adc[i];
    TIDW2Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTIDW3Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTIDW3Digis = adc.size();
  TIDW3Digis.resize(nTIDW3Digis);
  for (int i = 0; i < nTIDW3Digis; ++i) {
    TIDW3Digis[i].ADC = adc[i];
    TIDW3Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW1Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW1Digis = adc.size();
  TECW1Digis.resize(nTECW1Digis);
  for (int i = 0; i < nTECW1Digis; ++i) {
    TECW1Digis[i].ADC = adc[i];
    TECW1Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW2Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW2Digis = adc.size();
  TECW2Digis.resize(nTECW2Digis);
  for (int i = 0; i < nTECW2Digis; ++i) {
    TECW2Digis[i].ADC = adc[i];
    TECW2Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW3Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW3Digis = adc.size();
  TECW3Digis.resize(nTECW3Digis);
  for (int i = 0; i < nTECW3Digis; ++i) {
    TECW3Digis[i].ADC = adc[i];
    TECW3Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW4Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW4Digis = adc.size();
  TECW4Digis.resize(nTECW4Digis);
  for (int i = 0; i < nTECW4Digis; ++i) {
    TECW4Digis[i].ADC = adc[i];
    TECW4Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW5Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW5Digis = adc.size();
  TECW5Digis.resize(nTECW5Digis);
  for (int i = 0; i < nTECW5Digis; ++i) {
    TECW5Digis[i].ADC = adc[i];
    TECW5Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW6Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW6Digis = adc.size();
  TECW6Digis.resize(nTECW6Digis);
  for (int i = 0; i < nTECW6Digis; ++i) {
    TECW6Digis[i].ADC = adc[i];
    TECW6Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW7Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW7Digis = adc.size();
  TECW7Digis.resize(nTECW7Digis);
  for (int i = 0; i < nTECW7Digis; ++i) {
    TECW7Digis[i].ADC = adc[i];
    TECW7Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putTECW8Digis(std::vector<float> adc,
			        std::vector<int> strip)
{
  nTECW8Digis = adc.size();
  TECW8Digis.resize(nTECW8Digis);
  for (int i = 0; i < nTECW8Digis; ++i) {
    TECW8Digis[i].ADC = adc[i];
    TECW8Digis[i].STRIP = strip[i];
  }

  return;
}

void PGlobalDigi::putBRL1Digis(std::vector<float> adc,
			       std::vector<int> row,
			       std::vector<int> column)
{
  nBRL1Digis = adc.size();
  BRL1Digis.resize(nBRL1Digis);
  for (int i = 0; i < nBRL1Digis; ++i) {
    BRL1Digis[i].ADC = adc[i];
    BRL1Digis[i].ROW = row[i];
    BRL1Digis[i].COLUMN = column[i];
  }

  return;
}

void PGlobalDigi::putBRL2Digis(std::vector<float> adc,
			       std::vector<int> row,
			       std::vector<int> column)
{
  nBRL2Digis = adc.size();
  BRL2Digis.resize(nBRL2Digis);
  for (int i = 0; i < nBRL2Digis; ++i) {
    BRL2Digis[i].ADC = adc[i];
    BRL2Digis[i].ROW = row[i];
    BRL2Digis[i].COLUMN = column[i];
  }

  return;
}

void PGlobalDigi::putBRL3Digis(std::vector<float> adc,
			       std::vector<int> row,
			       std::vector<int> column)
{
  nBRL3Digis = adc.size();
  BRL3Digis.resize(nBRL3Digis);
  for (int i = 0; i < nBRL3Digis; ++i) {
    BRL3Digis[i].ADC = adc[i];
    BRL3Digis[i].ROW = row[i];
    BRL3Digis[i].COLUMN = column[i];
  }

  return;
}

void PGlobalDigi::putFWD1pDigis(std::vector<float> adc,
				std::vector<int> row,
				std::vector<int> column)
{
  nFWD1pDigis = adc.size();
  FWD1pDigis.resize(nFWD1pDigis);
  for (int i = 0; i < nFWD1pDigis; ++i) {
    FWD1pDigis[i].ADC = adc[i];
    FWD1pDigis[i].ROW = row[i];
    FWD1pDigis[i].COLUMN = column[i];
  }

  return;
}

void PGlobalDigi::putFWD1nDigis(std::vector<float> adc,
				std::vector<int> row,
				std::vector<int> column)
{
  nFWD1nDigis = adc.size();
  FWD1nDigis.resize(nFWD1nDigis);
  for (int i = 0; i < nFWD1nDigis; ++i) {
    FWD1nDigis[i].ADC = adc[i];
    FWD1nDigis[i].ROW = row[i];
    FWD1nDigis[i].COLUMN = column[i];
  }

  return;
}

void PGlobalDigi::putFWD2pDigis(std::vector<float> adc,
				std::vector<int> row,
				std::vector<int> column)
{
  nFWD2pDigis = adc.size();
  FWD2pDigis.resize(nFWD2pDigis);
  for (int i = 0; i < nFWD2pDigis; ++i) {
    FWD2pDigis[i].ADC = adc[i];
    FWD2pDigis[i].ROW = row[i];
    FWD2pDigis[i].COLUMN = column[i];
  }

  return;
}

void PGlobalDigi::putFWD2nDigis(std::vector<float> adc,
				std::vector<int> row,
				std::vector<int> column)
{
  nFWD2nDigis = adc.size();
  FWD2nDigis.resize(nFWD2nDigis);
  for (int i = 0; i < nFWD2nDigis; ++i) {
    FWD2nDigis[i].ADC = adc[i];
    FWD2nDigis[i].ROW = row[i];
    FWD2nDigis[i].COLUMN = column[i];
  }

  return;
}

void PGlobalDigi::putMB1Digis(std::vector<int> wire,
			      std::vector<float> time)
{
  nMB1Digis = wire.size();
  MB1Digis.resize(nMB1Digis);
  for (int i = 0; i < nMB1Digis; ++i) {
    MB1Digis[i].WIRE = wire[i];
    MB1Digis[i].TIME = time[i];
  }

  return;
}

void PGlobalDigi::putMB2Digis(std::vector<int> wire,
			      std::vector<float> time)
{
  nMB2Digis = wire.size();
  MB2Digis.resize(nMB2Digis);
  for (int i = 0; i < nMB2Digis; ++i) {
    MB2Digis[i].WIRE = wire[i];
    MB2Digis[i].TIME = time[i];
  }

  return;
}

void PGlobalDigi::putMB3Digis(std::vector<int> wire,
			      std::vector<float> time)
{
  nMB3Digis = wire.size();
  MB3Digis.resize(nMB3Digis);
  for (int i = 0; i < nMB3Digis; ++i) {
    MB3Digis[i].WIRE = wire[i];
    MB3Digis[i].TIME = time[i];
  }

  return;
}

void PGlobalDigi::putMB4Digis(std::vector<int> wire,
			      std::vector<float> time)
{
  nMB4Digis = wire.size();
  MB4Digis.resize(nMB4Digis);
  for (int i = 0; i < nMB4Digis; ++i) {
    MB4Digis[i].WIRE = wire[i];
    MB4Digis[i].TIME = time[i];
  }

  return;
}

void PGlobalDigi::putCSCstripDigis(std::vector<float> adc)
{
  nCSCstripDigis = adc.size();
  CSCstripDigis.resize(nCSCstripDigis);
  for (int i = 0; i < nCSCstripDigis; ++i) {
    CSCstripDigis[i].ADC = adc[i];
  }

  return;
}

void PGlobalDigi::putCSCwireDigis(std::vector<float> time)
{
  nCSCwireDigis = time.size();
  CSCwireDigis.resize(nCSCwireDigis);
  for (int i = 0; i < nCSCwireDigis; ++i) {
    CSCwireDigis[i].TIME = time[i];
  }

  return;
}
