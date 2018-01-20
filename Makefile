#directory
PBERRDIR=../PBErr
PBMATHDIR=../PBMath
GSETDIR=../GSet
SHAPOIDDIR=../Shapoid

# Build mode
# 0: development (max safety, no optimisation)
# 1: release (min safety, optimisation)
# 2: fast and furious (no safety, optimisation)
BUILDMODE=0

include $(PBERRDIR)/Makefile.inc

INCPATH=-I./ -I$(PBERRDIR)/ -I$(PBMATHDIR)/ -I$(GSETDIR)/ -I$(SHAPOIDDIR)/
BUILDOPTIONS=$(BUILDPARAM) $(INCPATH)

# compiler
COMPILER=gcc

#rules
all : main

main: main.o pberr.o bcurve.o shapoid.o pbmath.o gset.o Makefile 
	$(COMPILER) main.o pberr.o bcurve.o shapoid.o pbmath.o gset.o $(LINKOPTIONS) -o main

main.o : main.c $(PBERRDIR)/pberr.h $(SHAPOIDDIR)/shapoid.h $(PBMATHDIR)/pbmath.h $(GSETDIR)/gset.h bcurve.h bcurve-inline.c Makefile
	$(COMPILER) $(BUILDOPTIONS) -c main.c

bcurve.o : bcurve.c bcurve.h bcurve-inline.c Makefile
	$(COMPILER) $(BUILDOPTIONS) -c bcurve.c

shapoid.o : $(SHAPOIDDIR)/shapoid.c $(SHAPOIDDIR)/shapoid.h $(SHAPOIDDIR)/shapoid-inline.c $(PBMATHDIR)/pbmath.h $(GSETDIR)/gset.h $(PBERRDIR)/pberr.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(SHAPOIDDIR)/shapoid.c

pberr.o : $(PBERRDIR)/pberr.c $(PBERRDIR)/pberr.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(PBERRDIR)/pberr.c

pbmath.o : $(PBMATHDIR)/pbmath.c $(PBMATHDIR)/pbmath-inline.c $(PBMATHDIR)/pbmath.h Makefile $(PBERRDIR)/pberr.h
	$(COMPILER) $(BUILDOPTIONS) -c $(PBMATHDIR)/pbmath.c

gset.o : $(GSETDIR)/gset.c $(GSETDIR)/gset-inline.c $(GSETDIR)/gset.h Makefile $(PBERRDIR)/pberr.h
	$(COMPILER) $(BUILDOPTIONS) -c $(GSETDIR)/gset.c

clean : 
	rm -rf *.o main

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main
	
unitTest :
	main > unitTest.txt; diff unitTest.txt unitTestRef.txt
