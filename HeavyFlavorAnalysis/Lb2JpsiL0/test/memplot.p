set key top left
set xlabel "Events as reported by SimpleMemoryCheck"
set ylabel "memory footprint / MB"
set term postscript color
set out "memplot.ps"
plot "nofix.txt", "withfix.txt", "with2fix.txt"

