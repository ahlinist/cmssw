#!/bin/bash

function die { echo $1: status $2 ;  exit $2; }

partition || die 'partition failed' $?
