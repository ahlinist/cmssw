import ROOT
import re
import os

_passed_matcher = re.compile(
    r'<passed_cumulative_numWeighted#a1#s1>f=(?P<value>[0-9\.]*)</passed_cumulative_numWeighted#a1#s1>')
_processed_matcher = re.compile(
    r'<processed_numWeighted#a1#s1>f=(?P<value>[0-9\.]*)</processed_numWeighted#a1#s1>')

def get_processed_and_passed_events(filename, filter_stats, filter):
    file = ROOT.TFile(file, "READ")
    filter_location = os.path.normpath(
        "/".join(["DQMData/", filter_stats, filter]))
    filter_stats_dir = file.Get(filter_location)
    passed = None
    processed = None
    for key in filter_stats_dir.GetListOfKeys():
        passed_match = _passed_matcher.match(key.GetName())
        if passed_match:
            passed = float(passed_match.group('value'))
        processed_match = _processed_matcher.match(key.GetName())
        if processed_match:
            processed = float(processed_match.group('value'))
    file.Close()
    return (passed, processed)
