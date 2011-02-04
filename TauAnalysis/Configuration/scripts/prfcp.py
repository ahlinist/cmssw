#!/usr/bin/env python

'''

prfcp

Python command wrapper about rfcp, that keeps trying event if it gets a
scheduler error.

From 'man rfcp'

RETURN CODES
       0    Ok.
       1    Bad parameter.
       2    System error.
       3    Unknown error.
       16   Device or resource busy.
       28   No space left on device.
       196  Request killed.
       198  Stager not active.
       200  Bad checksum.


'''

import sys
import subprocess
import shlex

from_file = sys.argv[1]
to_file = sys.argv[2]

max_attempts = 5

# Retry the rfcp job when you get these return codes
retriable_return_codes = set([None, 2, 3, 16])

attempt = 0
return_code = None

print "<prfcp> %s %s" % (from_file, to_file)

while True:
    if return_code is not None:
        print "<prfcp> got code %i, trying again" % return_code
    return_code = subprocess.call(['rfcp', from_file, to_file])
    if return_code not in retriable_return_codes:
        break
    attempt += 1
    if attempt > max_attempts:
        print "<prfcp> reached max number of attempts (%i), returning %i" % (
            max_attempts, return_code)
        break

sys.exit(return_code)
