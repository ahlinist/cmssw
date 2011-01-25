import re
import json

ucd_files = [
    'run2010A_for_mike.txt',
    'run2010B_for_mike.txt'
]

uw_files = [
    #'mikes_numbers.txt'
    #'ourAnaOnMikes2010B.txt',
    #'ourAnaOnMikes2010A.txt',
    'our_events_failed_mikes_mu_iso.txt'
]

json_files = [
    '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Nov4ReReco_Collisions10_JSON.txt'
]

# Good run ranges
good_run_ranges = {}
for json_file in json_files:
    good_run = open(json_file, 'r')
    good_run_dict = json.loads(good_run.read())
    for key, val in good_run_dict.iteritems():
        good_run_ranges[int(key)] = val
    good_run.close()

def event_in_json(event_info):
    run, lumi, evt = event_info
    if run in good_run_ranges:
        lumiOK = False
        for lumi_range in good_run_ranges[run]:
            if lumi >= lumi_range[0] and lumi <= lumi_range[1]:
                lumiOK = True
                break
        return lumiOK
    else:
        return False

ucd_matcher = re.compile('(?P<run>\d*):(?P<lumi>\d*):(?P<evt>\d*)')
uw_matcher = re.compile('\*(?P<garbage>[0-9 ]+)\*(?P<run>[0-9 ]+)\*(?P<lumi>[0-9 ]+)\*(?P<evt>[0-9 ]+)\*')
#uw_matcher = ucd_matcher

ucd_events = set([])
uw_events = set([])

def fill(files, matcher, event_set):
    for filename in files:
        file = open(filename, 'r')
        for line in file.readlines():
            match = matcher.match(line)
            if match:
                event_set.add(tuple(
                    map(int, map(match.group, ['run', 'lumi', 'evt']))
                ))

fill(ucd_files, ucd_matcher, ucd_events)
fill(uw_files, uw_matcher, uw_events)

print "Found %i UCD events" % len(ucd_events)
print "Found %i UW events" % len(uw_events)

print "There are %i common events" % len(ucd_events.intersection(uw_events))

ucd_only = ucd_events - uw_events
print "======= UCD only events (%i) ========" % len(ucd_only)
for event in ucd_only:
    print ":".join(map(str, event))

uw_only = uw_events - ucd_events
print "======= UW only events (%i) ========" % len(uw_only)
for event in uw_only:
    print ":".join(map(str, event))

uw_output_file = open('uw_events.txt', 'w')
for event in uw_events:
    uw_output_file.write(":".join(map(str, event)) + "\n")

ucd_only_file = open('ucd_only_events.txt', 'w')
for event in ucd_only:
    ucd_only_file.write(":".join(map(str, event)) + "\n")

uw_only_file = open('uw_only_events.txt', 'w')
for event in uw_only:
    uw_only_file.write(":".join(map(str, event)) + "\n")

print "There are %i UCD  events that fail JSON cut" % len([
    event for event in ucd_events if not event_in_json(event)])
print "There are %i UW events that fail JSON cut" % len([
    event for event in uw_events if not event_in_json(event)])

