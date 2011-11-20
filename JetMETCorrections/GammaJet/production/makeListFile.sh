#!/bin/bash

rfdir $1 | awk '{printf "\"rfio://'$1'/%s\",",$9}' | sed 's/\(.*\)./\1\n/' > $2.txt
