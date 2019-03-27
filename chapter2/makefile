CC=gcc
IDIR=./
CFLAGS=-I$(IDIR)
CFLAGS:=-std=gnu99
LDIR=
ODIR=obj
LIBS=-lm
DEBUG=-g
MACRO=__DEBUG__


_DEPS=
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=chapter2.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) $(DEBUG) -D$(MACRO) -c -o $@ $< $(CFLAGS)

test_chapter2:$(OBJ)
	$(CC) $(DEBUG) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY:clean

clean:
	rm -f test_chapter2 $(ODIR)/*.o *~ core $(INCDIR)/*~
