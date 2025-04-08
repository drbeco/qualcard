# Autoversion makefile
# (C) 2015-2016, Ruben Carlo Benante

.PHONY: clean cleanall
.PRECIOUS: %.o
SHELL=/bin/bash -o pipefail

MAJOR = 1
MINOR = 8
BUILD = $(shell date +"%g%m%d.%H%M%S")
DEFSYM = $(subst .,_,$(BUILD))
VERSION = "\"$(MAJOR).$(MINOR).$(BUILD)\""
CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -c -std=gnu99 -Wno-format-truncation
# -Wno-unused-variable -Wno-unused-function
#CFLAGS = -Wall -Wextra -Ofast -c -std=gnu99
#-ansi -pedantic-errors -c -Ofast
CPPFLAGS = -DVERSION=$(VERSION) -DBUILD="\"$(BUILD)\""
LDLIBS = -Wl,--defsym,BUILD_$(DEFSYM)=0 -lm -lgmp
#OBJ = libeco-ux64.o
LIBECO_BUILD = $(shell date +"%g%m%d.%H%M%S")
LIBECO_VERSION = 1.0
LIBECO_CFLAGS = -Ofast -c -Wno-unused-variable -Wno-unused-function 
LIBECO_CPPFLAGS = -DDEBUG=0 -DVERSION=$(LIBECO_VERSION) -DBUILD=$(LIBECO_BUILD)
LIBECO_LDLIBS = -lm
o = qualcard

#object
%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ |& tee errors.err

#binary
$(o) : % : %.o $(OBJ)
	$(CC) $(LDLIBS) $^ -o $@ |& tee errors.err
	echo $(o) version $(VERSION) > VERSION

#library
libeco-ux64.o : libeco.c
	$(CC) $(LIBECO_CFLAGS) $(LIBECO_CPPFLAGS) $(LDLIBS) libeco.c -o libeco-ux64.o

clean:
	rm -f *.o errors.err

cleanall:
	rm -i $(o) *.x *.o errors.err
