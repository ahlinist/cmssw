to run the scropt over the nTuples
1. modify run.py to your needs
2. python run.py -b

once loop is done
1. please rename the histo_*.root files to histo_DATASETNAME.root where DATASETNAME doesn't contain "_", this will be fixed later
2. modify plotHistos.py so it picked all processed histo_*.root files, you can also specify where to write plots and in what format
3. python plotHistos.py -b 