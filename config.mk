###############################################################################
# Project:  COCKTAIL training
# Descr:    common definitions used by all Makefiles
# Author: Dr. Juergen Vollmer <Juergen.vollmer@informatik-vollmer.de>
# $Id: config.mk,v 1.16 2015/03/31 13:19:34 vollmer Exp $
###############################################################################

###############################################################################
# This configuration file has to be used as following:
# Add to your Makefile at the very beginning:
#-------------------------------------------
#     MAIN   = // name of main program
#
#     SRC_C  = // list of handwritten C-sources (*.c files)
#     SRC_H  = // list of handwritten C-sources (*.h files)
#     SRC    = // list of handwritten specifications
#
#     GEN_C  = // list of generated C-sources (*.c files)
#     GEN_H  = // list of generated C-sources (*.h files)
#     GEN    = // list of other generated files
#
#     include ../config.mk
#
#     all: $(MAIN)
#
#     # your other make file rules
#
#     include ../common.mk
#
#-------------------------------------------
# Some sources 'x.y' contain only a implementation stub, which should be
# completed as an exercise.
# The file 'x-solution.y' contains the solution of the exercise.
# Calling MAKE with SOLUTION="-solution" will use that sources.
# If Makefile-variable SOLUTION is not set, the stub source will be used.
# Source files with a stub and a complete implementation are denoted in
# the Makefiles as: x$(SOLUTION).y
# To create a parser with the solution sources, call
#	make solution
# SOLUTION        = -solution
# SOLUTION_CFLAGS = -DSOLUTION
SOLUTION =
SOLUTION_CFLAGS=
#-------------------------------------------
###############################################################################
# The common Makefile goals are:
#   all        : create your system
#   solution   : create the system with the solution sources
#   gen        : generate all needed source files
#   test       : run a simple test suite
#   depend     : create a .depend file containing the dependencies
#   clean      : remove all generated files
#   realclean  : remove all generated files and some more
###############################################################################

.PHONY: all solution gen test clean realclean test

###############################################################################

# which system are we on?
UNAME	      = $(shell uname)
ARCH	      = $(shell uname -m)
SYSTEM        = XXXXX

# Any Linux based system
ifeq ($(UNAME), Linux)
SYSTEM       = linux
endif

# Windows 2000
ifeq ($(UNAME), CYGWIN_NT-5.0)
SYSTEM       = windows
endif

# Windows XP
ifeq ($(UNAME), CYGWIN_NT-5.1)
SYSTEM       = windows
endif

# Windows Vista
ifeq ($(UNAME), CYGWIN_NT-6.0)
SYSTEM       = windows
endif

ifeq ($(UNAME), Darwin)
SYSTEM       = darwin
endif

ifeq ($(SYSTEM), XXXXX)
START:
	echo This is an unknown system
endif

###############################################################################

# configure this to your needs, may be set also in $(HOME)/.bashrc
# Location of the cocktail toobox
COCKTAIL	= /home/cocktail

# setting the PATH is GNU-specific, also in $(HOME)/.bashrc
PATH 		:= $(PATH):$(COCKTAIL)/bin:../rPrepro:../../rPrepro:/etc/alternatives/

# common used flags for LARK
LARK_OPTS =
# w explain implicitly and dynamically repaired conflicts, only
# D explain new conflicts, only
# U explain explicitly  repaired conflicts
# V explain implicitly  repaired conflicts
# W explain LALR(2)     repaired conflicts
# X explain dynamically repaired conflicts
# 5 generate parser with graphical visualization
# s suppress informations and warnings
# S print messages (do not store)
# c generate C         source code
# d generate header file or definition module
# i generate implementation part or module

# LARK / grammar debugging, to produce output set "Parser_Debug = rtrue"
C_FLAGS_LARK	= -DYYDEBUG

# common used flags for AST
# AST options to generate some debugging help           // use CC #defines
# -k generate graph type checker   procedure CheckTREE	  -DCHECKTREE
# -q generate ASCII graph browser  procedure QueryTREE    -DQUERYTREE
# -e generate graphic browser      procedure DrawTREE     -DDRAWTREE
# +D generate graphic browser procedure DrawTREE enclosed in #ifdef DRAWTREE

# -g generate binary graph reader  procedure GetTREE      -DGETTREE
# -p generate binary graph writer  procedure PutTREE      -DPUTTREE
# -r generate ASCII  graph reader  procedure ReadTREE     -DREADTREE
# -w generate ASCII  graph writer  procedure WriteTREE    -DWRITETREE
C_FLAGS_AST	= -DDRAWTREE -DCHECKTREE -DWRITETREE

###############################################################################
# C-compiler and its flags

# Tcl/Tk include files and libraries
# use if the AST or Parser broser should be used
ifeq ($(SYSTEM), linux)
  ifeq ($(ARCH), x86_64)
    TCL_VER = $(shell basename `ls /usr/lib64/libtcl*.so|head -n1` .so | sed -e's/[a-z]*//')
  else
    TCL_VER = $(shell basename `ls /usr/lib/libtcl*.so|head -n1` .so | sed -e's/[a-z]*//')
  endif
TCL		= /usr/X11R6
TCL_INC		= -I $(TCL)/include
TCL_LIB		= -L$(TCL)/lib -ltcl$(TCL_VER) -ltk$(TCL_VER) -lX11
endif

ifeq ($(SYSTEM), windows)
TCL_INC	= -I/usr/include
TCL_LIB	= -ltcl84 -ltk84
endif

ifeq ($(SYSTEM), darwin)
TCL_INC	= -I/usr/X11/include
TCL_LIB	= -ltcl8.4 -ltk8.4
endif

# Operating system specific linker flags
ifeq ($(SYSTEM), linux)
SYS_LIB	= -ldl
endif
ifeq ($(SYSTEM), windows)
SYS_LIB =
endif
ifeq ($(SYSTEM), darwin)
SYS_LIB =
endif

# search util.c in main directory
VPATH		= .:..

# C-compiler and linker flags
# use    make CFLAGS=...   to specify further C-compiler flags like
# -DDEBUG

CC		= gcc
X_DEBUG		= -g -Wall -ansi -pedantic
		# -g:        Debugging the program
                # -DDEBUG    Use this as you need
		# -Wall:     lots's of warnings   GNU-specific
		# -pedantic  be more pedantic     GNU-specific
X_CFLAGS	= -I.. -I$(COCKTAIL)/lib/cocktail/include $(TCL_INC)	 	\
		  $(X_DEBUG) $(C_FLAGS_AST) $(C_FLAGS_LARK) 			\
		  $(SOLUTION_CFLAGS) $(CFLAGS)
X_LDFLAGS	= -L$(COCKTAIL)/lib/cocktail -lreuse $(TCL_LIB) $(SYS_LIB) \
		  $(LDFLAGS)

###############################################################################

# list of all object files, based on the variables $(GEN_C) and $(SRC_C)
OBJ		= $(addsuffix .o, $(basename  $(notdir $(GEN_C) $(SRC_C))))

###############################################################################

# JAVA needs another suffix for solutions..... --> MainSolution
SOLUTION_SUFFIX = -solution

JAVAC		= javac

###############################################################################
