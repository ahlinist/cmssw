#!/bin/bash
bsub -q 8nh 'bash batchRun.sh 1000 0 CleanTB_Events_1000GeV_barrel_0T.root'
bsub -q 8nh 'bash batchRun.sh 1000 1 CleanTB_Events_1000GeV_endcaps_0T.root'
bsub -q 8nh 'bash batchRun.sh 10 0 CleanTB_Events_10GeV_barrel_0T.root'
bsub -q 8nh 'bash batchRun.sh 10 1 CleanTB_Events_10GeV_endcaps_0T.root'

