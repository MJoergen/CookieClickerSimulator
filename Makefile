sources = main.cc CFloat.cc player.cc

objects = $(sources:.cc=.o)
depends = $(sources:.cc=.d)
CC = g++
OPTIONS  = -W -Wall
OPTIONS  += -O3 -fomit-frame-pointer
#OPTIONS  += -g -pg
#OPTIONS += -fbranch-probabilities
#OPTIONS += -fprofile-arcs
#OPTIONS += -S -fverbose-asm -dA
#DEFINES += -DNDEBUG

CookieClicker: $(objects)
	$(CC) -o $@ $(objects) $(OPTIONS)
	mv $@ $(HOME)/bin

%.d: %.cc Makefile
	set -e; $(CC) -M $(CPPFLAGS) $(DEFINES) $(INCLUDE_DIRS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

include $(depends)

%.o : %.cc Makefile
	$(CC) $(OPTIONS) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@

clean:
	-rm $(objects)
	-rm $(depends)

ctags:
	ctags $(sources) *.h


