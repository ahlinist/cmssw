#
f77=g77
cflags=-O -g -c
link=g77
libs=`cernlib -v pro geant pawlib graflib/X11 packlib mathlib kernlib`
linkopts=-v

all: mixture 

mixture: mixture.o
	$(link) mixture.o $(libs) $(linkopts) -o mixture

mixture.o: mixture.f
	$(f77) $(cflags) mixture.f 

scx0_test: scx0_test.o
	$(link) scx0_test.o $(libs) $(linkopts) -o scx0_test

scx0_test.o: scx0_test.f
	$(f77) $(cflags) scx0_test.f 

clean:
	rm *.o
