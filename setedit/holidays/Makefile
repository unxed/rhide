CC=gcc
CFLAGS=-O2 -Wall -Werror -gstabs+3 -pipe
dyndir=/usr/lib/setedit
dynversion=.1.0.0

LIBS=argentina datetools defholidays

all: $(addsuffix .so,$(LIBS))

libs: $(addsuffix .so,$(LIBS))

%.o: %.c
	$(CC) -c -fPIC $(CFLAGS) $<

%.so: %.o
	$(CC) -o $@ $< -shared -Wl,-soname,$(basename $@).so$(dynversion)

%.ins: %.so
	install -d $(dyndir)
	-install $< $(dyndir)/$@

install: libs $(addsuffix .ins,$(LIBS))

clean:
	rm -f *.o *.so

