
datadir=$CMSSW_BASE/src/RecoHI/HiCentralityAlgos/data

input=../data/CentralityTables.root
output=$datadir/CentralityTables.db

for tag in \
HFhits40_MC_Hydjet4TeV_MC_3XY_V24_v0 \
EEbcs40_MC_Hydjet4TeV_MC_3XY_V24_v0 \
HFhits20_MC_Hydjet4TeV_MC_3XY_V24_v0 \
EEbcs20_MC_Hydjet4TeV_MC_3XY_V24_v0 \
HFhits10_MC_Hydjet4TeV_MC_3XY_V24_v0 \
EEbcs10_MC_Hydjet4TeV_MC_3XY_V24_v0 \
HFhits5_MC_Hydjet4TeV_MC_3XY_V24_v0 \
EEbcs5_MC_Hydjet4TeV_MC_3XY_V24_v0 \
HFhits40_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs40_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits20_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs20_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits10_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs10_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits5_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs5_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits40_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs40_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits20_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs20_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits10_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs10_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits5_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
EEbcs5_MC_Hydjet2760GeV_MC_3XY_V24_v0 \
HFhits40_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
EEbcs40_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
HFhits20_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
EEbcs20_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
HFhits10_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
EEbcs10_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
HFhits5_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
EEbcs5_MC_Hydjet2760TeV_MC_3XY_V24_NoZS_v0 \
HFhits40_MC_AMPT2760GeV_MC_3XY_V24_v0 \
EEbcs40_MC_AMPT2760GeV_MC_3XY_V24_v0 \
HFhits20_MC_AMPT2760GeV_MC_3XY_V24_v0 \
EEbcs20_MC_AMPT2760GeV_MC_3XY_V24_v0 \
HFhits10_MC_AMPT2760GeV_MC_3XY_V24_v0 \
EEbcs10_MC_AMPT2760GeV_MC_3XY_V24_v0 \
HFhits5_MC_AMPT2760GeV_MC_3XY_V24_v0 \
EEbcs5_MC_AMPT2760GeV_MC_3XY_V24_v0
do
cmsRun makeDBFromTFile.py outputTag=$tag inputFile=$input outputFile=$output
done







