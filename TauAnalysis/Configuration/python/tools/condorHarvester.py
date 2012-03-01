#!/usr/bin/python

import sys,os,stat,subprocess,time

if 'condor' not in os.listdir('.'):
    print 'Creating directory "condor"'
    os.mkdir('condor')

run = sys.argv[1].rsplit('Run',1)[1][0:2]

baseName = sys.argv[1].rsplit('/',1)[1].rstrip('.root')
submitFileName = 'condor/' + baseName + '.submit'
jobFileName = 'condor/' + baseName + '.sh'

subFile = open(submitFileName,'w')
jobFile = open(jobFileName,'w')

subFile.write('universe = vanilla\n')
subFile.write('log = condor/' + baseName + '.log\n')
subFile.write('output = condor/' + baseName + '.out\n')
subFile.write('executable = ' + jobFileName + '\n')
subFile.write('getenv = true\n')
subFile.write('Queue 1\n')
subFile.write('\n')
subFile.close()

jobFile.write('#!/bin/sh\n')
jobFile.write('cd ~/work/Run' + run + '\n')
jobFile.write('TauAnalysis/Configuration/python/tools/genericHarvester.py ' + '%s' % ' '.join(sys.argv[1:]) +'\n')
jobFile.write('return 0')
jobFile.close()

os.system('chmod 777 ' + jobFileName)

print 'Submitting ' + submitFileName + ' to condor\n'
condorJobID =  subprocess.Popen(['condor_submit','./'+submitFileName],stdout=subprocess.PIPE).communicate()[0].split('\n')[2].split(' ')[-1].rstrip('.')

print 'Running as condor job ' + condorJobID + '\n'
finished = False
while not finished:
    if condorJobID not in subprocess.Popen(['condor_q','jkolb'],stdout=subprocess.PIPE).communicate()[0]:
        finished = True
    time.sleep(30)
print submitFileName + ' has finished\n'

