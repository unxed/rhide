# Generated automatically by the configure script

ifneq ($(strip $(prefix)),)
  MPREFIX=$(prefix)
else
  MPREFIX=/usr
endif
ifeq ($(INSTALL),)
  INSTALL=install
endif
libdir=$(MPREFIX)/lib
CFLAGS=-O2 -Wall -Werror -gstabs+3 -pipe
CXXFLAGS=-O2 -Wall -Werror -gstabs+3 -pipe
SET_USE_FHS=no
MAINTAINER_MODE=1
export

.PHONY: needed infview libmpegsnd internac doc-basic holidays

all: Makefile editor libset infview


Makefile: config.pl conflib.pl
	$(error Please reconfigure the package! Alternative: "touch Makefile")

libmpegsnd:
	$(MAKE) -C mp3/mpegsound -f mpegsnd.mkf

internac:
	$(MAKE) -C internac

doc-basic:
	$(MAKE) -C doc txt info

# Libraries not created by RHIDE projects
needed: libmpegsnd holidays include/vername.h internac doc-basic

editor: needed
	$(MAKE) -C makes

libset:
	$(MAKE) -C makes libset

infview:
	$(MAKE) -C makes infview

holidays:
	$(MAKE) -C holidays

include/vername.h: change.log
	perl updaterev.pl

install-editor: editor
	$(MAKE) -C makes install "EXTRA_INS_OPS=--no-compress "

install-libset: libset
	$(INSTALL) -d -m 0755 $(libdir)
	$(INSTALL) -m 0644 makes/libset.a $(libdir)


install-infview: infview
	$(MAKE) -C makes install-infview "EXTRA_INS_OPS=--no-compress "

install: install-editor install-libset install-infview


distrib-editor: needed
	$(MAKE) -C makes distrib  "EXTRA_INS_OPS=--no-compress "

distrib-source:
	$(MAKE) -C makes distrib-source  "EXTRA_INS_OPS=--no-compress "

distrib-infview: needed
	$(MAKE) -C makes distrib-infview  "EXTRA_INS_OPS=--no-compress "

distrib: distrib-editor distrib-infview

clean:
	cd makes; $(MAKE) clean-o; $(MAKE) clean-docs; cd ..
	rm -f configure.cache
	rm -f Makefile
	rm -f errormsg.txt
	rm -f doc/sdh.exe
	rm -f include/configed.h
	rm -rf makes/linux/result makes/linux/resultInf makes/linux/setedit-* makes/linux/infview-*
	rm -f makes/*.a makes/*.exe makes/*.gdt
	rm -f mp3/libamp/*.a mp3/libamp/obj/*.o
	rm -f mp3/mpegsound/*.a mp3/mpegsound/obj/*.o
	$(MAKE) -C holidays clean
