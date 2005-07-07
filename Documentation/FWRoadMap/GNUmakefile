############################################################
#
# GNUmakefile for generation of PDF document from LaTeX source.
#
# This is not a very clever makefile; if you have an installation of
# MiKTeX, use 'makefile' instead
#
# $Id: GNUmakefile,v 1.4 2005/03/08 14:05:25 paterno Exp $
#
#
############################################################

all: RoadMap.pdf

RoadMap.dvi: RoadMap.tex GNUmakefile memarticle.cls
	latex  RoadMap.tex
	latex  RoadMap.tex
	latex  RoadMap.tex

# It *should* be better to use dvips -Ppdf, but some TeX 
# installations have a defective 'config.pdf' for dvips
# that causes strange ligature replacements, i.e. "pound Sterling"
# is used in place of the "fi" ligature.
RoadMap.ps: RoadMap.dvi
	dvips  -o RoadMap.ps RoadMap.dvi

RoadMap.pdf: RoadMap.ps
	ps2pdf RoadMap.ps

clean:
	rm -f RoadMap.out RoadMap.toc RoadMap.aux RoadMap.log RoadMap.dvi RoadMap.ps RoadMap.tex.bak RoadMap.tex~ GNUmakefile~

clobber: clean
	rm -f RoadMap.pdf


