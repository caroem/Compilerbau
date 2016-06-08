###############################################################################
# Project:  COCKTAIL training
# Descr:    common rules used by all Makefiles, for the usage see config.mk
# Author:   Dr. Juergen Vollmer <Juergen.Vollmer@informatik-vollmer.de>
# $Id: common.mk,v 1.23 2015/03/31 13:41:01 vollmer Exp $
###############################################################################

# link the $(MAIN) program
$(MAIN): gen $(OBJ)
	$(CC) -lm $(X_CFLAGS) -o $(MAIN) $(OBJ) $(X_LDFLAGS)

# create all generated sources
gen: $(GEN) $(GEN_C)

# use the "solution" sources:
solution: clean
	$(MAKE) SOLUTION=$(SOLUTION_SUFFIX) SOLUTION_CFLAGS=-DSOLUTION all
	$(MAKE) SOLUTION=$(SOLUTION_SUFFIX) test

test-solution: clean
	$(MAKE) solution
	$(MAKE) SOLUTION=$(SOLUTION_SUFFIX) test

# compile the stuff
.SUFFIXES:	.c .o
.c.o:
	$(CC) $(X_CFLAGS) -c $<

.SUFFIXES:	.java .class
.java.class:
	$(JAVAC)  $<

# create dependencies into the file .depend
.PHONY: depend
depend: clean gen
	@ rm -f .depend
	@ # remove "uninteressting" dependencies
	@ $(CC) $(X_CFLAGS) -MM $(SRC_C) $(GEN_C) |			\
		sed -e"s|/usr/[^ ]*||g" 				\
	            -e"s|/home/[^ ]*||g" 				\
		    -e'/^[ ]*\\[ ]*$$/d' > .depend

.PHONY: clean
clean::
	rm -f $(OBJ) $(GEN) $(GEN_C) $(GEN_H) $(MAIN) $(MAIN).exe \
	      *-solution.o					  \
	      yy* *~ *.dbg *.dlt *.cft core*

.PHONY: solution-clean
solution-clean::
	$(MAKE) clean SOLUTION=$(SOLUTION_SUFFIX)

.PHONY: real-clean
real-clean: clean solution-clean

###############################################################################
# generate and delete stub's and solution sources from input files (contained
# in directory src and having the suffix ".in") using rPrepro
# 	make mk-sources
# generates the stubs ans solution sources (which have the infix "-solution")
#       $(SOLUTION_SUFFIX)
#       make delete-sources
# deletes the sources generated from src/*.in files
###############################################################################

RPREPRO_FLAGS=-n -s -USOLUTION -DSOLUTION_CMT_START='/*' -DSOLUTION_CMT_END='*/'  -PDUMMY

.PHONY: mk-sources
mk-sources::
	$(MAKE) SOLUTION=$(SOLUTION_SUFFIX)  		\
		SOLUTION_SUFFIX=$(SOLUTION_SUFFIX)  	\
		RPREPRO_FLAGS="-n -s -DSOLUTION -PS -DSOLUTION_CMT_START -DSOLUTION_CMT_END"     \
		do-mk-sources
	$(MAKE) SOLUTION=""             		\
		SOLUTION_SUFFIX=$(SOLUTION_SUFFIX)  	\
		RPREPRO_FLAGS="-n -s -USOLUTION -DSOLUTION_CMT_START='/*' -DSOLUTION_CMT_END='*/'  -PDUMMY" \
		do-mk-sources

# internal goal, which does the job
.PHONY: mk-sources
do-mk-sources:: $(SRC_C) $(SRC_H) $(SRC)

%$(SOLUTION).c: src/%.c.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).h: src/%.h.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).pars: src/%.pars.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).scan: src/%.scan.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).rex: src/%.rex.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).ast: src/%.ast.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).puma: src/%.puma.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).ag: src/%.ag.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).java: src/%.java.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

%$(SOLUTION).java: src/%.java.in
	rPrepro $(RPREPRO_FLAGS) -o $@ $<

.PHONY: delete-sources
delete-sources:
	if   [ -d src ];			\
	then for f in `ls src/*.in`;		\
	     do					\
	       rm -f `basename $${f//.in}`;	\
	     done;				\
	     rm -f *solution* *Solution*;	\
	fi

FORCE:

###############################################################################

# include the dependencies, created by
#	$(MAKE) depend
-include .depend

###############################################################################
