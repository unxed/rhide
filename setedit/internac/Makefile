# 'make languages' will build the language files
# 'make install.languages' installs the language files at the correct place

CXX=g++
CXXFLAGS=-O2 -Wall -Werror -gstabs+3 -pipe

ifneq ($(strip $(DJDIR)),)
export editor_OS:=DJGPP
endif

# If not DOS then just Linux, no other supported by now
ifeq ($(strip $(editor_OS)),)
export editor_OS:=Linux
endif

srcdir=.
LANGUAGES=es de
PACKAGE=setedit
prefix=$(DJDIR)
locale_dir=$(prefix)/share/locale

po_list_l=$(shell cat po_list)

ifeq ($(editor_OS),DJGPP)
# To avoid problems with command line length limit
po_list_cl=@po_list
update_p=update
else
update_p=cp
po_list_cl=$(po_list_l)
endif

po_subdirs=

CFLAGS=-O

ifdef MAINTAINER_MODE
PO_DEPS=potfiles.po fix.exe $(addprefix h_,$(addsuffix .po,$(LANGUAGES)))
POTFILES_DEPS=po_list $(po_list_l)
else
PO_DEPS=
POTFILES_DEPS=
endif

all: $(addsuffix .mo,$(LANGUAGES))

ifeq ($(xgettext),)
xgettext=xgettext
endif

ifeq ($(msgmerge),)
msgmerge=msgmerge
endif

ifeq ($(msgfmt),)
msgfmt=msgfmt
endif

ifeq ($(libintl),)
libintl=libintl.a
endif

getcolors.exe: getcolors.cc
	$(CXX) -o getcolors.exe -s $(CXXFLAGS) -I../setedit/include getcolors.cc

colors.cc: getcolors.exe ../setedit/include/pal.h
	./getcolors.exe > colors.cc

po_list: colors.cc ../makes/lista
	perl makelist.pl

clean:
	rm -f potfiles.po
	rm -f $(addsuffix .mo,$(LANGUAGES))
	rm -f $(addsuffix .po,$(LANGUAGES))
	rm -f po_list

potfiles.po: $(POTFILES_DEPS)
#@echo "*** Ignore warnings about unterminated strings, is a bug in gettext."
#@echo "*** Ulrich Drepper, the maintainer, refuses to fix it."
	$(xgettext) --default-domain=dummy --add-comments --keyword=_ \
	  --keyword=__ --directory=$(srcdir) --omit-header \
	  --add-location $(po_list_cl)
	touch dummy.po
	$(update_p) dummy.po $@
	rm dummy.po

T=$(subst /,\,$@)

fix.exe: fix.c
	$(CXX) -o fix.exe -s $(CXXFLAGS) fix.c

$(addsuffix .po,$(LANGUAGES)): $(PO_DEPS)
#if not exist $(T) cp $^ $@
	touch $@
	./fix.exe $(addprefix h_,$@) $< __.pot
	-$(msgmerge) --force-po -o __.pon $@ __.pot
	-$(update_p) __.pon $@
	rm -f __.pon
	rm -f __.pot
	./fix.exe $(addprefix h_,$@) $@ $@
#$(addsuffix .mo,$(LANGUAGES)): $(msgfmt)

%.mo: %.po
	-$(msgfmt) -o $@ $<


languages: $(addsuffix .mo,$(LANGUAGES))

# This creates the .po files for each language from $(LANGUAGES)
# See the sample translated file de.po, how to translate
languages.src: $(addsuffix .po,$(LANGUAGES))

#if you don't have the fileutils 3.13, you must copy the files manually
%.ins: %.mo
	ginstall -d $(locale_dir)/$*/LC_MESSAGES
	-ginstall $< $(locale_dir)/$*/LC_MESSAGES/$(PACKAGE).mo

install.languages.start:
	rm -f languages.log

install: languages install.languages.start \
			$(addsuffix .ins,$(LANGUAGES))

