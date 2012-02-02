#!/usr/bin/env python

'''

prfcp

Python command wrapper about rfcp,
that keeps trying even if it gets a scheduler error.

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

import random
import shlex
import subprocess
import sys
import time

from_file = sys.argv[1]
to_file = sys.argv[2]

max_attempts = 10

# Retry the rfcp job when you get these return codes
retriable_return_codes = set([None, 2, 3, 16])

attempt = 0
return_code = None

print "<prfcp> %s %s" % (from_file, to_file)

random.seed()

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
    # CV: wait for about 5 minutes, in order to prevent submitting too many castor request within too short time,
    #     indicated by message
    #       "stage_get: Maximum number of requests exceeded, you are only permitted to execute 3000 requests in 500 seconds"
    #     in stdout log-files
    time.sleep(random.randint(10, 500))

sys.exit(return_code)
