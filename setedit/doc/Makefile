#
# Copyright (c) 1997,1998,1999,2000 by Salvador Eduardo Tropea
#
# Make file to generate the documentation for SET's editor
# and the needed utilties
#
# This file was adapted by Robert H�hne to integrate it
# easilly with RHIDE.
#

CC=gcc
# Compat Layer headers location
clheaders=../../tvision/include
# set srcdir to the directory where the sources are
srcdir=.
# set this to the directory, where to search for @include
incdir=.
#extraincdir=$(RHIDESRC)/doc Now I included the files in the distribution
VPATH=$(srcdir)

# define here the wanted doc's
targets=editor sdg infeng
man_targets=setedit infview

# define here for each target the index commandline for
# sdh which is then used in $(sdh_ops) with $(index_$*)
# by replacing $* with the actual target
index_editor=-n "Index of key commands" kindex.idx kindex.tdx
index_sdg=-n "Index of variables and commands" vindex.idx vindex.tdx

# set the commandline options for sdh
sdh_ops=-c -n Index cindex.idx cindex.tdx $(index_$*) \
	-I $(srcdir) -I $(incdir) 

all: all_docs

# I'm using .exe to avoid the sdg. under DOS.
# I think Linux people can live with sdg.exe.
sdh.exe: sdh.c
	$(CC) -s -o $@ -O2 -I $(clheaders) $<

%.inf: %.tx
	./sdh.exe -oi $(sdh_ops) -i $*

%.txt: %.tx
	./sdh.exe -ot $(sdh_ops) -i $*

%.html: %.tx
	./sdh.exe -oh $(sdh_ops) -i $*

%.dvi: %.tx
	./sdh.exe -od $(sdh_ops) -i $*

%.ps: %.tx
	./sdh.exe -op $(sdh_ops) -i $*

# this is only a theoretical suffix only used to produce
# .info .txt and .html with one rule
%.doc: %.tx
	./sdh.exe -othi $(sdh_ops) -i $*

# this is only a theoretical suffix only used to produce
# all the available formats with one rule
%.all: %.tx
	./sdh.exe -ohti $(sdh_ops) -i $*

%.1: %.man
	groff -man -Tascii $< > $@

info: $(addsuffix .inf,$(targets))
txt: $(addsuffix .txt,$(targets))
html: $(addsuffix .html,$(targets))
dvi: $(addsuffix .dvi,$(targets))
ps: $(addsuffix .ps,$(targets))
man: $(addsuffix .1,$(man_targets))

all_suffixes=all doc inf txt html dvi ps

$(foreach suffix,$(all_suffixes),$(addsuffix .$(suffix),$(targets))): sdh.exe

text: txt

all_docs: $(addsuffix .all,$(targets)) man

doc: $(addsuffix .doc,$(targets))

clean-docs:
	rm -f $(addsuffix .inf,$(targets)) $(addsuffix .txt,$(targets)) \
	$(addsuffix .html,$(targets)) $(addsuffix .dvi,$(targets)) \
	$(addsuffix .ps,$(targets))
