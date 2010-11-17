import os
import re

multiStatus = open("status_fnal.out.2", "r").read()

for text in re.split("crab:  Version 2.7.5 running on", multiStatus):
  ## Get the dataset name
  m = re.search(r"\tworking directory\s+(/.+)", text)
  if not m: continue
  path = m.groups()[0]
  head, dataset = os.path.split(path)
  if dataset == "":
    head, dataset = os.path.split(head)
#   print dataset
  ## Get of the list of jobs with Exit Code 1
  exitCode = 1
  pattern = r">>>>>>>>> (\d+) Jobs with Wrapper Exit Code : %d.*"
  pattern += "List of jobs: (\d.*)\n"
  pattern = r""">>>>>>>>> (\d+) Jobs with Wrapper Exit Code : (\d).*
.*List of jobs: (\d.*)
"""
  for nJobs, exitCode, jobList in re.findall(pattern, text):
    if int(exitCode) != 0:
#       print "exit code %s: %s" % (exitCode, jobList)
      print """nohup crab -c %s \\
      -resubmit %s \\
      >& %s/res/resubmit.out &
      """ % (dataset, jobList, dataset)
#   print "--"

if __name__ == "__main__": import user