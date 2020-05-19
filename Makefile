#=========================================================================
# COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
# All rights reserved  
#=========================================================================
# Author:                Eamonn M. Kenny
# Project Manager:       Dr. Peter J. Cullen
# First Modification:    October 2001
# Previous Projects:     none
# Current  Project:      IP2000 STIWRO
# Software Tool Version: 1.0
#=========================================================================

# compiler type
CC=g++

# optimisation or debug mode
OPT=-g -DVERBOSE

# make a library archive
AR= ar

# rule for making object files given a .cpp file
SUFFIXES= .cpp .c .o .hh

.cpp.o: 
	$(CC) -Wall -c $(OPT) $(IFLAGS) $<

# Working directory for executable and files
WORKDIR= runtime
EXEMULTI= $(WORKDIR)/Release/multi.exe
EXEMULTIH= $(WORKDIR)/Release/multih.exe
EXESINGLE= $(WORKDIR)/Release/single.exe
EXESINGLEH= $(WORKDIR)/Release/singleh.exe
EXEGOPTICS= $(WORKDIR)/Release/go.exe
EXEPRINT1= $(WORKDIR)/Release/print1.exe
EXEPRINT2= $(WORKDIR)/Release/print2.exe
EXELP= $(WORKDIR)/Release/lp.exe
EXEPRINT152P= $(WORKDIR)/Release/print152p.exe
EXEPRINT44P= $(WORKDIR)/Release/print44p.exe
EXECONORSHOUSE= $(WORKDIR)/Release/conorshouse.exe
EXECONORSHOUSE28PTS = $(WORKDIR)/Release/conorshouse-28pts.exe
EXEKAVANAGH = $(WORKDIR)/Release/kavanagh-testset.exe

# C++ object files
PROPTOOL= boundary.o building.o complex.o intrinsic.o \
	point3d.o storage.o convex.o ComplexVector.o \
	image.o bts.o diagram.o matlab.o raypath.o plane.o edge.o \
	pointinfo.o segment3d.o reflection.o raytracing.o diffraction.o \
	path-loss.o room.o diffcoeff.o box.o capacity.o \
	parameters.o reloader.o linear-program.o maths-utilities.o \
        cornerdiff.o configuration.o downlink.o
#	path-loss.o room.o diffcoeff.o optimisation.o box.o capacity.o \

TESTHERTZ= testhertz.o ComplexVector.o point3d.o segment3d.o diagram.o \
	complex.o 

LP= main-optimiseh.o
PRINT152P= main-optimise-print152p.o
PRINT44P= main-optimise-print44p.o

CONORSHOUSE= main-conorshouse.o
CONORSHOUSE28PTS = main-conorshouse-28pts.o

SINGLEH= main-singleh.o
SINGLE= main-single.o

MULTI = main-multi.o
MULTIH = main-multih.o

GOPTICS = main-go.o
KAVANAGH = main-kavanagh.o

PRINT1 = main-printing.o
PRINT2 = main-printing2.o

IFLAGS= -I. 
LFLAGS= -L. -L/usr/lib
LIBS= -lboost_system -lboost_filesystem -lm

all: lp print152p print44p multi multih single singleh go print1 print2 conorshouse conorshouse-28pts kavanagh-testset

lp: $(PROPTOOL) $(LP)
	$(CC) -o $(EXELP) $(OPT) $(IFLAGS) $(PROPTOOL) $(LP) \
	$(LFLAGS) $(LIBS)

conorshouse: $(PROPTOOL) $(CONORSHOUSE)
	$(CC) -o $(EXECONORSHOUSE) $(OPT) $(IFLAGS) $(PROPTOOL) $(CONORSHOUSE) \
	$(LFLAGS) $(LIBS)

conorshouse-28pts: $(PROPTOOL) $(CONORSHOUSE28PTS)
	$(CC) -o $(EXECONORSHOUSE28PTS) $(OPT) $(IFLAGS) $(PROPTOOL) $(CONORSHOUSE28PTS) \
	$(LFLAGS) $(LIBS)

kavanagh-testset: $(PROPTOOL) $(KAVANAGH) 
	$(CC) -o $(EXEKAVANAGH) $(OPT) $(IFLAGS) $(PROPTOOL) $(KAVANAGH) \
	$(LFLAGS) $(LIBS)

print152p: $(PROPTOOL) $(PRINT152P)
	$(CC) -o $(EXEPRINT152P) $(OPT) $(IFLAGS) $(PROPTOOL) $(PRINT152P) \
	$(LFLAGS) $(LIBS)

print44p: $(PROPTOOL) $(PRINT44P)
	$(CC) -o $(EXEPRINT44P) $(OPT) $(IFLAGS) $(PROPTOOL) $(PRINT44P) \
	$(LFLAGS) $(LIBS)

multi: $(PROPTOOL) $(MULTI)
	$(CC) -o $(EXEMULTI) $(OPT) $(IFLAGS) $(PROPTOOL) $(MULTI) \
	$(LFLAGS) $(LIBS)

multih: $(PROPTOOL) $(MULTIH)
	$(CC) -o $(EXEMULTIH) $(OPT) $(IFLAGS) $(PROPTOOL) $(MULTIH) \
	$(LFLAGS) $(LIBS)

go: $(PROPTOOL) $(GOPTICS)
	$(CC) -o $(EXEGOPTICS) $(OPT) $(IFLAGS) $(PROPTOOL) $(GOPTICS) \
	$(LFLAGS) $(LIBS)

single: $(PROPTOOL) $(SINGLE)
	$(CC) -o $(EXESINGLE) $(OPT) $(IFLAGS) $(PROPTOOL) $(SINGLE) \
	$(LFLAGS) $(LIBS)

singleh: $(PROPTOOL) $(SINGLEH)
	$(CC) -o $(EXESINGLEH) $(OPT) $(IFLAGS) $(PROPTOOL) $(SINGLEH) \
	$(LFLAGS) $(LIBS)

print1: $(PROPTOOL) $(PRINT1)
	$(CC) -o $(EXEPRINT1) $(OPT) $(IFLAGS) $(PROPTOOL) $(PRINT1) \
	$(LFLAGS) $(LIBS)

print2: $(PROPTOOL) $(PRINT2)
	$(CC) -o $(EXEPRINT2) $(OPT) $(IFLAGS) $(PROPTOOL) $(PRINT2) \
	$(LFLAGS) $(LIBS)

hertz: $(TESTHERTZ) 
	$(CC) -o ./a.exe $(OPT) $(IFLAGS) $(TESTHERTZ) $(LFLAGS) $(LIBS)

# clean up working directories
# removes all object files, executables, plotmtv files, cores and result files
cleansave:
	rm -f $(WORKDIR)/buildings/*.sav

clean:
	rm -f *.o $(WORKDIR)/Release/* $(WORKDIR)/Debug/* \
	$(WORKDIR)/vstudio/*.opt $(WORKDIR)/vstudio/*.ncb $(WORKDIR)/vstudio/*.plg $(WORKDIR)/*dump \
	$(WORKDIR)/*.txt* $(WORKDIR)/buildings/*.sav

# make backup to sub-directory backup (development only)
back:
	cp   *.cpp    ../backup
	cp   Makefile ../backup
	cp   *.hh     ../backup

