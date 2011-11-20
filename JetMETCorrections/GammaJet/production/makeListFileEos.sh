#!/bin/bash

/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select ls $1 | awk '{printf "\"root://eoscms//'$1'/%s\",",$1}' | sed 's/\(.*\)./\1\n/' > $2.txt
