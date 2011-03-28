#/bin/bash
grep SELECTED $@ |\
sed 's/^.*SELECTED://' |\
sed '/kRatio/ s/^/# /'